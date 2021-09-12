/*
 * WebManager.cpp
 *
 *  Created on: Nov 10, 2020
 *      Author: 
 */

#include <ConfigManager.h>
#include <EthManager.h>
#include <Rules/RuleObject.h>
#include <Rules/Rules.h>
#include <WebManager.h>
#include <bits/signum-generic.h>
#include <tins/ethernetII.h>
#include <tins/exceptions.h>
#include <tins/hw_address.h>

#include <algorithm>
#include <array>
#include <cstdlib>
#include <fstream>
#include <functional>
#include <iterator>
#include <list>
#include <memory>
#include <nlohmann/json.hpp>
#include <restbed>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

/*
 * Creates a new WebManager object
 */
WebManager::WebManager() {}

/*
 * Destroys the WebManager object
 */
WebManager::~WebManager() {}

/*
 * Function that is responsible for booting up the server side of our web api
 * Sets handlers for get/post
 * Sets our default settings and resource fields
 *
 */
void WebManager::startWebAPI() {
  std::cout << "Starting Web API...\n";

  auto ruleResource = std::make_shared<restbed::Resource>();
  auto statusResource = std::make_shared<restbed::Resource>();
  auto managementResource = std::make_shared<restbed::Resource>();
  auto nicknameResource = std::make_shared<restbed::Resource>();
  auto statsResource = std::make_shared<restbed::Resource>();
  auto vtResource = std::make_shared<restbed::Resource>();

  ruleResource->set_path("/rule");

  statusResource->set_path("/status");

  managementResource->set_path("/settings");

  nicknameResource->set_path("/nickname");

  statsResource->set_path("/statistics");

  ruleResource->set_method_handler(
      "GET", [&](const auto session) { ruleGetHandler(session); });

  ruleResource->set_method_handler(
      "PATCH", [&](const auto session) { rulePatchHandler(session); });

  statusResource->set_method_handler(
      "GET", [&](const auto session) { statusHandler(session); });

  managementResource->set_method_handler(
      "PATCH", [&](const auto session) { settingsPatchHandler(session); });

  managementResource->set_method_handler(
      "GET", [&](const auto session) { settingsGetHandler(session); });

  ruleResource->set_method_handler(
      "POST", [&](const auto session) { filterListPostHandler(session); });

  nicknameResource->set_method_handler(
      "PATCH", [&](const auto session) { nicknamePatchHandler(session); });

  nicknameResource->set_method_handler(
      "GET", [&](const auto session) { nicknameGetHandler(session); });

  statsResource->set_method_handler(
      "GET", [&](const auto session) { statsGetHandler(session); });

  auto settings = std::make_shared<restbed::Settings>();
  settings->set_port(1984);
  settings->set_default_header("Connection", "close");
  settings->set_bind_address(
      ConfigManager::getInstanceConfigManager()->getHOST());
  settings->set_worker_limit(1);  // Do not multithread the requests

  service.set_signal_handler(
      SIGINT, [this](const int signum) { this->service.stop(); });

  service.publish(ruleResource);
  service.publish(statusResource);
  service.publish(managementResource);
  service.publish(nicknameResource);
  service.publish(statsResource);
  service.set_authentication_handler(
      [&](const std::shared_ptr<restbed::Session> session,
          const std::function<void(const std::shared_ptr<restbed::Session>)>
              &callback) { authenticateRequest(session, callback); });
  service.start(settings);
}

/*
 *Retrieves a json object from the patch message sent from the client
 *The json object describes a rule to be carried out/modified in the config
 *The rulePatch handler deserializes the message and handles it
 */
void WebManager::rulePatchHandler(
    const std::shared_ptr<restbed::Session> session) {
  auto request = session->get_request();
  std::string length = request->get_header("Content-length", "0");
  session->fetch(
      std::stoi(length), [](const std::shared_ptr<restbed::Session> session,
                            const restbed::Bytes &body) {
        EthManager *man =
            ConfigManager::getInstanceConfigManager()->getEthernetManager();
        bool firewallState = man->isRunning();

        // Stop the firewall to avoid race conditions.
        man->stopFirewall();

        std::string content;
        try {
          nlohmann::json ruleJson = nlohmann::json::parse(
              std::string((char *)body.data(), body.size()))["rule"];

          if (!ruleJson.empty()) {
            ConfigManager::getInstanceConfigManager()->updateRule(ruleJson);

            content = "success";
          } else {
            content = "error: Missing rule object";
          }
        } catch (nlohmann::json::parse_error &ex) {
          content = "error: invalid json";
        } catch (nlohmann::json::type_error &e) {
          content = "error: missing required parameters";
        } catch (Tins::invalid_address &) {
          content = "error: invalid address";
        } catch (...) {
          content = "An unknown error occurred";
        }

        if (firewallState) {
          man->startFirewallNewThread();
        }

        session->close(restbed::OK, content,
                       {{"Content-Length", std::to_string(content.size())}});
      });
}

/*
 *Receives a get request from the server and sends back the relevant information
 *Information is sent back as a json object. Rule retrieved is dependant on name
 *Name is specified by the front end.
 */
void WebManager::ruleGetHandler(
    const std::shared_ptr<restbed::Session> session) {
  auto request = session->get_request();
  std::string content;

  std::string ruleName = request->get_query_parameter("name", "");
  if (ruleName == "") {
    content = "Error: A rule name must be specified.";
  } else {
    json jRule;
    RuleObject *rule =
        ConfigManager::getInstanceConfigManager()->getRuleObjectByName(
            ruleName);

    if (rule) {
      jRule["enabled"] = rule->isEnabled();
      jRule["block"] = rule->isBlockEnabled();
      jRule["report"] = rule->isReportEnabled();
      jRule["filterBlockList"] = rule->isFilterBlockListEnabled();
      jRule["name"] = ruleName;
      jRule["deviceBlockList"] = rule->isDeviceBlockListEnabled();
      jRule["filterList"] = rule->getFilterListDisplay();

      // Need to get deviceList as strings
      std::vector<std::string> deviceListStrings;
      const std::unordered_set<Tins::EthernetII::address_type, MacHash>
          deviceList = rule->getDeviceList();
      std::transform(
          deviceList.begin(), deviceList.end(),
          std::inserter(deviceListStrings, deviceListStrings.begin()),
          [](auto mac) {
            std::string macStr = mac.to_string();
            std::unordered_map<std::string, std::string> nicknameDict =
                ConfigManager::getInstanceConfigManager()
                    ->getNicknameDictionary();
            for (auto it = nicknameDict.begin(); it != nicknameDict.end();
                 it++) {
              if (it->second == macStr) {
                return it->first;
              }
            }
            return macStr;
          });
      jRule["deviceList"] = deviceListStrings;

      json response;
      response["rule"] = jRule;
      content = response.dump();
    } else {
      content = "Error: Invalid rule name";
    }
  }
  session->close(restbed::OK, content,
                 {{"Content-Length", std::to_string(content.size())}});
}

/*
 * Request handler for get requests sent to the status resource
 * Responds with the status of the firewall -Running or Stopped
 *
 */
void WebManager::statusHandler(
    const std::shared_ptr<restbed::Session> session) {
  bool status = ConfigManager::getInstanceConfigManager()
                    ->getEthernetManager()
                    ->isRunning();
  std::string content;
  if (status) {
    content = "Running";
  } else {
    content = "Stopped";
  }
  session->close(restbed::OK, content,
                 {{"Content-Length", std::to_string(content.size())}});
}

/*
 * Request handler for Get requests sent to the settings resource
 * Responds with json object of relevant settings data
 */
void WebManager::settingsGetHandler(
    const std::shared_ptr<restbed::Session> session) {
  json management;
  std::string content;
  management["storedEmail"] =
      ConfigManager::getInstanceConfigManager()->getEmailAddress();
  management["minEmailTime"] =
      ConfigManager::getInstanceConfigManager()->getMinEmailTime();
  management["wan"] = ConfigManager::getInstanceConfigManager()->getWAN();
  management["lan"] = ConfigManager::getInstanceConfigManager()->getLAN();
  management["host"] = ConfigManager::getInstanceConfigManager()->getHOST();
  management["vt_api_key"] =
      ConfigManager::getInstanceConfigManager()->getVT_API_KEY();
  management["httpEnabled"] =
      ConfigManager::getInstanceConfigManager()->getReplace();
  content = management.dump();
  session->close(restbed::OK, content,
                 {{"Content-Length", std::to_string(content.size())}});
}

/*
 *Request handler for patch requests sent to the settings resource
 * Updates the config manager via the updateSettings method
 * Responds with "success" or relevant error message
 */
void WebManager::settingsPatchHandler(
    const std::shared_ptr<restbed::Session> session) {
  auto request = session->get_request();
  std::string length = request->get_header("Content-length", "0");

  session->fetch(
      std::stoi(length), [](const std::shared_ptr<restbed::Session> session,
                            const restbed::Bytes &body) {
        EthManager *man =
            ConfigManager::getInstanceConfigManager()->getEthernetManager();
        bool firewallState = man->isRunning();

        // Stop the firewall to avoid race conditions.
        man->stopFirewall();

        std::string content;
        try {
          nlohmann::json settingsJson = nlohmann::json::parse(
              std::string((char *)body.data(), body.size()));

          if (!settingsJson.empty()) {
            ConfigManager::getInstanceConfigManager()->updateSettings(
                settingsJson);
            if (settingsJson.contains("action")) {
              if (settingsJson["action"] == "restart") {
                if (!system("shutdown -r now")) {
                  content = "Failed to reboot";
                }
              } else if (settingsJson["action"] != "disable") {
                man->startFirewallNewThread();
              }
            } else if (firewallState) {
              man->startFirewallNewThread();
            }
            content = "success";
          } else {
            content = "error: Missing setting object";
            if (firewallState) {
              man->startFirewallNewThread();
            }
          }
        } catch (nlohmann::json::parse_error &ex) {
          content = "error: invalid json";
          if (firewallState) {
            man->startFirewallNewThread();
          }
        } catch (...) {
          content = "An unknown error occured";
          if (firewallState) {
            man->startFirewallNewThread();
          }
        }

        session->close(restbed::OK, content,
                       {{"Content-Length", std::to_string(content.size())}});
      });
}
/*
 * Method handler to handle authentication
 * Runs prior to every request, ensures user on front end has proper
 * authentication credentials. If the auth is bad returns an unauthorized
 * message
 *
 */
void WebManager::authenticateRequest(
    const std::shared_ptr<restbed::Session> session,
    const std::function<void(const std::shared_ptr<restbed::Session>)>
        &callback) {
  auto request = session->get_request();
  std::string authString = request->get_header("Authorization");

  // update this to grab our auth string from our config
  // Default one below is admin:pass
  std::string storedAuth = ConfigManager::getInstanceConfigManager()->getAUTH();
  if (authString == ("Basic " + storedAuth)) {
    callback(session);
  } else {
    session->close(restbed::UNAUTHORIZED, {{"WWW-Authenticate", "Basic"}});
  }
}

/**
 * Request handler for post requests sent to the rule resource
 * Post requests to the rule resource are reserved for filterList updates
 * Post requests will either request to change to a different preset,
 * or add websites to an existing filter list.
 * Returns "success" or relevant error message
 */
void WebManager::filterListPostHandler(
    const std::shared_ptr<restbed::Session> session) {
  auto request = session->get_request();
  std::string length = request->get_header("Content-length", "0");
  session->fetch(
      std::stoi(length), [](const std::shared_ptr<restbed::Session> session,
                            const restbed::Bytes &body) {
        EthManager *man =
            ConfigManager::getInstanceConfigManager()->getEthernetManager();
        bool firewallState = man->isRunning();

        // Stop the firewall to avoid race conditions.
        man->stopFirewall();

        std::string content, filterListName, action, preset;
        try {
          action = nlohmann::json::parse(
              std::string((char *)body.data(), body.size()))["action"];
          filterListName = nlohmann::json::parse(
              std::string((char *)body.data(), body.size()))["fileName"];
          preset = nlohmann::json::parse(
              std::string((char *)body.data(), body.size()))["preset"];
          std::string LINE;

          if (preset != "") {
            std::ifstream HighFile;
            std::ifstream MedFile;
            std::ifstream LowFile;
            HighFile.open("./include/HighPreset");
            MedFile.open("./include/MediumPreset");
            LowFile.open("./include/LowPreset");

            if (!HighFile.fail() && !MedFile.fail() && !LowFile.fail()) {
              if (preset == "high") {
                removeEachFilterList(LowFile, LINE);
                removeEachFilterList(MedFile, LINE);
                addEachFilterList(HighFile, LINE);
              } else if (preset == "medium") {
                removeEachFilterList(LowFile, LINE);
                removeEachFilterList(HighFile, LINE);
                addEachFilterList(MedFile, LINE);
              } else if (preset == "low") {
                removeEachFilterList(HighFile, LINE);
                removeEachFilterList(MedFile, LINE);
                addEachFilterList(LowFile, LINE);
              } else if (preset == "none") {
                removeEachFilterList(HighFile, LINE);
                removeEachFilterList(MedFile, LINE);
                removeEachFilterList(LowFile, LINE);
              }
            }

            HighFile.close();
            MedFile.close();
            LowFile.close();
          }

          if (filterListName != "") {
            if (action == "add") {
              ConfigManager::getInstanceConfigManager()->addLIST(
                  "./include/PrecompiledFilters/" +
                  (std::string)filterListName);
            } else if (action == "del") {
              ConfigManager::getInstanceConfigManager()->delLIST(
                  "./include/PrecompiledFilters/" +
                  (std::string)filterListName);
            }
          }

          if (action == "reset") {
            ConfigManager::getInstanceConfigManager()->resetConfig();
          }

          content = "success";

        } catch (nlohmann::json::parse_error &ex) {
          content = "error: invalid json";
        } catch (nlohmann::json::type_error &e) {
          content = "error: missing required parameters";
        } catch (...) {
          content = "An unknown error occurred";
        }

        if (firewallState) {
          man->startFirewallNewThread();
        }

        session->close(restbed::OK, content,
                       {{"Content-Length", std::to_string(content.size())}});
      });
}

/**
 * Helper method for adding filters in a list
 */
void WebManager::addEachFilterList(std::ifstream &file, std::string &LINE) {
  while (getline(file, LINE)) {
    ConfigManager::getInstanceConfigManager()->addLIST(LINE);
  }
}

/*
 * Helper method for removing filters from our lists
 */
void WebManager::removeEachFilterList(std::ifstream &file, std::string &LINE) {
  while (getline(file, LINE)) {
    ConfigManager::getInstanceConfigManager()->delLIST(LINE);
  }
}

/*
 * Request handler for patch requests sent to the nicknames resource
 * Updates the config manager's nickname dictionary with relevant actions
 * Returns "success" or the appropriate error message.
 */
void WebManager::nicknamePatchHandler(
    const std::shared_ptr<restbed::Session> session) {
  auto request = session->get_request();
  std::string length = request->get_header("Content-length", "0");
  session->fetch(
      std::stoi(length), [](const std::shared_ptr<restbed::Session> session,
                            const restbed::Bytes &body) {
        // Unlike other requests, nicknames are really only used by the web
        // manager. Since these requests are not multithreaded, we don't need to
        // stop the firewall.
        std::string content = "success";

        try {
          nlohmann::json nicknameJson = nlohmann::json::parse(
              std::string((char *)body.data(), body.size()));

          if (!nicknameJson.empty()) {
            if (nicknameJson.contains("action")) {
              if (nicknameJson["action"] == "add") {
                bool wasAdded =
                    ConfigManager::getInstanceConfigManager()->addNickname(
                        nicknameJson);
                if (!wasAdded) {
                  content = "Error: Nicknames must be unique";
                }

              } else if (nicknameJson["action"] == "del") {
                ConfigManager::getInstanceConfigManager()->delNickname(
                    nicknameJson);
              }
            }
          } else {
            content = "error: Missing action";
          }
        } catch (nlohmann::json::parse_error &ex) {
          content = "error: invalid json";
        } catch (Tins::invalid_address &) {
          content = "error: invalid address";
        } catch (nlohmann::json::type_error &e) {
          content = "error: missing required parameters";
        } catch (...) {
          content = "An unknown error occured";
        }

        session->close(restbed::OK, content,
                       {{"Content-Length", std::to_string(content.size())}});
      });
}

/*
 * Request handler for get requests sent to the nickname resource
 * Responds with a json object containing a list of 3 tuples
 * Within the three tuples is information such as mac_addr, nickname, data_usage
 * Data usage is in megabytes
 */
void WebManager::nicknameGetHandler(
    const std::shared_ptr<restbed::Session> session) {
  json response;
  std::string content;

  auto configManager = ConfigManager::getInstanceConfigManager();
  std::unordered_map<std::string, std::string> nicknameDict =
      configManager->getNicknameDictionary();

  typedef std::tuple<std::string, std::string, unsigned long long>
      userInfoTuple;
  std::list<userInfoTuple> userInfo;

  auto macAddrs = configManager->getDataStats();

  for (auto pair = macAddrs.begin(); pair != macAddrs.end(); ++pair) {
    std::string nick = pair->first.to_string();

    for (auto it = nicknameDict.begin(); it != nicknameDict.end(); ++it)
      if (it->second == pair->first.to_string()) nick = it->first;

    userInfo.push_back(
        std::make_tuple(nick, pair->first.to_string(),
                        pair->second / 1024000));  // Send to backend in MB
  }

  json jList(userInfo);
  response["nickname"] = jList;
  content = response.dump();
  session->close(restbed::OK, content,
                 {{"Content-Length", std::to_string(content.size())}});
}

/*
 *Request handler for get requests sent to the statistics resource
 *Responds with statistics obtained from the statisticsmanager
 *Statistics include totalPackets and weekly violations.
 */
void WebManager::statsGetHandler(
    const std::shared_ptr<restbed::Session> session) {
  session->get_request();
  json response;
  std::string content;

  auto configManager = ConfigManager::getInstanceConfigManager();

  unsigned long long total_packets = configManager->get_total_packet_stats();
  json jLong(total_packets);

  std::array<int, 7> weeklyViolation = configManager->getViolationStats();
  json jArr(weeklyViolation);

  response["totalPackets"] = jLong;
  response["weeklyViolations"] = jArr;

  content = response.dump();
  session->close(restbed::OK, content,
                 {{"Content-Length", std::to_string(content.size())}});
}

/*
 * ConfigManager.cpp
 * Author: 
 */

#include <ConfigManager.h>
#include <Constants.h>
#include <string.h>
#include <tins/ethernetII.h>
#include <tins/hw_address.h>

#include <algorithm>
#include <array>
#include <iomanip>
#include <iterator>
#include <unordered_set>
#include <utility>

ConfigManager *ConfigManager::instance;

/*
 * Constructor for the config manager
 */
ConfigManager::ConfigManager() {
  json j;
  std::ifstream i("config.json");
  i >> j;
  ConfigManager::ethMan = NULL;
  ConfigManager::LAN = (std::string)j["LAN"];
  ConfigManager::WAN = (std::string)j["WAN"];
  ConfigManager::HOST = (std::string)j["HOST"];
  ConfigManager::AUTH = (std::string)j["AUTH"];
  ConfigManager::VT_API_KEY = (std::string)j["VT_API_KEY"];
  ConfigManager::EMAIL = (std::string)j["EMAIL"];
  ConfigManager::EMAIL_SECS = (double)j["EMAIL_SECS"];
  this->setViolationStats(j["VIOLATION_WEEK"].get<std::array<int, 7>>());
  ConfigManager::last_vio_day = (int)j["VIOLATION_DAY"];
  ConfigManager::total_packets = (unsigned long long)j["TOTAL_PACKETS"];
  ConfigManager::replace = (bool)j["httpEnabled"];
  ConfigManager::nicknameDictionary =
      j["NICKNAME_DICT"].get<std::unordered_map<std::string, std::string>>();
  json ruleList = j["ruleList"];
  for (json rule : ruleList) {
    ConfigManager::updateRule(rule);
  }
}

/*
 * Destructor for the config manager
 */
ConfigManager::~ConfigManager() {}

/*
 * Getter for the config manager instance
 */
ConfigManager *ConfigManager::getInstanceConfigManager() { return instance; }

/*
 * Creates the singleton for the config manager
 */
void ConfigManager::createSingletonConfigManager() {
  if (!instance) instance = new ConfigManager();
}

/*
 * Destroys the singleton for the config manager
 */
void ConfigManager::freeSingletonConfigManager() {
  if (instance) delete instance;
}

/*
 * An iterator that points to the start of the enabled rules collection.
 */
std::vector<RuleObject *>::iterator ConfigManager::enabledRulesBegin() {
  return enabledRules.begin();
}

/*
 * An iterator that points to the end of the enabled rules collection.
 */
std::vector<RuleObject *>::iterator ConfigManager::enabledRulesEnd() {
  return enabledRules.end();
}

/*
 * A helper function which maps a rule's name to its rule object
 */
RuleObject *ConfigManager::getRuleObjectByName(std::string ruleName) {
  auto rule = availableRules.find(ruleName);
  if (rule == availableRules.end())  // If the rule name is invalid, return null
    return NULL;
  else
    return rule->second;
}

/*
 * Update the rule and it's corresponding spot in the config.json file
 */
void ConfigManager::updateRule(nlohmann::json &rule) {
  RuleObject *obj = getRuleObjectByName(rule["name"]);
  if (obj == NULL) return;

  if (rule.contains("block")) {
    obj->setBlockEnabled((bool)rule["block"]);
  }
  if (rule.contains("filterBlockList")) {
    obj->setFilterBlockListEnabled((bool)rule["filterBlockList"]);
  }
  if (rule.contains("deviceBlockList")) {
    obj->setDeviceBlockListEnabled((bool)rule["deviceBlockList"]);
  }
  if (rule.contains("report")) {
    obj->setReportEnabled((bool)rule["report"]);
  }
  if (rule.contains("name")) {
    obj->setName((std::string)rule["name"]);
  }
  if (rule.contains("enabled")) {
    obj->setEnabled((bool)rule["enabled"]);
    if ((bool)rule["enabled"]) {  // Rule should be enabled
      if (std::find(enabledRules.begin(), enabledRules.end(), obj) ==
          enabledRules.end()) {
        // Rule not found, so we add it
        enabledRules.push_back(obj);
      }
    } else {  // Rule should be disabled
      if (std::find(enabledRules.begin(), enabledRules.end(), obj) !=
          enabledRules.end()) {
        // Rule found, so we remove it
        enabledRules.erase(
            std::remove(enabledRules.begin(), enabledRules.end(), obj),
            enabledRules.end());
      }
    }
  }
  // Insert rules
  if (rule.contains("action")) {
    if (rule["action"] == "add") {
      if (rule.contains("filterList")) {
        for (std::string x : rule["filterList"]) obj->addFilter(x);
      }
      if (rule.contains("deviceList")) {
        for (std::string y : rule["deviceList"]) {
          if (nicknameDictionary.count(y)) {
            obj->addDevice(nicknameDictionary[y]);
          } else {
            Tins::EthernetII::address_type mac(y);
            obj->addDevice(y);
          }
        }
      }
    }
    // Remove rules
    else if (rule["action"] == "del") {
      if (rule.contains("filterList")) {
        for (std::string x : rule["filterList"]) obj->eraseFilter(x);
      }
      if (rule.contains("deviceList")) {
        for (std::string y : rule["deviceList"]) {
          if (nicknameDictionary.count(y)) {
            obj->eraseDevice(nicknameDictionary[y]);
          } else {
            obj->eraseDevice(y);
          }
        }
      }
    }
  }
  ConfigManager::writeConfigFile();
  return;
}

/*
 * Takes in a settings json and updated the config manager accordingly
 */
void ConfigManager::updateSettings(nlohmann::json &settings) {
  if (settings.contains("wan")) {
    setWAN(settings["wan"]);
  }
  if (settings.contains("lan")) {
    setLAN(settings["lan"]);
  }
  if (settings.contains("host")) {
    setHOST(settings["host"]);
  }
  if (settings.contains("storedEmail")) {
    setEmailAddress(settings["storedEmail"]);
  }
  if (settings.contains("minEmailTime")) {
    setMinEmailTime(settings["minEmailTime"]);
  }
  if (settings.contains("auth")) {
    setAUTH(settings["auth"]);
  }
  if (settings.contains("vt_api_key")) {
    setVT_API_KEY(settings["vt_api_key"]);
  }
  if (settings.contains("httpEnabled")) {
    setReplace(settings["httpEnabled"]);
  }
  ConfigManager::writeConfigFile();
  return;
}

/*
 * Takes in a nickname json and adds a new nickname for a mac address
 */
bool ConfigManager::addNickname(const nlohmann::json &nickname) {
  std::string name = nickname["name"];
  std::string value = nickname["value"];

  // Used for ensuring our value is a mac address
  Tins::EthernetII::address_type mac(value);

  // Check if we need to rename a device
  for (auto it = nicknameDictionary.begin(); it != nicknameDictionary.end();
       ++it) {
    if (it->second == value) {
      nicknameDictionary.erase(it);
      break;
    }
  }

  // Add the device but make sure the nickname is unique.
  bool ret = false;
  if (!nicknameDictionary.count(name)) {
    nicknameDictionary.emplace(name, mac.to_string());
    ret = true;
  }

  ConfigManager::writeConfigFile();
  return ret;
}

/*
 * Takes in a nickname json and removes a nickname for a mac address
 */
void ConfigManager::delNickname(const nlohmann::json &nickname) {
  std::string name = nickname["name"];
  if (nicknameDictionary.count(name)) {
    nicknameDictionary.erase(name);
  }
  ConfigManager::writeConfigFile();
}

/*
 * Returns a copy of the nickname dictionary
 */
std::unordered_map<std::string, std::string>
ConfigManager::getNicknameDictionary() {
  return nicknameDictionary;
}

/*
 * Getter for the WAN device
 */
std::string ConfigManager::getWAN() { return ConfigManager::WAN; }

/*
 * Setter for WAN
 */
void ConfigManager::setWAN(const std::string &wan_) {
  ConfigManager::WAN = wan_;
}

/*
 * Setter for LAN
 */
void ConfigManager::setLAN(const std::string &lan_) {
  ConfigManager::LAN = lan_;
}

/*
 * Setter for HOST
 */
void ConfigManager::setHOST(const std::string &host_) {
  ConfigManager::HOST = host_;
}

/*
 * Getter for the LAN device
 */
std::string ConfigManager::getLAN() { return ConfigManager::LAN; }

/*
 * Getter for the HOST IP
 */
std::string ConfigManager::getHOST() { return ConfigManager::HOST; }

/*
 * Getter for the auth token
 */
std::string ConfigManager::getAUTH() { return ConfigManager::AUTH; }

/*
 * Getter for the VirusTotal API key
 */
std::string ConfigManager::getVT_API_KEY() { return ConfigManager::VT_API_KEY; }

/*
 * Setter for the auth token
 */
void ConfigManager::setAUTH(const std::string &auth_) {
  ConfigManager::AUTH = auth_;
}

/*
 * Setter for the VirusTotal API key
 */
void ConfigManager::setVT_API_KEY(const std::string &key_) {
  ConfigManager::VT_API_KEY = key_;
}

/*
 * Getter for the email address to send reports to
 */
std::string ConfigManager::getEmailAddress() { return ConfigManager::EMAIL; }

/*
 * Setter for the email address to send reports to
 */
void ConfigManager::setEmailAddress(const std::string &address) {
  ConfigManager::EMAIL = address;
}

/*
 * Getter for the minimum time between sending another email in seconds
 */
double ConfigManager::getMinEmailTime() { return EMAIL_SECS; }

/*
 * Setter for the minimum time between sending another email in seconds
 */
void ConfigManager::setMinEmailTime(double t) { ConfigManager::EMAIL_SECS = t; }

/*
 * Reads in a 1 item per line file that can contain DNS xor IPs
 */
void ConfigManager::addLIST(const std::string &fileNAME) {
  std::string LINE, parameters, ruleName;
  std::ifstream MyReadFile;
  std::string::size_type pos;

  MyReadFile.open(fileNAME);
  if (!MyReadFile.fail()) {
    while (getline(MyReadFile, LINE)) {
      pos = LINE.find(' ', 0);
      parameters = LINE.substr(pos + 1);

      // Trim trailing whitespace off of the parameters
      size_t p = parameters.find_last_not_of(" \t");
      if (std::string::npos != p) parameters.erase(p + 1);

      ruleName = LINE.substr(0, pos);

      RuleObject *obj = getRuleObjectByName(ruleName);
      if (std::find(enabledRules.begin(), enabledRules.end(), obj) ==
          enabledRules.end()) {
        // Rule not found, so we add it
        enabledRules.push_back(obj);
      }
      obj->setEnabled(true);
      obj->addFilter(parameters);
    }
    MyReadFile.close();
    ConfigManager::writeConfigFile();
  }
}

void ConfigManager::delLIST(const std::string &fileNAME) {
  std::string LINE, parameters, ruleName;
  std::ifstream MyReadFile;
  std::string::size_type pos;

  MyReadFile.open(fileNAME);
  if (!MyReadFile.fail()) {
    while (getline(MyReadFile, LINE)) {
      pos = LINE.find(' ', 0);
      parameters = LINE.substr(pos + 1);

      // Trim trailing whitespace off of the parameters
      size_t p = parameters.find_last_not_of(" \t");
      if (std::string::npos != p) parameters.erase(p + 1);

      ruleName = LINE.substr(0, pos);

      RuleObject *obj = getRuleObjectByName(ruleName);
      obj->eraseFilter(parameters);
    }
    MyReadFile.close();
    ConfigManager::writeConfigFile();
  }
}

/*
 * Setter for the ethernet manager
 */
void ConfigManager::setEthernetManager(EthManager *eth) {
  ConfigManager::ethMan = eth;
}

/*
 * Getter for the ethernet manager
 */
EthManager *ConfigManager::getEthernetManager() {
  return ConfigManager::ethMan;
}

/*
 * This funtion writes the current config to our config file
 */
void ConfigManager::writeConfigFile() {
  // Only one instance of this function should be ran at a time
  static std::mutex write_mutex;
  std::unique_lock<std::mutex> lock(write_mutex);

  json j;
  std::ofstream o("config.json");
  j["HOST"] = getHOST();
  j["LAN"] = getLAN();
  j["WAN"] = getWAN();
  j["AUTH"] = getAUTH();
  j["EMAIL"] = getEmailAddress();
  j["EMAIL_SECS"] = getMinEmailTime();
  j["NICKNAME_DICT"] = getNicknameDictionary();
  j["VT_API_KEY"] = getVT_API_KEY();
  j["VIOLATION_WEEK"] = getViolationStats();
  j["VIOLATION_DAY"] = last_vio_day.load();
  j["TOTAL_PACKETS"] = total_packets.load();
  j["httpEnabled"] = replace;
  std::unordered_set<json> *j_list = new std::unordered_set<json>();
  for (auto rule : availableRules) {
    // No point in storing Website Filters in the config file - it just controls
    // other filters
    if (rule.first == "website") continue;

    RuleObject *ruleobj = rule.second;
    json jj = *ruleobj;
    j_list->insert(jj);
  }
  j["ruleList"] = *j_list;
  o << std::setw(4) << j << std::endl;
  o.close();
}

/*
 * This function allows nlohmann to serialize a RuleObject into json
 */
void to_json(json &j, const RuleObject &rule) {
  const bool filterBlockList = rule.isFilterBlockListEnabled();
  const bool deviceBlockList = rule.isDeviceBlockListEnabled();
  const bool block = rule.isBlockEnabled();
  const bool enabled = rule.isEnabled();
  RuleObject *nonConstRuleObj = const_cast<RuleObject *>(&rule);
  const std::unordered_set<std::string> filterList =
      nonConstRuleObj->getFilterListDisplay();
  const std::string name = rule.getName();
  const bool report = rule.isReportEnabled();

  // Need to get deviceList as strings
  std::vector<std::string> deviceListStrings;
  const std::unordered_set<Tins::EthernetII::address_type, MacHash> deviceList =
      nonConstRuleObj->getDeviceList();
  std::transform(deviceList.begin(), deviceList.end(),
                 std::inserter(deviceListStrings, deviceListStrings.begin()),
                 [](auto mac) { return mac.to_string(); });

  j = json{{"block", block},  // @suppress("Symbol is not resolved")
           {"action", "add"},
           {"filterBlockList", filterBlockList},
           {"deviceBlockList", deviceBlockList},
           {"deviceList", deviceListStrings},
           {"enabled", enabled},
           {"filterList", filterList},
           {"name", name},
           {"report", report}};
}

/*
 * Serialize json to ruleObject
 */
void from_json(const json &j, RuleObject &rule) {
  bool block, filterBlockList, deviceBlockList, enabled, report;
  std::unordered_set<std::string> deviceList, filterList;
  std::string name;

  j.at("block").get_to(block);
  j.at("filterBlockList").get_to(filterBlockList);
  j.at("deviceBlockList").get_to(deviceBlockList);
  j.at("deviceList").get_to(deviceList);
  j.at("enabled").get_to(enabled);
  j.at("filterList").get_to(filterList);
  j.at("name").get_to(name);
  j.at("report").get_to(report);

  rule.setBlockEnabled(block);
  rule.setFilterBlockListEnabled(filterBlockList);
  rule.setDeviceBlockListEnabled(deviceBlockList);

  // Set deviceList here
  // Set filterList here

  rule.setEnabled(enabled);
  rule.setName(name);
  rule.setReportEnabled(report);
}

/*
 * Reset the config.json to default, removing all the rules.
 */
void ConfigManager::resetConfig() {
  AUTH = Constants::default_auth;
  EMAIL = Constants::default_email;
  EMAIL_SECS = Constants::default_email_secs;
  VT_API_KEY = Constants::default_vt_api_key;
  nicknameDictionary.clear();
  for (auto pair : availableRules) {
    RuleObject *rule = pair.second;

    rule->setBlockEnabled(Constants::default_block);
    rule->setReportEnabled(Constants::default_report);
    rule->setFilterBlockListEnabled(Constants::default_FilterBlockList);
    if (pair.first == "mac") {
      rule->setDeviceBlockListEnabled(Constants::default_MAC_DeviceBlockList);
    } else {
      rule->setDeviceBlockListEnabled(Constants::default_DeviceBlockList);
    }
    if (pair.first == "data") {
      // Our nickname functionality only works properly if data limits are
      // enabled.
      rule->setEnabled(true);
    } else {
      rule->setEnabled(Constants::default_enabled);
    }

    rule->setName(pair.first);

    rule->clearFilters();
    rule->clearDevices();
  }
  enabledRules.clear();  // Disable all the rules
  writeConfigFile();
  return;
}

/*
 * Getter for the device usage stats
 */
const std::unordered_map<Tins::EthernetII::address_type, unsigned long long,
                         MacHash>
ConfigManager::getDataStats() {
  return static_cast<DataLimits *>(getRuleObjectByName("data"))->getDataUsage();
}

/*
 * Getter for the violation week stats
 */
std::array<int, 7> ConfigManager::getViolationStats() {
  // Convert our atomic to a temp copy
  std::array<int, 7> ret;
  std::transform(violation_week.begin(), violation_week.end(), ret.begin(),
                 [](std::atomic_int &i) -> int { return i.load(); });
  return ret;
}

/*
 * Setter for the violation week stats. Should only be called for initialization
 * from config as it will override existing items making it not thread safe.
 */
void ConfigManager::setViolationStats(const std::array<int, 7> &input) {
  for (int i = 0; i < (int)input.size(); i++) {
    this->violation_week[i].store(input[i]);
  }
}

/*
 * Getter for the last day we set violation stats
 */
int ConfigManager::get_last_day() { return last_vio_day.load(); }

/*
 * Setter for the last day we set violations stats
 */
void ConfigManager::set_last_day(int last) { last_vio_day.store(last); }

/*
 * Increments the total packet counterssss
 */
void ConfigManager::add_total_packet() { total_packets++; }

/*
 * Getter for the webmanager to receive the total packet stat
 */
unsigned long long ConfigManager::get_total_packet_stats() {
  return total_packets.load();
}

void ConfigManager::add_violation(int day) { violation_week[day]++; }

void ConfigManager::reset_violations(int day) {
  violation_week[day].store(0);
  if (day == 0) total_packets.store(0);  // Clear our packets on Sundays
}

void ConfigManager::setReplace(bool r_) { replace = r_; }

bool ConfigManager::getReplace() { return replace; }

/*
 * ConfigManager.h
 *
 *  Author:
 */

#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <fstream>
#include <iostream>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

#include "EthManager.h"
#include "Rules/RuleObject.h"
#include "Rules/Rules.h"
#include "nlohmann/json.hpp"

using nlohmann::json;

class ConfigManager {
 private:
  ConfigManager();
  ~ConfigManager();
  static ConfigManager *instance;
  std::unordered_map<std::string, RuleObject *> availableRules =
      Rules::createImplementedRules();
  std::vector<RuleObject *> enabledRules;
  std::string WAN;
  std::string LAN;
  std::string HOST;
  std::string AUTH;
  std::string EMAIL;
  std::string VT_API_KEY;
  double EMAIL_SECS;
  EthManager *ethMan;
  std::unordered_map<std::string, std::string> nicknameDictionary;
  std::atomic_int last_vio_day;
  std::atomic_ullong total_packets;
  void setViolationStats(const std::array<int, 7> &input);
  std::array<std::atomic_int, 7> violation_week;
  bool replace = Constants::default_replace;

 public:
  static ConfigManager *getInstanceConfigManager();
  static void createSingletonConfigManager();
  static void freeSingletonConfigManager();

  RuleObject *getRuleObjectByName(std::string);

  std::vector<RuleObject *>::iterator enabledRulesBegin();
  std::vector<RuleObject *>::iterator enabledRulesEnd();

  void updateRule(nlohmann::json &j);
  void updateSettings(nlohmann::json &j);
  void resetConfig();
  std::string getWAN();
  std::string getLAN();
  std::string getHOST();
  std::string getAUTH();
  std::string getVT_API_KEY();
  void setWAN(const std::string &);
  void setVT_API_KEY(const std::string &);
  void setLAN(const std::string &);
  void setHOST(const std::string &);
  void setAUTH(const std::string &);
  void setMinEmailTime(double time);
  void setEmailAddress(const std::string &);
  std::string getEmailAddress();
  double getMinEmailTime();
  void addLIST(const std::string &);
  void delLIST(const std::string &);
  bool addNickname(const nlohmann::json &nickname);
  void delNickname(const nlohmann::json &nickname);
  std::unordered_map<std::string, std::string> getNicknameDictionary();
  void setEthernetManager(EthManager *ethMan);
  EthManager *getEthernetManager();
  void writeConfigFile();
  const std::unordered_map<Tins::EthernetII::address_type, unsigned long long,
                           MacHash>
  getDataStats();
  std::array<int, 7> getViolationStats();
  int get_last_day();
  void set_last_day(int);
  void add_total_packet();
  void add_violation(int day);
  void reset_violations(int day);
  unsigned long long get_total_packet_stats();
  bool getReplace();
  void setReplace(bool);
};

void to_json(json &j, const RuleObject &rule);
void from_json(const json &j, RuleObject &rule);

#endif

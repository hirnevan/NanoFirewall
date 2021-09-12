/*
 * Rules.h
 *
 * This header is to be modified when a new rule is created.
 * It stores the declarations for all rule objects
 *
 *  Created on: Nov 13, 2020
 *      Author: 
 */

#ifndef RULES_RULES_H_
#define RULES_RULES_H_

#include <Constants.h>
#include <Rules/RuleObject.h>
#include <bits/types/time_t.h>
#include <tins/ethernetII.h>
#include <tins/ip_address.h>

#include <atomic>
#include <bitset>
#include <cstdint>
#include <map>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>

class IpFilter : public RuleObject {
  bool packetViolatesRule(const Tins::PDU &pdu) override;
};

class DnsFilter : public RuleObject {
  bool packetViolatesRule(const Tins::PDU &pdu) override;
};

class TlsFilter : public RuleObject {
  bool packetViolatesRule(const Tins::PDU &pdu) override;
};

class SmartFilter : public RuleObject {
  bool packetViolatesRule(const Tins::PDU &pdu) override;

 public:
  static bool isPrivateIPv4(Tins::IPv4Address &ip);
};

class AdvDnsFilter : public IpFilter {
 public:
  AdvDnsFilter();
  ~AdvDnsFilter() override;
  void addFilter(std::string &) override;
  void eraseFilter(std::string &) override;
  void clearFilters() override;
  const std::unordered_set<std::string> &getFilterListDisplay() override;

 private:
  std::unordered_map<std::string, std::unordered_set<std::string>> dns_to_ip;
  std::unordered_set<std::string> filterListDomains;
  static std::string get_ip_str(const struct sockaddr *sa);
};

class WebsiteFilter : public RuleObject {
 public:
  void addFilter(std::string &) override;
  void eraseFilter(std::string &) override;
  void clearFilters() override;
  void addDevice(std::string &) override;
  void eraseDevice(std::string &) override;
  void clearDevices() override;
  bool packetViolatesRule(const Tins::PDU &pdu) override;
  const std::unordered_set<std::string> &getFilterListDisplay() override;
  bool isBlockEnabled() const override;
  void setBlockEnabled(bool) override;
  bool isReportEnabled() const override;
  void setReportEnabled(bool) override;
  bool isFilterBlockListEnabled() const override;
  void setFilterBlockListEnabled(bool) override;
  bool isDeviceBlockListEnabled() const override;
  void setDeviceBlockListEnabled(bool) override;
};

class MacFilter : public RuleObject {
 public:
  MacFilter() {
    // Since the device list is essentially the MAC filter's filter list,
    // We want the default value to be a block list.
    // Otherwise it will block everything and the user will probably be confused
    this->setDeviceBlockListEnabled(Constants::default_MAC_DeviceBlockList);
  }

 private:
  bool packetViolatesRule(const Tins::PDU &pdu) override;
};

class PortFilter : public RuleObject {
  bool packetViolatesRule(const Tins::PDU &pdu) override;
};

class TimeFilter : public RuleObject {
 private:
  struct HourMinute {
    uint_fast8_t hour;
    uint_fast8_t minute;
    bool operator<(HourMinute const &b) const;
  };
  struct TimeRange {
    HourMinute start;
    HourMinute end;
    bool isInRange(const HourMinute &time) const;
  };
  std::multimap<std::string, TimeRange> timeRanges;

 public:
  bool packetViolatesRule(const Tins::PDU &pdu) override;
  void addFilter(std::string &) override;
  void eraseFilter(std::string &) override;
  void clearFilters() override;
};

class ProtocolFilter : public RuleObject {
 private:
  enum availableProtocols { http, https, icmp, ftp, ipv4, ipv6, numProtocols };
  std::bitset<numProtocols> protocols;

 public:
  bool packetViolatesRule(const Tins::PDU &pdu) override;
  void addFilter(std::string &) override;
  void eraseFilter(std::string &) override;
  void clearFilters() override;
};

class DataLimits : public RuleObject {
 public:
  DataLimits();
  ~DataLimits() override;
  bool packetViolatesRule(const Tins::PDU &pdu) override;
  void addFilter(std::string &) override;
  void eraseFilter(std::string &) override;
  void clearFilters() override;
  const std::unordered_map<Tins::EthernetII::address_type, unsigned long long,
                           MacHash>
  getDataUsage();

 private:
  std::unordered_map<Tins::EthernetII::address_type, std::atomic_ullong,
                     MacHash>
      dataLimits;
  std::unordered_map<Tins::EthernetII::address_type, std::atomic_ullong,
                     MacHash>
      dataUsage;
  std::atomic<uint8_t> prev_day;
};

class GeoLocationFilter : public RuleObject {
 public:
  bool packetViolatesRule(const Tins::PDU &pdu) override;
};

// Defines our implemented rules.
class Rules {
 public:
  /*
   * Returns a map which provides the rule objects we have implemented and their
   * IDs
   */
  static std::unordered_map<std::string, RuleObject *>
  createImplementedRules() {
    std::unordered_map<std::string, RuleObject *> ruleMap;

    ruleMap.emplace(std::make_pair("ip", new IpFilter()));
    ruleMap.emplace(std::make_pair("dns", new DnsFilter()));
    ruleMap.emplace(std::make_pair("advdns", new AdvDnsFilter()));
    ruleMap.emplace(std::make_pair("mac", new MacFilter()));
    ruleMap.emplace(std::make_pair("port", new PortFilter()));
    ruleMap.emplace(std::make_pair("data", new DataLimits()));
    ruleMap.emplace(std::make_pair("time", new TimeFilter()));
    ruleMap.emplace(std::make_pair("protocol", new ProtocolFilter()));
    ruleMap.emplace(std::make_pair("smart", new SmartFilter()));
    ruleMap.emplace(std::make_pair("geolocation", new GeoLocationFilter()));
    ruleMap.emplace(std::make_pair("tls", new TlsFilter()));
    ruleMap.emplace(std::make_pair("website", new WebsiteFilter()));

    return ruleMap;
  }
};

#endif /* RULES_RULES_H_ */

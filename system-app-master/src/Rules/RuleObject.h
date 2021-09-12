/*
 * Rule.h
 *
 *  Created on: Nov 5, 2020
 *      Author: 
 */

#ifndef RULES_RULEOBJECT_H_
#define RULES_RULEOBJECT_H_

#include <Constants.h>
#include <stddef.h>
#include <tins/ethernetII.h>

#include <string>
#include <unordered_map>
#include <unordered_set>

// Custom Hash Functor that will compute the hash on the
// passed MAC address
struct MacHash {
 public:
  size_t operator()(const Tins::EthernetII::address_type& mac) const {
    size_t hash = 7;

    hash = hash * hash + 8 * mac[0];
    hash = hash - 8 * mac[1];
    hash = hash * hash + 8 * mac[2];
    hash = hash - 8 * mac[3];
    hash = hash * hash + 8 * mac[4];
    hash = hash - 8 * mac[5];

    return hash;
  }
};

/*
 * Rule is an abstract class declaration to ensure that all
 * rules contain the proper functions for our Ethernet manager
 * to call
 */
class RuleObject {
 public:
  RuleObject();
  virtual ~RuleObject();

  /*
   * This function will be called whenever an interface that this rule is
   * subscribed to receives a packet. It should return true if the packet
   * violates a rule, and false if it is an acceptable packet.
   */
  virtual bool packetViolatesRule(const Tins::PDU& pdu) = 0;

  virtual const std::unordered_set<std::string>& getFilterListDisplay();
  const std::unordered_set<Tins::EthernetII::address_type, MacHash>&
  getDeviceList();
  virtual void addFilter(std::string&);
  virtual void eraseFilter(std::string&);
  virtual void clearFilters();
  virtual void addDevice(std::string&);
  virtual void eraseDevice(std::string&);
  virtual void clearDevices();

  bool shouldFilterDevice(const Tins::PDU& pdu);

  // Getters and setters
  virtual bool isBlockEnabled() const;
  virtual void setBlockEnabled(bool);
  virtual bool isReportEnabled() const;
  virtual void setReportEnabled(bool);
  virtual bool isFilterBlockListEnabled() const;
  virtual void setFilterBlockListEnabled(bool);
  virtual bool isDeviceBlockListEnabled() const;
  virtual void setDeviceBlockListEnabled(bool);
  bool isEnabled() const;
  void setEnabled(bool);
  std::string getName() const;
  void setName(std::string name_);

 protected:
  std::unordered_set<std::string> filterList;
  std::unordered_set<Tins::EthernetII::address_type, MacHash> deviceList;
  const std::unordered_set<std::string>& getFilterListBackground();

 private:
  bool block = Constants::default_block;
  bool report = Constants::default_report;
  bool isFilterBlockList = Constants::default_FilterBlockList;
  bool isDeviceBlockList = Constants::default_DeviceBlockList;
  bool enabled = Constants::default_enabled;
  std::string name;
};

#endif /* RULES_RULEOBJECT_H_ */

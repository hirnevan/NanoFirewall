/*
 * Rule.cpp
 *
 *  Created on: Nov 13, 2020
 *      Author: 
 */

#include <Rules/RuleObject.h>
#include <tins/ethernetII.h>
#include <tins/pdu.h>

#include <string>
#include <unordered_map>
#include <utility>

/*
 * Creates a rule object
 */
RuleObject::RuleObject() {}

/*
 * Destroys a rule object
 */
RuleObject::~RuleObject() {}

/*
 * Adds a new filter to the filter list
 */
void RuleObject::addFilter(std::string &filter) { filterList.insert(filter); }

/*
 * Removes a filter from the filter list
 */
void RuleObject::eraseFilter(std::string &filter) { filterList.erase(filter); }

/*
 * Clears all filters
 */
void RuleObject::clearFilters() { filterList.clear(); }

/*
 * Adds a device to the device list
 */
void RuleObject::addDevice(std::string &filter) { deviceList.insert(filter); }

/*
 * Removes a device to the device list
 */
void RuleObject::eraseDevice(std::string &filter) { deviceList.erase(filter); }

/*
 * Clears all devices
 */
void RuleObject::clearDevices() { deviceList.clear(); }

/*
 * Returns a reference to this rule object's filter list (background)
 * This is what a rule object handles behind the scenes
 */
const std::unordered_set<std::string> &RuleObject::getFilterListBackground() {
  return filterList;
}

/*
 * Returns a reference to this rule object's filter list (foreground)
 * This is what is stored in the config and on the web manager
 */
const std::unordered_set<std::string> &RuleObject::getFilterListDisplay() {
  return filterList;
}

/*
 * Returns a reference to this rule object's device list.
 */
const std::unordered_set<Tins::EthernetII::address_type, MacHash>
    &RuleObject::getDeviceList() {
  return deviceList;
}

/*
 * Returns whether a given PDU needs to be filtered based on our device list
 */
bool RuleObject::shouldFilterDevice(const Tins::PDU &pdu) {
  if (auto eth = pdu.find_pdu<Tins::EthernetII>()) {
    if (deviceList.find(eth->src_addr()) != deviceList.end()) {
      // If block list is enabled and it is on the list, then we want to inspect
      // this packet further.
      return isDeviceBlockListEnabled();
    } else {
      // If allow list is enabled and it is not on the list, then we want to
      // inspect it further
      return !isDeviceBlockListEnabled();
    }
  } else {
    // This packet isn't Ethernet so we will just allow it through (probably a
    // network protocol)
    return false;
  }
}

/*
 * Getter for block packets that violate rule
 */
bool RuleObject::isBlockEnabled() const { return block; }

/*
 * Getter for report packets that violate rule
 */
bool RuleObject::isReportEnabled() const { return report; }

/*
 * Getter for block list (true) vs allow list (false) for filter list
 */
bool RuleObject::isFilterBlockListEnabled() const { return isFilterBlockList; }

/*
 * Setter for block list (true) vs allow list (false)
 */
void RuleObject::setBlockEnabled(bool toBlock) { block = toBlock; }

/*
 *Getter for block list (true) vs allow list (false) for device list
 */
bool RuleObject::isDeviceBlockListEnabled() const { return isDeviceBlockList; }

/*
 * Getter for block list (true) vs allow list (false)
 */
bool RuleObject::isEnabled() const { return enabled; }

/*
 * Setter for whether rule is enabled
 */
void RuleObject::setEnabled(bool toEnabled) { enabled = toEnabled; }

/*
 * Setter for report packets that violate rule
 */
void RuleObject::setReportEnabled(bool toReport) { report = toReport; }

/*
 * Setter for block packets that violate rule
 */
void RuleObject::setFilterBlockListEnabled(bool toBlockList) {
  isFilterBlockList = toBlockList;
}

/*
 * Setter to block devices
 */
void RuleObject::setDeviceBlockListEnabled(bool toBlockList) {
  isDeviceBlockList = toBlockList;
}

/*
 * Getter for the name of this rule
 */
std::string RuleObject::getName() const { return name; }

/*
 * Setter for the name of this rule
 */
void RuleObject::setName(std::string name_) { name = name_; }

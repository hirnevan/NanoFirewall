/*
 * DataLimits.cpp
 *
 *  Created on: Feb 19, 2021
 *      Author: 
 */

#include <Constants.h>
#include <Rules/RuleObject.h>
#include <Rules/Rules.h>
#include <tins/ethernetII.h>
#include <tins/pdu.h>
#include <tins/rawpdu.h>

#include <algorithm>
#include <ctime>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>

/*
 * Constructs a new data limits rule object
 */
DataLimits::DataLimits() : RuleObject() { prev_day.store(0); }

/*
 * Destructor for a data limit rule object
 */
DataLimits::~DataLimits() {}

/*
 * Adds a new data limit
 * expected to be of the form <mac adddr><space><data limit in bytes>
 * macaddr should be a valid MAC address
 * the data limit should fit within an unsigned long long
 * The data limit is applied for a 24 hour interval
 */
void DataLimits::addFilter(std::string &filter) {
  std::string macAddr = filter.substr(0, 17);
  unsigned long long dataLimit = std::stoull(filter.substr(18));

  // Remove any duplicates (old limits for the same addr)
  this->eraseFilter(macAddr);

  // Insert the new
  filterList.insert(filter);
  dataLimits.insert(std::make_pair(macAddr, dataLimit));
}

/*
 * Removes an existing data limit filter
 * Only the MAC addr needs to be provided.
 */
void DataLimits::eraseFilter(std::string &macAddr) {
  if (dataLimits.count(macAddr)) {
    std::string oldFilter = macAddr + ' ' + std::to_string(dataLimits[macAddr]);
    filterList.erase(oldFilter);
  }

  dataLimits.erase(macAddr);
}

/*
 * Clears existing filters
 */
void DataLimits::clearFilters() {
  filterList.clear();
  dataLimits.clear();
}

/*
 * Returns a map between MAC addresses and their data usage
 * Resets once per day
 * Returns a reference that should not be modified (const)
 */
const std::unordered_map<Tins::EthernetII::address_type, unsigned long long,
                         MacHash>
DataLimits::getDataUsage() {
  // Convert our atomic to a temp copy
  std::unordered_map<Tins::EthernetII::address_type, unsigned long long,
                     MacHash>
      ret;
  std::transform(dataUsage.begin(), dataUsage.end(),
                 std::inserter(ret, ret.end()), [](auto &pair) {
                   return std::make_pair(pair.first, pair.second.load());
                 });
  return ret;
}

/*
 * Determines if the given PDU violates a data limit rule
 */
bool DataLimits::packetViolatesRule(const Tins::PDU &pdu) {
  if (auto eth = pdu.find_pdu<Tins::EthernetII>()) {
    auto usage = dataUsage.find(eth->dst_addr());

    // If no usage here, initialize to zero
    if (usage == dataUsage.end()) {
      dataUsage.emplace(std::make_pair(eth->dst_addr(), 0));
      usage = dataUsage.find(eth->dst_addr());
    }

    // First increment the usage
    const Tins::RawPDU *payload = pdu.find_pdu<Tins::RawPDU>();
    if (payload) usage->second += payload->size();

    // Then check if it violates their allotment
    auto limit = dataLimits.find(eth->dst_addr());
    if (limit != dataLimits.end()) {
      // Verify usage data is still valid. Reset once per day.
      time_t raw_time = time(NULL);
      tm *local_time = localtime(&raw_time);
      uint8_t day = local_time->tm_wday;
      if (day != prev_day.load()) {
        dataUsage.clear();
        prev_day.store(day);
      }

      // Check the limit
      if (usage->second.load() > limit->second.load()) return true;
    }
  }

  return false;
}

/*
 * IpFilter.cpp
 *
 *  Created on: Nov 13, 2020
 *      Author: 
 */

#include <Rules/Rules.h>
#include <tins/ip.h>
#include <tins/ipv6.h>
#include <tins/pdu.h>

#include <unordered_map>
#include <unordered_set>

/*
 * Allow or block IP addresses based on this objects filter.
 * This checks IPv4, IPv6, src and dst IP addresses.
 */
bool IpFilter::packetViolatesRule(const Tins::PDU &pdu) {
  if (const Tins::IP *ip = pdu.find_pdu<Tins::IP>()) {  // Check IPv4
    if (getFilterListBackground().count(ip->src_addr().to_string()) ||
        getFilterListBackground().count(ip->dst_addr().to_string())) {
      // If block list is enabled and it is on the list, then it is a violation.
      return isFilterBlockListEnabled();
    } else {
      // If allow list is enabled and it is not on the list, then it is a
      // violation.
      return !isFilterBlockListEnabled();
    }
  } else if (const Tins::IPv6 *ip = pdu.find_pdu<Tins::IPv6>()) {  // Check IPv6
    if (getFilterListBackground().count(ip->src_addr().to_string()) ||
        getFilterListBackground().count(ip->dst_addr().to_string())) {
      // If block list is enabled and it is on the list, then it is a violation.
      return isFilterBlockListEnabled();
    } else {
      // If allow list is enabled and it is not on the list, then it is a
      // violation.
      return !isFilterBlockListEnabled();
    }
  } else {
    // Wasn't IP, not our job to filter it.
    return false;
  }
}

/*
 * MacFilter.cpp
 *
 *  Created on: Jan 23, 2021
 *      Author: 
 */

#include <ExternalInterfaces/VirusTotalManager.h>
#include <Rules/Rules.h>
#include <tins/address_range.h>
#include <tins/ip.h>
#include <tins/ip_address.h>
#include <tins/pdu.h>

#include <string>

/*
 * Determines if the IP provided is a private IP address
 * These follow the standard Class C, Class B, Class A, and multicast
 * conventions
 */
bool SmartFilter::isPrivateIPv4(Tins::IPv4Address &ip) {
  static Tins::IPv4Range classC = Tins::IPv4Address("192.168.0.0") / 16;
  static Tins::IPv4Range classB = Tins::IPv4Address("172.16.0.0") / 12;
  static Tins::IPv4Range classA = Tins::IPv4Address("10.0.0.0") / 8;
  static Tins::IPv4Range multicast = Tins::IPv4Address("224.0.0.0") / 24;

  return (classC.contains(ip) || classB.contains(ip) || classA.contains(ip) ||
          multicast.contains(ip));
}

/*
 * Uses the VirusTotal API to determine if a packet violates a rule
 * Note that this may or may not perform a request based on usage restrictions
 */
bool SmartFilter::packetViolatesRule(const Tins::PDU &pdu) {
  // Extract only external IP addresses (no use to scan an internal IP)
  // Makes the critical assumption that only one IP (src or dst) can be
  // external.
  if (const Tins::IP *ip =
          pdu.find_pdu<Tins::IP>()) {  // VirusTotal API only supports IPv4
    Tins::IPv4Address addr;
    std::string search;

    if (!isPrivateIPv4(addr = ip->src_addr())) {
      search = addr.to_string();
    } else if (!isPrivateIPv4(addr = ip->dst_addr())) {
      search = addr.to_string();
    } else {
      // Both private IP addresses. No need to search
      return false;
    }

    // Try to determine notoriety. Note this may not result in a request based
    // on usage limits.
    return VirusTotalManager::isDetectedIp(search);
  }

  return false;
}

/*
 * PortFilter.cpp
 *
 *  Created on: Jan 23, 2021
 *      Author: 
 */

#include <Rules/Rules.h>
#include <tins/pdu.h>
#include <tins/tcp.h>
#include <tins/udp.h>

#include <string>
#include <unordered_map>
#include <unordered_set>

/*
 * Allow or block port based on this objects filter.
 * Realistically, the source port doesn't matter. So we only filter destination
 * ports
 */
bool PortFilter::packetViolatesRule(const Tins::PDU &pdu) {
  const Tins::UDP *udp = pdu.find_pdu<Tins::UDP>();
  const Tins::TCP *tcp = pdu.find_pdu<Tins::TCP>();

  if (udp) {
    if (getFilterListBackground().count(std::to_string(udp->dport()))) {
      // If block list is enabled and it is on the list, then it is a violation.
      return isFilterBlockListEnabled();
    } else {
      // If allow list is enabled and it is not on the list, then it is a
      // violation.
      return !isFilterBlockListEnabled();
    }
  } else if (tcp) {
    if (getFilterListBackground().count(std::to_string(tcp->dport()))) {
      // If block list is enabled and it is on the list, then it is a violation.
      return isFilterBlockListEnabled();
    } else {
      // If allow list is enabled and it is not on the list, then it is a
      // violation.
      return !isFilterBlockListEnabled();
    }
  } else {
    // Not a TCP/UDP packet - not applicable to this filter.
    return false;
  }
}

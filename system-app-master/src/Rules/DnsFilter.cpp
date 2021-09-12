/*
 * DnsFilter.cpp
 *
 *  Created on: Nov 13, 2020
 *      Author: 
 */

#include <Rules/Rules.h>
#include <tins/dns.h>
#include <tins/pdu.h>
#include <tins/rawpdu.h>
#include <tins/udp.h>

#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <vector>

/*
 * Allow or block DNS requests based on this objects filter.
 */
bool DnsFilter::packetViolatesRule(const Tins::PDU &pdu) {
  // DNS lives on UDP
  const Tins::UDP *udp = pdu.find_pdu<Tins::UDP>();
  if (!udp)  // If it is not a UDP packet, we don't want to deal with it.
    return false;

  // UDP is on port 53. If its not on 53 then don't deal with it
  if (udp->dport() != 53) return false;

  Tins::DNS dns = pdu.rfind_pdu<Tins::RawPDU>().to<Tins::DNS>();
  auto filters = getFilterListBackground();
  auto queries = dns.queries();

  // If any of the queries are on the filter list and it is a block list
  // then it is a violation
  return isFilterBlockListEnabled() ==
         std::any_of(queries.begin(), queries.end(), [filters](auto &query) {
           return filters.count(query.dname());
         });
}

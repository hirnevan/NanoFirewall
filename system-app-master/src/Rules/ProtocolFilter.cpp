/*
 * ProtocolFilter.cpp
 *
 *  Created on: Mar 12, 2021
 *      Author: 
 */

#include <Rules/Rules.h>
#include <tins/icmp.h>
#include <tins/ip.h>
#include <tins/ipv6.h>
#include <tins/pdu.h>
#include <tins/tcp.h>
#include <tins/udp.h>

#include <bitset>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>

/*
 * Determines if this PDU vilates a protocol filtering rule
 */
bool ProtocolFilter::packetViolatesRule(const Tins::PDU &pdu) {
  bool found = false;
  const Tins::TCP *tcp;
  const Tins::UDP *udp;

  if (this->protocols[http]) {
    tcp = pdu.find_pdu<Tins::TCP>();
    if (tcp) found = (tcp->dport() == 80 || tcp->dport() == 8080);
  }

  if (this->protocols[https]) {
    tcp = pdu.find_pdu<Tins::TCP>();
    if (tcp) found = (tcp->dport() == 443);
  }

  if (this->protocols[icmp]) {
    const Tins::ICMP *icmp = pdu.find_pdu<Tins::ICMP>();
    if (icmp) found = true;
  }

  if (this->protocols[ftp]) {
    // Regular FTP is on TCP 20/21
    tcp = pdu.find_pdu<Tins::TCP>();
    if (tcp) found = (tcp->dport() == 443);

    // TFTP is on UDP 69
    udp = pdu.find_pdu<Tins::UDP>();
    if (udp) found = (udp->dport() == 69);
  }

  if (this->protocols[ipv4]) {
    const Tins::IP *ip = pdu.find_pdu<Tins::IP>();
    if (ip) found = true;
  }

  if (this->protocols[ipv6]) {
    const Tins::IPv6 *ip = pdu.find_pdu<Tins::IPv6>();
    if (ip) found = true;
  }

  return (found == this->isFilterBlockListEnabled());
}

/*
 * Adds a supported protocol to the filter list.
 * Options:
 * 	http
 * 	https
 * 	icmp
 * 	ftp
 * 	ipv4
 * 	ipv6
 */
void ProtocolFilter::addFilter(std::string &filter) {
  filterList.insert(filter);

  if (filter == "http") {
    this->protocols.set(http);
  } else if (filter == "https") {
    this->protocols.set(https);
  } else if (filter == "icmp") {
    this->protocols.set(icmp);
  } else if (filter == "ftp") {
    this->protocols.set(ftp);
  } else if (filter == "ipv4") {
    this->protocols.set(ipv4);
  } else if (filter == "ipv6") {
    this->protocols.set(ipv6);
  } else {
    // If its not suported then take it off our list.
    filterList.erase(filter);
  }
}

/*
 * Removes a supported protocol to the filter list.
 * Options:
 * 	http
 * 	https
 * 	icmp
 * 	ftp
 * 	ipv4
 * 	ipv6
 */
void ProtocolFilter::eraseFilter(std::string &filter) {
  filterList.erase(filter);

  if (filter == "http") {
    this->protocols.reset(http);
  } else if (filter == "https") {
    this->protocols.reset(https);
  } else if (filter == "icmp") {
    this->protocols.reset(icmp);
  } else if (filter == "ftp") {
    this->protocols.reset(ftp);
  } else if (filter == "ipv4") {
    this->protocols.reset(ipv4);
  } else if (filter == "ipv6") {
    this->protocols.reset(ipv6);
  }
}

/*
 * Resets the protocol filters
 */
void ProtocolFilter::clearFilters() {
  filterList.clear();
  this->protocols.reset();
}

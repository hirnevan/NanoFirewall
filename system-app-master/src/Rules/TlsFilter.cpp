/*
 * TlsFilter.cpp
 *
 *  Created on: Mar 28, 2021
 *      Author: 
 */

#include <PDUs/tls.h>
#include <Rules/Rules.h>
#include <tins/pdu.h>
#include <tins/rawpdu.h>
#include <tins/tcp.h>

#include <algorithm>
#include <string>
#include <unordered_map>
#include <unordered_set>

/*
 * Determine if the TLS handshake server name matches one of our filters
 * Note that this filter uses a "contained in" format rather than "equal to"
 * Therefore a filter "google" will handle "google.com" and "server.google.com"
 */
bool TlsFilter::packetViolatesRule(const Tins::PDU &pdu) {
  // TLS lives on TCP
  const Tins::TCP *tcp = pdu.find_pdu<Tins::TCP>();
  if (!tcp)  // If it is not a TCP packet, we don't want to deal with it.
    return false;

  // TLS is on port 443. If its not on 443 then don't deal with it
  if (tcp->dport() != 443) return false;

  // Verify the contents are not empty.
  if (tcp->inner_pdu() == NULL) return false;

  TLS tls = ((Tins::RawPDU *)tcp->inner_pdu())->to<TLS>();

  // Retrieve the server name identifier, if possible
  std::string sni = tls.get_server_name();

  if (sni.length() > 0) {
    // SNIs usually contain multiple servers for the same hostname.
    // So we check if any of our filters are contained in the SNI
    // ie. s.google.com is filtered by "google.com"

    return isFilterBlockListEnabled() ==
           std::any_of(filterList.begin(), filterList.end(),
                       [sni](auto &filter) {
                         return sni.find(filter) != std::string::npos;
                       });
  }

  // If it got to this point, we can allow it.
  return false;
}

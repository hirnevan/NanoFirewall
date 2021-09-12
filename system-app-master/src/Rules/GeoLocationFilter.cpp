/**
 *GeolocationFilter.cpp
 *Created by: 
 */

#include <ExternalInterfaces/GeolocationManager.h>
#include <Rules/Rules.h>
#include <tins/ip.h>
#include <tins/ip_address.h>
#include <tins/pdu.h>

#include <string>
#include <unordered_map>
#include <unordered_set>

/*
 * Determines if a PDU comes from a geolocation in the filter list
 * Filters should consist of the country's ISO code (ie. US, RU, etc.)
 */
bool GeoLocationFilter::packetViolatesRule(const Tins::PDU &pdu) {
  if (const Tins::IP *ip = pdu.find_pdu<Tins::IP>()) {  // Check IPv4
    Tins::IPv4Address addr;

    // Only lookup external IP addresses
    if (!SmartFilter::isPrivateIPv4(addr = ip->src_addr())) {
      std::string lookup = addr.to_string();

      try {
        std::string country = GeolocationManager::GetCountryIsoCode(lookup);
        if (filterList.count(country)) {
          // On list and block list is a violation
          return this->isFilterBlockListEnabled();
        } else {
          // Not on list and allow list is violation
          return !this->isFilterBlockListEnabled();
        }
      } catch (...) {  // Do nothing on failed lookup
      }
    }
  }

  return false;
}

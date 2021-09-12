/*
 * GeolocationManager.cpp
 *
 *  Created on: Mar 27, 2021
 *      Author: 
 */

#include <ExternalInterfaces/GeolocationManager.h>

// Define our singleton variables. Essentially static class constructor
namespace GeolocationManager {
GeoLite2PP::DB db("./include/GeoLiteDB/GeoLite2-Country.mmdb");
}

using namespace GeolocationManager;

/*
 * Attempts to return the ISO 3166 Country Code given an IPv4 address.
 * Throws if the IP is not found in the database
 * Expects only publicly routable IP addresses
 */
std::string GeolocationManager::GetCountryIsoCode(std::string &ip) {
  return db.get_field(ip, "", GeoLite2PP::VCStr{"country", "iso_code"});
}

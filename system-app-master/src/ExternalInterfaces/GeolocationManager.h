/*
 * GeolocationManager.h
 *
 *  Created on: Mar 27, 2021
 *      Author: 
 */

#ifndef EXTERNALINTERFACES_GEOLOCATIONMANAGER_H_
#define EXTERNALINTERFACES_GEOLOCATIONMANAGER_H_

#include <GeoLite2PP/GeoLite2PP.hpp>
#include <string>

// Use a namespace to mimic a static class
namespace GeolocationManager {
std::string GetCountryIsoCode(std::string &ip);
extern GeoLite2PP::DB db;
}  // namespace GeolocationManager

#endif /* EXTERNALINTERFACES_GEOLOCATIONMANAGER_H_ */

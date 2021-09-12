/*
 * WebsiteFilter.cpp
 *
 *  Created on: Apr 3, 2021
 *      Author: 
 */

#include <ConfigManager.h>
#include <Rules/Rules.h>

#include <regex>
#include <string>
#include <unordered_map>
#include <unordered_set>

/*
 * Adds a new website filter
 * This adds the filter to both dns and advdns
 * This also adds a stripped version to the tls filter
 *  - Note that the stripped version could be overharsh
 *  - For example, ads.google.com becomes all of google
 *  - Also, google.co.uk becomes "co"
 */
void WebsiteFilter::addFilter(std::string &s) {
  ConfigManager *cm = ConfigManager::getInstanceConfigManager();
  cm->getRuleObjectByName("dns")->addFilter(s);
  cm->getRuleObjectByName("advdns")->addFilter(s);

  // TLS we want to strip the name before adding
  std::smatch match;
  if (std::regex_search(s, match, std::regex("(.*\\.)*(.*)\\."))) {
    std::string servName = match.str(2);
    cm->getRuleObjectByName("tls")->addFilter(servName);
  }
}

/*
 * Erases the filter from dns, advdns, and tls filters
 */
void WebsiteFilter::eraseFilter(std::string &s) {
  ConfigManager *cm = ConfigManager::getInstanceConfigManager();
  cm->getRuleObjectByName("dns")->eraseFilter(s);
  cm->getRuleObjectByName("advdns")->eraseFilter(s);

  // TLS we want to strip the name before removing
  std::smatch match;
  if (std::regex_search(s, match, std::regex("(.*\\.)*(.*)\\."))) {
    std::string servName = match.str(2);
    cm->getRuleObjectByName("tls")->eraseFilter(servName);
  }
}

/*
 * Removes all filters from dns, advdns, and tls filters
 */
void WebsiteFilter::clearFilters() {
  ConfigManager *cm = ConfigManager::getInstanceConfigManager();
  cm->getRuleObjectByName("dns")->clearFilters();
  cm->getRuleObjectByName("advdns")->clearFilters();
  cm->getRuleObjectByName("tls")->clearFilters();
}

/*
 * Adds device to dns, advdns, and tls filters
 */
void WebsiteFilter::addDevice(std::string &s) {
  ConfigManager *cm = ConfigManager::getInstanceConfigManager();
  cm->getRuleObjectByName("dns")->addDevice(s);
  cm->getRuleObjectByName("advdns")->addDevice(s);
  cm->getRuleObjectByName("tls")->addDevice(s);
}

/*
 * Removes device from dns, advdns, and tls filters
 */
void WebsiteFilter::eraseDevice(std::string &s) {
  ConfigManager *cm = ConfigManager::getInstanceConfigManager();
  cm->getRuleObjectByName("dns")->eraseDevice(s);
  cm->getRuleObjectByName("advdns")->eraseDevice(s);
  cm->getRuleObjectByName("tls")->eraseDevice(s);
}

/*
 * Clears devices from dns, advdns, and tls filters
 */
void WebsiteFilter::clearDevices() {
  ConfigManager *cm = ConfigManager::getInstanceConfigManager();
  cm->getRuleObjectByName("dns")->clearDevices();
  cm->getRuleObjectByName("advdns")->clearDevices();
  cm->getRuleObjectByName("tls")->clearDevices();
}

/*
 * WebsiteFilter is just a proxy for adding other filter types.
 * Therefore we always want it disabled. This is called on every packet
 */
bool WebsiteFilter::packetViolatesRule(const Tins::PDU &pdu) {
  // If we ever got to this point, disable it so we don't get here in the
  // future. Since its a bool, we don't worry about the race condition much. It
  // will eventually go false.
  this->setEnabled(false);
  return false;
}

/*
 * Returns website filters
 */
const std::unordered_set<std::string> &WebsiteFilter::getFilterListDisplay() {
  // Return the filter list for DNS since they should all match.
  return ConfigManager::getInstanceConfigManager()
      ->getRuleObjectByName("dns")
      ->getFilterListDisplay();
}

/*
 * Getter for website filters enabled
 */
bool WebsiteFilter::isBlockEnabled() const {
  return ConfigManager::getInstanceConfigManager()
      ->getRuleObjectByName("dns")
      ->isBlockEnabled();
}

/*
 * Setter for dns, advdns, and tls block
 */
void WebsiteFilter::setBlockEnabled(bool b) {
  ConfigManager *cm = ConfigManager::getInstanceConfigManager();
  cm->getRuleObjectByName("dns")->setBlockEnabled(b);
  cm->getRuleObjectByName("advdns")->setBlockEnabled(b);
  cm->getRuleObjectByName("tls")->setBlockEnabled(b);
}

/*
 * Getter for website filter report enabled
 */
bool WebsiteFilter::isReportEnabled() const {
  return ConfigManager::getInstanceConfigManager()
      ->getRuleObjectByName("dns")
      ->isReportEnabled();
}

/*
 * Setter for dns, advdns, and tls report
 */
void WebsiteFilter::setReportEnabled(bool b) {
  ConfigManager *cm = ConfigManager::getInstanceConfigManager();
  cm->getRuleObjectByName("dns")->setReportEnabled(b);
  cm->getRuleObjectByName("advdns")->setReportEnabled(b);
  cm->getRuleObjectByName("tls")->setReportEnabled(b);
}

/*
 * Getter for website filter block list enabled
 */
bool WebsiteFilter::isFilterBlockListEnabled() const {
  return ConfigManager::getInstanceConfigManager()
      ->getRuleObjectByName("dns")
      ->isFilterBlockListEnabled();
}

/*
 * Setter for dns, advdns, and tls filter block list
 */
void WebsiteFilter::setFilterBlockListEnabled(bool b) {
  ConfigManager *cm = ConfigManager::getInstanceConfigManager();
  cm->getRuleObjectByName("dns")->setFilterBlockListEnabled(b);
  cm->getRuleObjectByName("advdns")->setFilterBlockListEnabled(b);
  cm->getRuleObjectByName("tls")->setFilterBlockListEnabled(b);
}

/*
 * Getter for website device block list enabled
 */
bool WebsiteFilter::isDeviceBlockListEnabled() const {
  return ConfigManager::getInstanceConfigManager()
      ->getRuleObjectByName("dns")
      ->isDeviceBlockListEnabled();
}

/*
 * Setter for dns, advdns, and tls device block list
 */
void WebsiteFilter::setDeviceBlockListEnabled(bool b) {
  ConfigManager *cm = ConfigManager::getInstanceConfigManager();
  cm->getRuleObjectByName("dns")->setDeviceBlockListEnabled(b);
  cm->getRuleObjectByName("advdns")->setDeviceBlockListEnabled(b);
  cm->getRuleObjectByName("tls")->setDeviceBlockListEnabled(b);
}

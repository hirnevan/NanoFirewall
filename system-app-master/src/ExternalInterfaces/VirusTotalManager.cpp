/*
 * VirusTotalManager.cpp
 *
 *  Created on: Mar 27, 2021
 *      Author: 
 */

#include <ConfigManager.h>
#include <ExternalInterfaces/VirusTotalManager.h>
#include <Rules/RuleObject.h>
#include <bits/types/time_t.h>

#include <algorithm>
#include <cstddef>
#include <cstdio>
#include <iostream>
#include <memory>
#include <nlohmann/json.hpp>
#include <restbed>
#include <vector>

// Define our singleton variables. Essentially static class constructor
namespace VirusTotalManager {
time_t prev_time = 0;
int requests_since_prev = 0;
std::mutex vt_mutex;
std::array<std::string, recents_size> recents;
int recent_index = 0;
}  // namespace VirusTotalManager

using namespace VirusTotalManager;
using namespace restbed;

/*
 * Interfaces with the VirusTotal API. Returns true if any positives are known
 * from this IP address. Returns false if no positives are found, or false if
 * the usage limit is reached. This function manages its own locks, so it is
 * threadsafe to be called from anywhere.
 */
bool VirusTotalManager::isDetectedIp(std::string &ip) {
  bool shouldRequest = false;

  // Begin critical section
  vt_mutex.lock();

  if (difftime(time(NULL), prev_time) > duration_seconds) {
    // Reset our counters
    requests_since_prev = 0;
    prev_time = time(NULL);
  }

  if (requests_since_prev < requests_per_duration) {
    if (std::find(recents.begin(), recents.end(), ip) == recents.end()) {
      requests_since_prev++;
      shouldRequest = true;

      // Keep track of recents so that we don't do a bunch of requests for the
      // same IP.
      recents[recent_index] = ip;
      recent_index = (recent_index + 1) % recents_size;
    }
  }

  // end critical section
  vt_mutex.unlock();

  // The requests themselves can be concurrent.
  // We already verified whether it is safe to perform a request in the above
  // critical section.
  if (shouldRequest) {
    std::cout << "Performing a VirusTotal query for: " << ip << std::endl;

    auto request = std::make_shared<Request>(
        Uri("https://www.virustotal.com/vtapi/v2/ip-address/report"));
    request->set_query_parameter("ip", ip);
    request->set_query_parameter(
        "apikey", ConfigManager::getInstanceConfigManager()->getVT_API_KEY());
    request->set_header("Accept", "*/*");
    request->set_header("Host", "www.virustotal.com");

    auto response = Http::sync(request);
    std::string lengthStr = response->get_header("Content-Length", "0");

    if (lengthStr == "0" || lengthStr == "") {
      // Quit early if the request wasn't valid.
      return false;
    }

    std::size_t len;
    sscanf(lengthStr.c_str(), "%zu", &len);
    Http::fetch(len, response);

    try {
      nlohmann::json json = nlohmann::json::parse(std::string(
          (char *)response->get_body().data(), response->get_body().size()));

      if (json.contains("detected_downloaded_samples")) {
        if (json["detected_downloaded_samples"].size() > 0) {
          std::cout << "An IP address with malicious notoriety was found on "
                       "VirusTotal: "
                    << ip << std::endl;

          // This IP address has detected samples... This is bad!
          // Since we don't want any of these bad IPs on the device, add an IP
          // filter first. Note this does NOT write to the config file
          // immediately, but will upon a future edit.
          ConfigManager::getInstanceConfigManager()
              ->getRuleObjectByName("ip")
              ->addFilter(ip);
          return true;
        }
      }
    } catch (nlohmann::detail::parse_error &e) {
    }
  }

  return false;
}

/*
 * VirusTotalManager.h
 *
 *  Created on: Mar 27, 2021
 *      Author: tyler
 */

#ifndef EXTERNALINTERFACES_VIRUSTOTALMANAGER_H_
#define EXTERNALINTERFACES_VIRUSTOTALMANAGER_H_

#include <bits/types/time_t.h>

#include <array>
#include <mutex>
#include <string>

// Use a namespace to mimic a static class
namespace VirusTotalManager {
bool isDetectedIp(std::string &ip);
// Allow 4 requests every 6 minutes. This keeps us under our 1000/day and
// 30000/month
constexpr int requests_per_duration = 4;
constexpr int duration_seconds = 360;
constexpr int recents_size = 32;  // Store the 32 most recent queries in a list.

extern time_t prev_time;
extern int requests_since_prev;
extern std::mutex vt_mutex;
extern std::array<std::string, recents_size> recents;
extern int recent_index;
}  // namespace VirusTotalManager

#endif /* EXTERNALINTERFACES_VIRUSTOTALMANAGER_H_ */

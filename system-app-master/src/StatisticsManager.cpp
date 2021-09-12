/*
 * StatisticsManager.cpp
 *
 *  Created on: Apr 1, 2021
 *      Author: 
 */

#include <ConfigManager.h>
#include <StatisticsManager.h>
#include <bits/types/struct_tm.h>
#include <bits/types/time_t.h>

#include <ctime>

StatisticsManager::StatisticsManager() {}

StatisticsManager::~StatisticsManager() {}

/*
 * Increments one violation stat for today, but first checks whether the day
 * needs to be incremented
 * ConfigManager uses atomics violation counter so this function is thread-safe
 */
void StatisticsManager::add_violation() {
  time_t raw_time = time(NULL);
  tm *local_time = localtime(&raw_time);
  int day = local_time->tm_wday;
  StatisticsManager::update_day(day);
  ConfigManager::getInstanceConfigManager()->add_violation(day);
}

/*
 * Updates the config manager with the last day we set violation stats for
 * This function locks and is thread safe
 */
void StatisticsManager::update_day(int day) {
  ConfigManager *cm = ConfigManager::getInstanceConfigManager();

  // Only one instance of this function should be ran at a time
  static std::mutex day_mutex;
  std::unique_lock<std::mutex> lock(day_mutex);

  if (day != cm->get_last_day()) {
    cm->set_last_day(day);
    cm->reset_violations(day);
    cm->writeConfigFile();
  }
}

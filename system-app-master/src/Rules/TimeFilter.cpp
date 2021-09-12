/*
 * TimeFilter.cpp
 *
 *  Created on: Feb 25, 2021
 *      Author: 
 */

#include <Rules/Rules.h>
#include <bits/types/struct_tm.h>
#include <bits/types/time_t.h>
#include <inttypes.h>

#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <ctime>
#include <map>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>

/*
 * Determines if a PDU occurred in a filtered time
 */
bool TimeFilter::packetViolatesRule(const Tins::PDU &pdu) {
  time_t raw_time = time(NULL);
  tm *local_time = localtime(&raw_time);
  const HourMinute now = {(uint_fast8_t)local_time->tm_hour,
                          (uint_fast8_t)local_time->tm_min};

  return isFilterBlockListEnabled() ==
         std::any_of(timeRanges.begin(), timeRanges.end(), [now](auto &range) {
           return range.second.isInRange(now);
         });
}

/*
 * Adds a new time filter
 * Expected in <start><space><end>
 * Start and end in HH:MM format in 24-hour time.
 */
void TimeFilter::addFilter(std::string &filter) {
  filterList.insert(filter);

  // Parse our filter into a time range. It should be in the format 'start end'
  // Where start and end are times in hh:mm format. hh should be 24 hour format.
  uint_fast8_t startH, startM, endH, endM;
  sscanf(filter.c_str(),
         "%" SCNuFAST8 ":%" SCNuFAST8 " %" SCNuFAST8 ":%" SCNuFAST8, &startH,
         &startM, &endH, &endM);

  // Since the time ranges is a multimap, we first want to delete any
  // duplicates.
  timeRanges.erase(filter);

  HourMinute start = {startH, startM};
  HourMinute end = {endH, endM};

  // If the end time is before the start time, we assume it is overnight.
  // We can achieve this by doing two filters based on midnight.
  if (end < start) {
    HourMinute endOfDay = {23, 59};
    HourMinute startOfDay = {00, 00};

    TimeRange range1 = {start, endOfDay};
    TimeRange range2 = {startOfDay, end};
    timeRanges.emplace(filter, range1);
    timeRanges.emplace(filter, range2);
  } else {
    TimeRange range = {start, end};
    timeRanges.emplace(filter, range);
  }
}

/*
 * Removes a time filter
 */
void TimeFilter::eraseFilter(std::string &filter) {
  filterList.erase(filter);
  timeRanges.erase(filter);
}

/*
 * Clears all time filters
 */
void TimeFilter::clearFilters() {
  filterList.clear();
  timeRanges.clear();
}

/*
 * Determine is a given HourMinute structure is less than another
 */
bool TimeFilter::HourMinute::operator<(HourMinute const &b) const {
  if (this->hour > b.hour) {
    return false;
  } else if (this->hour == b.hour) {
    if (this->minute > b.minute) {
      return false;
    }
  }

  return true;
}

/*
 * Determines if a given time is within a TimeRange
 */
bool TimeFilter::TimeRange::isInRange(const HourMinute &time) const {
  return (start < time) && (time < end);
}

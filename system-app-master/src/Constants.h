/*
 * Constants.h
 *
 *  Created on: Feb 8, 2021
 *      Author: 
 */

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

namespace Constants {
constexpr bool default_block = true;
constexpr bool default_report = false;
constexpr bool default_replace = false;
constexpr bool default_FilterBlockList = true;
constexpr bool default_DeviceBlockList = false;
constexpr bool default_MAC_DeviceBlockList = true;
constexpr bool default_enabled = false;
constexpr auto default_auth = "YWRtaW46cGFzcw==";
constexpr auto default_vt_api_key = "";
constexpr auto default_email = "";           // No default email address
constexpr double default_email_secs = 3600;  // Only send an email once per hour
constexpr auto report_email_address =
    "bmFub2ZpcmV3YWxscmVwb3J0c0BnbWFpbC5jb20=";
constexpr auto report_email_secret = "c2FsdFRoaXNJc0FCYWRQYXNzd29yZHNhbHQ=";
constexpr auto report_email_smtp = "c210cHM6Ly9zbXRwLmdtYWlsLmNvbQ==";
constexpr double seconds_in_day = 86400;
}  // namespace Constants

#endif /* CONSTANTS_H_ */

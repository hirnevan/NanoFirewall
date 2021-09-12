/*
 * EmailManager.cpp
 *
 *  Created on: Feb 13, 2021
 *      Author: 
 */

#include <ConfigManager.h>
#include <Constants.h>
#include <EmailManager.h>
#include <curl/curl.h>
#include <stddef.h>
#include <tins/ethernetII.h>
#include <tins/hw_address.h>
#include <tins/pdu.h>

#include <WebSocket++/base64.hpp>
#include <cstring>
#include <mutex>
#include <unordered_map>
#include <utility>

/*
 * Constructor for the email manager
 */
EmailManager::EmailManager() : prev_time(0), violations_since_last(0) {
  violations.clear();
}

/*
 * Destructor for the email manager
 */
EmailManager::~EmailManager() {}

/*
 * Notifies the email manager of a violation
 * Adds it to the list of violations to eventually send an email about
 * This function is thread-safe to call
 * Calls sendEmail after configured amount of time
 */
void EmailManager::addViolation(const Tins::PDU &pdu,
                                const std::string &ruleName) {
  // Only one instance of this function should be ran at a time
  static std::mutex email_mutex;
  std::unique_lock<std::mutex> lock(email_mutex);

  if (auto eth = pdu.find_pdu<Tins::EthernetII>()) {
    // We use a set so you don't get spammed with duplicates.
    auto nicknameDict =
        ConfigManager::getInstanceConfigManager()->getNicknameDictionary();
    std::string violator = eth->src_addr().to_string();
    for (auto it = nicknameDict.begin(); it != nicknameDict.end(); ++it) {
      if (it->second == violator) {
        violator = it->first;
        break;
      }
    }
    violations.insert(ruleName + " VIOLATION(S) FROM " + violator + "\n");
    violations_since_last++;

    if (difftime(time(NULL), prev_time) >
        ConfigManager::getInstanceConfigManager()->getMinEmailTime()) {
      sendEmail();
    }
  }
}

/*
 * A struct to be used by libcurl for uploading data
 */
struct upload_data {
  int lines_read;
  const char *payload_text[7];
};

/*
 * Sends an email using libcurl based on the violations that have been incurred
 */
void EmailManager::sendEmail() {
  // Build our content
  std::string content = "There have been " +
                        std::to_string(violations_since_last) +
                        " violations since last report:\n";

  for (auto it = violations.begin(); it != violations.end(); it++)
    content.append(*it);

  // Reset our counters
  violations.clear();
  violations_since_last = 0;
  prev_time = time(NULL);

  // Send the email
  ConfigManager *cm = ConfigManager::getInstanceConfigManager();
  if (cm->getEmailAddress() != "") {
    CURL *curl;

    struct curl_slist *recipients = NULL;
    struct upload_data upload;
    upload.lines_read = 0;
    std::string to =
        std::string("To: ") + cm->getEmailAddress() + std::string("\r\n");
    upload.payload_text[0] = to.c_str();
    std::string from =
        std::string("From: ") +
        websocketpp::base64_decode(Constants::report_email_address) +
        std::string(" (NanoFirewall Device Report)\r\n");
    upload.payload_text[1] = from.c_str();
    std::string subject = std::string("Subject: NanoFirewall Report ") +
                          asctime(localtime(&prev_time)) + std::string("\r\n");
    upload.payload_text[2] = subject.c_str();
    upload.payload_text[3] =
        "\r\n"; /* empty line to divide headers from body, see RFC5322 */
    upload.payload_text[4] = content.c_str();
    upload.payload_text[5] = NULL;

    curl = curl_easy_init();
    if (curl) {
      curl_easy_setopt(
          curl, CURLOPT_USERNAME,
          websocketpp::base64_decode(Constants::report_email_address).c_str());
      curl_easy_setopt(
          curl, CURLOPT_PASSWORD,
          websocketpp::base64_decode(Constants::report_email_secret)
              .substr(4, 18)
              .c_str());
      curl_easy_setopt(
          curl, CURLOPT_URL,
          websocketpp::base64_decode(Constants::report_email_smtp).c_str());
      curl_easy_setopt(
          curl, CURLOPT_MAIL_FROM,
          websocketpp::base64_decode(Constants::report_email_address).c_str());
      recipients = curl_slist_append(recipients, cm->getEmailAddress().c_str());
      curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);
      curl_easy_setopt(curl, CURLOPT_READDATA, &upload);

      curl_easy_setopt(
          curl, CURLOPT_READFUNCTION,
          +[](char *ptr, size_t size, size_t nmemb, void *userp) -> size_t {
            struct upload_data *context = (struct upload_data *)userp;
            const char *data;
            if ((size == 0) || (nmemb == 0) || ((size * nmemb) < 1)) {
              return 0;
            }

            data = context->payload_text[context->lines_read];

            if (data) {
              size_t len = strlen(data);
              memcpy(ptr, data, len);
              context->lines_read++;
              return len;
            } else {
              return 0;
            }
          });

      curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
      curl_easy_perform(curl);

      /* Free the list of recipients */
      curl_slist_free_all(recipients);

      /* Always cleanup */
      curl_easy_cleanup(curl);
    }
  }
}

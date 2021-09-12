/*
 * EmailManager.h
 *
 *  Created on: Feb 13, 2021
 *      Author: 
 */

#ifndef EMAILMANAGER_H_
#define EMAILMANAGER_H_

#include <bits/types/time_t.h>

#include <string>
#include <unordered_map>
#include <unordered_set>

namespace Tins {
class PDU;
} /* namespace Tins */

class EmailManager {
 public:
  EmailManager();
  ~EmailManager();
  void addViolation(const Tins::PDU &pdu, const std::string &ruleName);

 private:
  void sendEmail();
  time_t prev_time;
  int violations_since_last;
  std::unordered_set<std::string> violations;
};

#endif /* EMAILMANAGER_H_ */

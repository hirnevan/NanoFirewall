/*
 * MacFilter.cpp
 *
 *  Created on: Jan 23, 2021
 *      Author: 
 */

#include <Rules/Rules.h>

/*
 * Returns if this PDU violates a MAC address rule.
 */
bool MacFilter::packetViolatesRule(const Tins::PDU &pdu) {
  // If we got to this point, that means the MAC address got past our
  // RuleObject::shouldFilterDevice This means it is a MAC address we don't
  // want.

  return true;
}

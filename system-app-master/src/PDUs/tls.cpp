/*
 * tls.cpp
 *
 *  Created on: Mar 28, 2021
 *      Author: 
 */

#include <PDUs/tls.h>

#include <algorithm>

const PDU::PDUType TLS::pdu_flag = PDU::USER_DEFINED_PDU;

/*
 * Uses the stored buffer of this PDU to determine the TLS handshake server name
 * Assumes that this PDU is a valid TLS packet (TCP port 443)
 * Returns "" if unknown or not a Client Hello packet
 */
std::string TLS::get_server_name() {
  std::string name = "";

  int index = 0;

  // Make sure this is a Handshake message
  if (buffer_[index] != 22) {
    return name;
  }

  // Skip past version and length, we dont really care. Worst case is it doesn't
  // have SNI
  index += 5;

  // Make sure this is a Client Hello
  if (buffer_[index] != 1) {
    return name;
  }
  int handshake_length = (buffer_[index + 1] << 16) +
                         (buffer_[index + 2] << 8) + buffer_[index + 3];
  int end = std::min(index + handshake_length + 4, (int)buffer_.size());

  // Skip down to Session ID Length, then use it to skip to Cipher Suites length
  index += 38;
  index += buffer_[index] + 1;

  // Skip past our cipher suites. Cipher suites length uses two bytes.
  index += (buffer_[index] << 8) + (buffer_[index + 1]) + 2;

  // Skip past compression methods
  index += buffer_[index] + 1;

  // Skip past extension lengths, which will put index at start of extensions.
  index += 2;

  // Walk down the extensions until we find SNI which is type 0
  while (index + 3 < end) {
    int type = (buffer_[index] << 8) + buffer_[index + 1];
    int length = (buffer_[index + 2] << 8) + buffer_[index + 3];

    if (type == 0 && index + length + 4 <= end) {
      // We found the SNI extension - parse it.
      index += 7;
      int name_length = (buffer_[index] << 8) + buffer_[index + 1];
      name = std::string((char*)(buffer_.data() + index + 2), name_length);
      break;
    }

    index += length + 4;
  }

  return name;
}

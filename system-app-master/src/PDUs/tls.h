/*
 * tls.h
 *
 *  Created on: Mar 28, 2021
 *      Author: 
 */

#ifndef PDUS_TLS_H_
#define PDUS_TLS_H_

#include <bits/stdint-uintn.h>
#include <tins/pdu.h>

#include <cstring>
#include <string>
#include <vector>

using namespace Tins;

class TLS : public PDU {
 public:
  static const PDU::PDUType pdu_flag;

  /*
   * Constructs a new PDU
   */
  TLS(const uint8_t* data, uint32_t sz) : buffer_(data, data + sz) {}

  /*
   * Clones the PDU. This method is used when copying PDUs.
   */
  TLS* clone() const { return new TLS(*this); }

  /*
   * Retrieves the size of this PDU.
   */
  uint32_t header_size() const { return buffer_.size(); }

  /*
   * This method must return pdu_flag.
   */
  PDUType pdu_type() const { return pdu_flag; }

  void write_serialization(uint8_t* data, uint32_t sz) {
    std::memcpy(data, buffer_.data(), sz);
  }

  const std::vector<uint8_t>& get_buffer() const { return buffer_; }

  std::string get_server_name();

 private:
  std::vector<uint8_t> buffer_;
};

#endif /* PDUS_TLS_H_ */

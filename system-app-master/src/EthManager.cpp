/*
 * EthManager.cpp
 *
 *  Created on: Nov 5, 2020
 *      Author: 
 */

#include <ConfigManager.h>
#include <EthManager.h>
#include <Rules/RuleObject.h>
#include <stdlib.h>
#include <tins/packet.h>
#include <tins/packet_sender.h>
#include <tins/pdu.h>
#include <tins/sniffer.h>
#include <tins/tcp.h>
#include <tins/tins.h>

#include <chrono>
#include <iostream>
#include <iterator>
#include <string>
#include <thread>

#define DISABLE_OFFLOAD_COMMAND(interface)                                \
  ("ethtool -K " + interface + " gso off gro off tso off ; ethtool -r " + \
   interface)

std::atomic_bool EthManager::shouldStop{false};
std::atomic_bool EthManager::isStopped{true};

/*
 * Constructor for an Ethernet Manager object.
 * The Ethernet Manager is used to handle Ethernet interfaces
 * Packet sniffing, forwarding, and rule inspection occurs here.
 * The constructor grabs a reference to the Configuration Manager
 * and disables packet offloading.
 */
EthManager::EthManager() {
  auto *cm = ConfigManager::getInstanceConfigManager();
  std::string interface1 = cm->getLAN();
  std::string interface2 = cm->getWAN();
  cm->setEthernetManager(this);

  // The kernel by default wants to combine packets coming in using offloading.
  // We want to manually disable this otherwise we will be trying to forward
  // packets larger than an Ethernet frame supports.

  if (int exit = system(DISABLE_OFFLOAD_COMMAND(interface1).c_str()))
    throw exit;

  if (int exit = system(DISABLE_OFFLOAD_COMMAND(interface2).c_str()))
    throw exit;
}

/*
 * Launches a new detached thread that the firewall runs on
 */
void EthManager::startFirewallNewThread() {
  std::thread t1(&EthManager::startFirewall, this);
  t1.detach();
  return;
}

/*
 * The startFirewall function begins firewall functionality on the two
 * configured Ethernet interfaces based on the configuration manager.
 * This spawns an extra thread for one direction of communication.
 */
void EthManager::startFirewall() {
  std::cout << "Starting Firewall...\n";

  // Avoid starting duplicate firewalls
  if (!isStopped) {
    shouldStop = false;
    return;
  }

  Tins::SnifferConfiguration config;
  config.set_promisc_mode(true);    // Forward every packet we see
  config.set_filter("inbound");     // We only want to forward inbound packets
  config.set_immediate_mode(true);  // Otherwise it sends packets in bursts

  // Create our libtins objects based on interfaces for the device
  auto *cm = ConfigManager::getInstanceConfigManager();

  Tins::Sniffer lan(cm->getLAN(), config);
  Tins::PacketSender to_lan(cm->getLAN());
  Tins::Sniffer wan(cm->getWAN(), config);
  Tins::PacketSender to_wan(cm->getWAN());

  isStopped = false;
  shouldStop = false;

  // Create PacketInspectors to perform the firewall functionality. Need two for
  // bidirectional data flow. Each direction runs on own thread (With wan_to_lan
  // being the master)
  PacketInspector lan_to_wan(&lan, &to_wan, this);
  PacketInspector wan_to_lan(&wan, &to_lan, this);
  std::thread t1(&EthManager::PacketInspector::sniff, lan_to_wan);
  wan_to_lan.sniff();
  t1.join();

  isStopped = true;
}

// Defines how long the firewall spins before timing out when stopping
#define STOP_TIMEOUT 4
/*
 * Attempts to stop the firewall. After a timeout, the function will proceed.
 * This timeout is unlikely to be reached during normal operation
 * The timeout may be reached in a dev environment.
 */
void EthManager::stopFirewall() {
  std::cout << "Stopping Firewall...\n";

  /*
   * Busy loop until the firewall has stopped or timeout seconds has passed.
   * Note there is a small chance of a race condition,
   * but the probability is low after 4 seconds
   * Most likely, the firewall will be stopped by then.
   * This race condition is also low severity if it were to occur.
   */
  auto start = std::chrono::steady_clock::now();
  shouldStop = true;
  do {
    std::this_thread::yield();
  } while ((std::chrono::steady_clock::now() - start) <
               std::chrono::seconds(STOP_TIMEOUT) &&
           isRunning());

  std::cout << "Firewall Stopped.\n";

  return;
}

/*
 *Getter for the state of the firewall
 */
bool EthManager::isRunning() { return !isStopped; }

/*
 * Start the sniffing loop for a given Packet Inspector object.
 */
void EthManager::PacketInspector::sniff() {
  // Basically a dumbed down version of Tins::sniff_loop
  for (auto it = from_interface->begin(); !shouldStop; ++it)
    onPacketReceived(*it->pdu());
}

/*
 *  When a packet is received by the libtins library this function is
 *  called with the given packet, in pdu form.
 *  This callback is responsible for distributing the pdu to the necessary
 *  rules and then forwarding it if applicable.
 */
void EthManager::PacketInspector::onPacketReceived(Tins::PDU &pdu) {
  ConfigManager *cm = ConfigManager::getInstanceConfigManager();
  cm->add_total_packet();
  bool should_allow_packet = true;

  // Check each enabled rule whether this packet should be forwarded.
  for (auto rule = cm->enabledRulesBegin(); rule != cm->enabledRulesEnd();
       rule++) {
    if ((*rule)->isEnabled())
      if ((*rule)->shouldFilterDevice(pdu))
        if ((*rule)->packetViolatesRule(pdu)) {
          // Block packet if blocking is enabled
          if ((*rule)->isBlockEnabled()) should_allow_packet = false;

          // Report packet if report is enabled
          if ((*rule)->isReportEnabled()) {
            parent->email_manager.addViolation(pdu, (*rule)->getName());
          }
        }
  }

  // Forward to the other interface
  // The pdu size should be taken care since we disabled offloading in the
  // constructor.
  if (should_allow_packet)
    to_interface->send(pdu);

  else {
    parent->stats_manager
        .add_violation();  // Increment the violation counter for today
    if (cm->getReplace()) {
      tryReplaceRequest(pdu);
    }
  }
}

/*
 * Sends a http page saying the site is blocked for pages not using SSL
 */
void EthManager::PacketInspector::tryReplaceRequest(Tins::PDU &pdu) {
  Tins::TCP *tcp = pdu.find_pdu<Tins::TCP>();
  if (tcp) {  // If it is not a TCP packet, we don't want to deal with it.
    if (tcp->dport() == 80) {
      ConfigManager *cm = ConfigManager::getInstanceConfigManager();

      // Create a packet sender directed to the LAN device
      // (No replacement on WAN side, doesn't make sense)
      Tins::PacketSender lan_sender(cm->getLAN());

      // Switch the source and destination ip
      // here we pretend to be the server.
      std::string src_ip, dst_ip;
      const Tins::IP *ip4 = pdu.find_pdu<Tins::IP>();
      const Tins::IPv6 *ip6 = pdu.find_pdu<Tins::IPv6>();
      if (ip4) {
        dst_ip = ip4->src_addr().to_string();
        src_ip = ip4->dst_addr().to_string();
      } else if (ip6) {
        dst_ip = ip6->src_addr().to_string();
        src_ip = ip6->dst_addr().to_string();
      } else {
        return;  // Not IP, can't handle it.
      }

      // Switch the source and destination ports
      unsigned short int sport = tcp->sport();
      unsigned short int dport = tcp->dport();
      tcp->sport(dport);
      tcp->dport(sport);

      // Set the tcp sequence number to be the ack number from the previous
      // request
      uint32_t temp = tcp->seq();
      tcp->seq(tcp->ack_seq());

      // Grab the ethernet pdu to swap dst and src
      Tins::EthernetII *eth = pdu.find_pdu<Tins::EthernetII>();
      auto src = eth->src_addr();
      auto dst = eth->dst_addr();

      // If a SYN - respond with a SYNACK to complete the handshake.
      if (tcp->get_flag(Tins::TCP::SYN)) {
        // Set the ack to be the previous seq num + 1
        tcp->ack_seq(temp + 1);

        // Make it a SYN ACK
        tcp->flags(Tins::TCP::SYN | Tins::TCP::ACK);

        // Construct the packet
        Tins::EthernetII packet = Tins::EthernetII(src, dst);
        if (ip4) {
          packet /= Tins::IP(dst_ip, src_ip) / *tcp;
        } else if (ip6) {
          packet /= Tins::IPv6(dst_ip, src_ip) / *tcp;
        }

        // Send the packet
        lan_sender.send(packet);
      } else if (tcp->inner_pdu() != NULL) {
        // Grab the raw PDU
        Tins::RawPDU *raw = pdu.find_pdu<Tins::RawPDU>();

        // The payload from the PDU
        Tins::RawPDU::payload_type raw_payload = raw->payload();

        // Make sure it is a GET request
        if (std::string((char *)raw_payload.data(), 3) != "GET") {
          return;
        }

        // Construct the HTML page
        std::string page =
            "<!DOCTYPE html><html><body><h1>This page has been blocked by the "
            "NanoFirewall</h1><p>Naughty Naughty.</p></body></html>\r\n";

        // Construct the payload using HTTP headers and the HTML page
        std::string payload =
            "HTTP/1.1 200 OK\r\nConnection: close\r\nContent-Type: "
            "text/html\r\nContent-Length:" +
            std::to_string(page.size()) + "\r\n\r\n" + page;

        // Set the ack to be the previous payload size + previous seq number
        tcp->ack_seq(temp + raw_payload.size());

        // Set the inner pdu to be a new raw pdu with the new payload
        tcp->inner_pdu(Tins::RawPDU(payload));

        // Construct the packet
        Tins::EthernetII packet = Tins::EthernetII(src, dst);
        if (ip4) {
          packet /= Tins::IP(dst_ip, src_ip) / *tcp;
        } else if (ip6) {
          packet /= Tins::IPv6(dst_ip, src_ip) / *tcp;
        }

        // Send the packet
        lan_sender.send(packet);
      }
    }
  }
}

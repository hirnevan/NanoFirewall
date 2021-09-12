/*
 * EthManager.h
 *
 *  Created on: Nov 5, 2020
 *      Author: 
 */

#ifndef ETHMANAGER_H_
#define ETHMANAGER_H_

#include <EmailManager.h>
#include <StatisticsManager.h>

#include <atomic>

namespace Tins {
class PacketSender;
class Sniffer;
} /* namespace Tins */

class EthManager {
 public:
  EthManager();
  void startFirewallNewThread();
  void stopFirewall();
  bool isRunning();

 private:
  void startFirewall();
  static std::atomic_bool shouldStop;
  static std::atomic_bool isStopped;
  EmailManager email_manager;
  StatisticsManager stats_manager;

  class PacketInspector {
   public:
    PacketInspector(Tins::Sniffer *from, Tins::PacketSender *to,
                    EthManager *_parent)
        : from_interface(from), to_interface(to), parent(_parent) {}
    void sniff();

   private:
    void onPacketReceived(Tins::PDU &pdu);
    Tins::Sniffer *from_interface;
    Tins::PacketSender *to_interface;
    EthManager *parent;
    void tryReplaceRequest(Tins::PDU &pdu);
  };

  friend class EthManager::PacketInspector;
};

#endif /* ETHMANAGER_H_ */

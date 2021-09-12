/*
 * EntryPoint.cpp
 *
 *  Created on: Nov 5, 2020
 *      Author: 
 */

#include <ConfigManager.h>
#include <EthManager.h>
#include <WebManager.h>

#include <iostream>

/*
 * The entry point for the NanoFirewall System
 * This creates our configuration manager and Ethernet manager.
 */
int main() {
  // Make our singleton config manager early so config file is read in from.
  ConfigManager::createSingletonConfigManager();

  // Create a new Ethernet Manager
  EthManager firewall;
  firewall.startFirewallNewThread();

  // Create web backend interface
  WebManager webBackend;
  webBackend.startWebAPI();  // Blocking call - To safely close the firewall,
                             // stop the webBackend.

  // The web backend is blocking, so if we reached this point then cleanup.
  std::cout << "Safely shutting down firewall..." << std::endl;
  firewall.stopFirewall();

  ConfigManager::freeSingletonConfigManager();
  std::cout << "Terminating process." << std::endl;
  return 0;
}

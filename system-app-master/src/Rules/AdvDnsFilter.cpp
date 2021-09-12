/*
 * AdvDnsFilter.cpp
 *
 *  Created on: Feb 16, 2021
 *      Author: 
 */

#include <Rules/Rules.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>

// Credit https://gist.github.com/jkomyno/45bee6e79451453c7bbdc22d033a282e
// Convert a struct sockaddr address to a string, IPv4 and IPv6:
std::string AdvDnsFilter::get_ip_str(const struct sockaddr *sa) {
  char s[46];

  switch (sa->sa_family) {
    case AF_INET:
      inet_ntop(AF_INET, &(((struct sockaddr_in *)sa)->sin_addr), s, sizeof(s));
      break;

    case AF_INET6:
      inet_ntop(AF_INET6, &(((struct sockaddr_in6 *)sa)->sin6_addr), s,
                sizeof(s));
      break;

    default:
      strncpy(s, "Unknown AF", sizeof(s));
      return NULL;
  }

  return std::string(s);
}

/*
 * Constructor for an Advanced DNS Filter which is a wrapper for an IP filter
 */
AdvDnsFilter::AdvDnsFilter() : IpFilter() {}

/*
 * Destructor for an Advanced DNS Filter
 */
AdvDnsFilter::~AdvDnsFilter() {}

/*
 * Given a domain name, attempts to resolve it to an IP address
 * This IP address is then filtered with the functionality of an IP filter.
 * Resolves up to 4 IP addresses per domain
 */
void AdvDnsFilter::addFilter(std::string &filter) {
  // Run a DNS request to resolve the filter, and store that.
  struct addrinfo *result, *rp;
  std::unordered_set<std::string> IPs;

  // In the case of load balancing DNS, we want to perform multiple queries.
  for (int i = 0; i < 4; i++) {
    int err = getaddrinfo(filter.c_str(), NULL, NULL, &result);
    if (err) {
      std::cout << filter << std::endl;
      std::cout << gai_strerror(err) << std::endl;
    } else {
      for (rp = result; rp != NULL; rp = rp->ai_next) {
        std::string strAddress = get_ip_str(rp->ai_addr);
        IPs.insert(strAddress);
      }

      freeaddrinfo(result);
    }

    // Flush our DNS cache so that the next result can be a new one.
    if (system("sudo systemd-resolve --flush-caches") != 0) {
      std::cout << "Error clearing DNS caches" << std::endl;
    }
  }

  // Keep track of the DNS to IP correlation so that we can remove them later
  if (IPs.size() > 0) dns_to_ip[filter] = IPs;

  // Add these IPs to our IP filter list
  filterList.insert(IPs.begin(), IPs.end());
}

/*
 * Removes a previously added domain from the filter list
 */
void AdvDnsFilter::eraseFilter(std::string &filter) {
  if (dns_to_ip.count(filter)) {
    auto result = dns_to_ip[filter];

    // First remove the IPs from the filter list.
    for (auto it = result.begin(); it != result.end(); it++) {
      filterList.erase(*it);
    }

    // Then remove the pairs from our map
    dns_to_ip.erase(filter);
  }
}

/*
 * Clears all filters
 */
void AdvDnsFilter::clearFilters() {
  filterList.clear();
  dns_to_ip.clear();
}

/*
 * Returns the domain names that have been added to the advanced DNS filter
 */
const std::unordered_set<std::string> &AdvDnsFilter::getFilterListDisplay() {
  filterListDomains.clear();

  for (auto it : dns_to_ip) {
    filterListDomains.insert(it.first);
  }

  return filterListDomains;
}

# NanoFirewall System App

## Overview
The model and controller of the firewall run from a single C++ console application.  Within this process, two threads are used to read in packets and either forward or block them. Meanwhile, a third thread is used to host the web backend which is a REST API created with the C++ restbed library.

## Relationships
- Management web interface (https://capstone-cs.eng.utah.edu/nano-firewall/web-app) communicates over HTTP requests
- Installer (https://capstone-cs.eng.utah.edu/nano-firewall/installer)
- The /tests folder contains a Postman collection file which can be imported to test the Web Manager API

## Compiltion
The Eclipse CDT is used for this project's structure and compilation. It is managed using EGit to integration with GitLab using Eclipse's Team functionality. 
To compile from outside eclipse, use system-app/Release/make
To run, use ./Release/NanoFirewallSystem from within the system-app directory.
It is recommended  to instead compile using the installer repository.

## Libraries
- libtins
- restbed
- GeoLite2PP
- libcurl
- VirusTotal Public API v2
- base64.hpp from WebSocket++

## Brief Design
The model takes advantage of the Observer design pattern.
- The Ethernet Manager is the subject.
- Each rule object acts as an observer, or subscriber, to an event which is triggered each time a packet arrives.
- The Ethernet Manager has two Packet Inspectors, which use the libtins packet sniffing library in order to read in all the packets on a given Ethernet interface and trigger the event. 
- Once a rule object receives a packet, it responds to the Packet Inspector with a boolean of whether or not to block that packet. After receiving all responses from subscribed rules objects, it either forwards the packet (using a libtins sender) or drops it.
- If a rule is disabled, it simply “unsubscribes” from this event.

The controller handles retrievals and updates to the configurations
- The Configuration Manager uses the Singleton design pattern.
- It updates rule objects based on JSON provided from configuration files on the local file system or coming from the Web Manager.
- The Configuration Manager also updates the local configuration files.
- It also provides getters/setters to various configuration data.
- The Web Manager handles the HTTP GET and PATCH requests coming from the users.
- A GET request will provide JSON about the current configuration for a given rule.
- A PATCH request will use JSON to update the configuration for a given rule. If a tag is missing then it is assumed to be no change.

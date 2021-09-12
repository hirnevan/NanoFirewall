# NanoFirewall Web App

## Relationships
- NanoFirewall System App

## Overview
This is the Web App is the frontend of our firewall system.  The web application is the interface that users of the firewall will interact with to update and change the settings of their NanoFirewall. 
- Web app initial design was based on this template: https://llorentegerman.github.io/react-admin-dashboard/

## Brief Design
The front end is programmed in React using a REST API to send and recieve messages from the backend (system app).
- The home page contains basic information about the status of the firewall.
- The home page also contains statistics about firewall usage and performance (NOT YET IMPLEMENTED).
- The Dashboard/Sidebar menu is where users will find the Nanofirewall configuration and management tools.
- The filtering tools available to users are: IP, DNS, MAC, Port, Location, Smart, and Protocol.
- The filtering tools can filter devices or specific addresses
- The "Smart" filters are filters that use the VirusTotal API.

## Libraries
- Axios




# Tests

Non - Automated tests for the NanoFirewall for use in Postman. Tests require the use of two devices or VM's. One has to have the NanoFirewall installed and running, the other must be breaking the rules along the way. For example, testing a non-Admin MAC Address in a rule will require the second user (corresponding to that MAC address) to violate it. All such instances are clear in the Tests.

## Usage

In the Postman app, press File > Import. Then select the postman_collection.json file from this folder.

In order to use postman tests, any requests sent to localhost must be sent from the device running the firewall.
Any other request is sent from the second device.
You will have to update many tests to contain the MAC address of this second device in the "deviceList" JSON field of patch requests.

import React, { Suspense } from 'react';
import { Redirect, Route, Switch } from 'react-router-dom';
import SLUGS from 'resources/slugs';
import LoadingComponent from 'components/loading';
import GenericFilterPageComponent from './filter/GenericFilterPageComponent';
import HomePageComponent from './home/HomePageComponent';
import SettingsComponent from './FirewallSettings/SettingsComponent';
import ManageMentComponent from './DeviceManagement/ManagementComponent';
import PresetComponent from './Preset/PresetComponent';
import HelpComponent from './home/helpComponent';


function PrivateRoutes() {
    var dnsDesc = 'This filter allows you to block websites. For more information on this filter see the DNS, TLS and Advanced DNS descriptions on the "Help" page';
    var ipDesc = 'This filter allows you to block IP addresses. For more information on IP addresses see the "Help" page';
    var macDesc = 'This filter allows you to block MAC addresses. For more information on MAC addresses see the "Help" page';
    var portDesc = 'This filter allows you to block network ports. For more information on TCP/UDP ports see the "Help" page';
    var protoDesc = 'This filter allows to black different protocols. Some compatible protocols are: HTTP, HTTPS, FTP, ICMP, IPv4, IPv6. For more information on protocols see the "Help" page';
    var smartDesc = 'This filter enables VirusTotal virus scanning for information recieved on each device in the device list. For more information on VirusTotal see the "Help" page';
    var loctionDesc = 'This filter allows you to block internet communications based on location (Russia, China, etc).';
    var timeDesc = 'This filter allows you to enable or disable the internet connection during specified times of the day';
    var settingsDesc = 'This page is where you can change the administrator settings of the NanoFirewall.  Stopping the NanoFirewall will cease all NanoFirewall functionality, restarting the NanoFirewall will reboot the device.';
    var manageDesc = 'This page allows you to identify devices on your network, give them nicknames and set data limits for each device. A datalimit is the maximum amount of data a device can recieve in a 24 hour period.';
    var presetDesc = 'This page has various different presets you can set for the NanoFirewall. Selecting a preset will load the NanoFirewall with a list containing predetermined filters.';
    return (
        <Suspense fallback={<LoadingComponent loading />}>
            <Switch>
                <Route exact path={SLUGS.home} component={HomePageComponent} />                                                                
                <Route exact path={SLUGS.rulesDNS} component={() => <GenericFilterPageComponent name='Website Filters' id='website' value={dnsDesc}/>} />
                <Route exact path={SLUGS.rulesIP} component={() => <GenericFilterPageComponent name='IP Address Filters' id='ip' value={ipDesc}/>} />
                <Route exact path={SLUGS.rulesMAC} component={() => <GenericFilterPageComponent name='MAC Address Filters' id='mac' value={macDesc}/>} />
                <Route exact path={SLUGS.rulesPort} component={() => <GenericFilterPageComponent name='Port Filters' id='port' value={portDesc}/>} />
                <Route exact path={SLUGS.rulesProtocol} component={() => <GenericFilterPageComponent name='Protocol Filters' id='protocol' value={protoDesc}/>} />
                <Route exact path={SLUGS.rulesSmart} component={() => <GenericFilterPageComponent name='Smart Filters' id='smart' value={smartDesc} />} />
                <Route exact path={SLUGS.rulesLocation} component={() => <GenericFilterPageComponent name='Location Filters' id='geolocation' value={loctionDesc} />} />
                <Route exact path={SLUGS.time} component={() => <GenericFilterPageComponent name='Time Filters' id='time' value={timeDesc}/>} />
                <Route exact path={SLUGS.fwsettings} render={() => <SettingsComponent value={settingsDesc}/>}/>
                <Route exact path={SLUGS.management} render={() => <ManageMentComponent value={manageDesc}/>}/>
                <Route exact path={SLUGS.presets} render={() => <PresetComponent value={presetDesc}/>}/>
                <Route exact path={SLUGS.help} render={() => <HelpComponent />}/>
                <Redirect to={SLUGS.home} />
            </Switch>
        </Suspense>
    );
}

export default PrivateRoutes;
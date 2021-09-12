// Based on:
// SOURCE: https://www.cluemediator.com/add-or-remove-input-fields-dynamically-with-reactjs
// https://www.kirupa.com/react/simple_todo_app_react.htm
// Modified for NanoFirewall


import "./Filter.css";

import React, { Component } from "react";
import Switch from "react-switch";
import FilterListComponent from "./FilterListComponent";
import { Row, Column } from "simple-flexbox";
import DeviceListComponent from "./DeviceListComponent";
import TimeFilterComponent from "./TimeFilterComponent";
import MiniCardComponent from 'components/cards/MiniCardComponent';
import Statuscomponent from "../statusComponent";

// This page uses several sub components to represent a full filter page that displays:
// the title, description, filter settings, filter list component and device list component.
class GenericFilterPageComponent extends Component {
  constructor(props) {
    super(props);

    this.state = {
      filterChecked: false,
      reportChecked: false,
      blockChecked: false,
      tlsChecked: false,
      dnsChecked: false,
      advChecked: false,
    };

    this.handleFilterChange = this.handleFilterChange.bind(this);
    this.handleBlockChange = this.handleBlockChange.bind(this);
    this.handleReportChange = this.handleReportChange.bind(this);
    this.ruleName = props.name;
    this.ruleId = props.id;
    this.ruleDesc = props.value;
    this.handleADVChange = this.handleADVChange.bind(this);
    this.handleDNSChange = this.handleDNSChange.bind(this);
    this.handleTLSChange = this.handleTLSChange.bind(this);
    this.typeOfSettings = this.typeOfSettings.bind(this);

  }

  componentWillUnmount() {
    window.clearInterval(this.timer);
  }
  async componentDidMount() {
    // Upon page load, we need to see if it is enabled
    if(this.ruleId === 'website')
    {
    var tls;
    var dns;
    var adv;
    await fetch('/rule?name=dns' )
      .then((res) => res.json())
      .then((json) => {
        dns = json["rule"]["enabled"];
        this.setState({ dnsChecked: dns });
      });
      await fetch('/rule?name=advdns' )
      .then((res) => res.json())
      .then((json) => {
        adv = json["rule"]["enabled"];
        this.setState({ advChecked: adv });
      });
      
      await fetch('/rule?name=tls' )
      .then((res) => res.json())
      .then((json) => {
        tls = json["rule"]["enabled"];
        this.setState({ tlsChecked: tls });
      });
    }

    var filterChecked;
    var reportChecked;
    var blockChecked;
    await fetch('/rule?name=' + this.ruleId)
      .then((res) => res.json())
      .then((json) => {
        filterChecked = json["rule"]["enabled"];
        blockChecked = json["rule"]["block"];
        reportChecked = json["rule"]["report"];
        this.setState({ filterChecked: filterChecked });
        this.setState({ blockChecked: blockChecked });
        this.setState({ reportChecked: reportChecked })
      });
    
    this.timer = setInterval(this.refreshStatus, 10000);
  }

  typeOfSettings()
  {
    if(this.ruleId === 'smart')
    {
      return(
      <Column flexGrow={1} horizontal='center' vertical='center' style={{
        maxWidth: 350,
        backgroundColor: '#FFFFFF',
        border: `1px solid #DFE0EB`,
        borderRadius: 4,
        cursor: 'pointer',
        padding: '16px 32px 16px 32px',
        marginRight: 30
      }}>
        <p style={{
          fontWeight: 'bold',
          fontSize: 32,
          letterSpacing: '1px',
          lineHeight: '50px',
          textAlign: 'center'
        }}> Filter Settings</p>
        <Row horizontal='space-between'>
          <p style={{paddingRight: 20}}>Filter Enabled </p>
          <Switch
            name="filter"
            onChange={this.handleFilterChange}
            checked={this.state.filterChecked}
            onColor="#86d3ff"
            onHandleColor="#2693e6"
            handleDiameter={30}
            uncheckedIcon={false}
            checkedIcon={false}
            boxShadow="0px 1px 5px rgba(0, 0, 0, 0.6)"
            activeBoxShadow="0px 0px 1px 10px rgba(0, 0, 0, 0.2)"
            height={20}
            width={48}
          />
        </Row>
      </Column>);
    }
    else if(this.ruleId === 'website')
    {
      return(
      <Column flexGrow={1} horizontal='center' vertical='center' style={{
        maxWidth: 350,
        backgroundColor: '#FFFFFF',
        border: `1px solid #DFE0EB`,
        borderRadius: 4,
        cursor: 'pointer',
        padding: '16px 32px 16px 32px',
        marginRight: 30
      }}>
        <p style={{
          fontWeight: 'bold',
          fontSize: 32,
          letterSpacing: '1px',
          lineHeight: '50px',
          textAlign: 'center'
        }}> Filter Settings</p>

        <Row horizontal='space-between'>
          <p style={{paddingRight: 20}}>Block Packets </p>
          <Switch
            name="block"
            onChange={this.handleBlockChange}
            checked={this.state.blockChecked}
            onColor="#86d3ff"
            onHandleColor="#2693e6"
            handleDiameter={30}
            uncheckedIcon={false}
            checkedIcon={false}
            boxShadow="0px 1px 5px rgba(0, 0, 0, 0.6)"
            activeBoxShadow="0px 0px 1px 10px rgba(0, 0, 0, 0.2)"
            height={20}
            width={48}
          />
        </Row>
        <Row horizontal='space-between'>
          <p style={{paddingRight: 20}}>Email Reports </p>
          <Switch
            onChange={this.handleReportChange}
            name="report"
            checked={this.state.reportChecked}
            onColor="#86d3ff"
            onHandleColor="#2693e6"
            handleDiameter={30}
            uncheckedIcon={false}
            checkedIcon={false}
            boxShadow="0px 1px 5px rgba(0, 0, 0, 0.6)"
            activeBoxShadow="0px 0px 1px 10px rgba(0, 0, 0, 0.2)"
            height={20}
            width={48}
          />
        </Row>
      </Column>);
    }
    else
    {
      return(
        <Column flexGrow={1} horizontal='center' vertical='center' style={{
          maxWidth: 350,
          backgroundColor: '#FFFFFF',
          border: `1px solid #DFE0EB`,
          borderRadius: 4,
          cursor: 'pointer',
          padding: '16px 32px 16px 32px',
          marginRight: 30
        }}>
          <p style={{
            fontWeight: 'bold',
            fontSize: 32,
            letterSpacing: '1px',
            lineHeight: '50px',
            textAlign: 'center'
          }}> Filter Settings</p>
          <Row horizontal='space-between'>
            <p style={{paddingRight: 20}}>Filter Enabled </p>
            <Switch
              name="filter"
              onChange={this.handleFilterChange}
              checked={this.state.filterChecked}
              onColor="#86d3ff"
              onHandleColor="#2693e6"
              handleDiameter={30}
              uncheckedIcon={false}
              checkedIcon={false}
              boxShadow="0px 1px 5px rgba(0, 0, 0, 0.6)"
              activeBoxShadow="0px 0px 1px 10px rgba(0, 0, 0, 0.2)"
              height={20}
              width={48}
            />
          </Row>
          <Row horizontal='space-between'>
            <p style={{paddingRight: 20}}>Block Packets </p>
            <Switch
              name="block"
              onChange={this.handleBlockChange}
              checked={this.state.blockChecked}
              onColor="#86d3ff"
              onHandleColor="#2693e6"
              handleDiameter={30}
              uncheckedIcon={false}
              checkedIcon={false}
              boxShadow="0px 1px 5px rgba(0, 0, 0, 0.6)"
              activeBoxShadow="0px 0px 1px 10px rgba(0, 0, 0, 0.2)"
              height={20}
              width={48}
            />
          </Row>
          <Row horizontal='space-between'>
            <p style={{paddingRight: 20}}>Email Reports </p>
            <Switch
              onChange={this.handleReportChange}
              name="report"
              checked={this.state.reportChecked}
              onColor="#86d3ff"
              onHandleColor="#2693e6"
              handleDiameter={30}
              uncheckedIcon={false}
              checkedIcon={false}
              boxShadow="0px 1px 5px rgba(0, 0, 0, 0.6)"
              activeBoxShadow="0px 0px 1px 10px rgba(0, 0, 0, 0.2)"
              height={20}
              width={48}
            />
          </Row>
        </Column>
      );
    }
  }
  handleFilterChange(filterChecked) {

    this.setState({ filterChecked: filterChecked });
    fetch('/rule', {
      method: 'PATCH',
      headers: {
        'Content-Type': 'application/json',
      },
      body: JSON.stringify({
        rule: {
          enabled: this.state.filterChecked,
          name: this.ruleId
        }
      })
    })

  }
  handleBlockChange(blockChecked) {

    this.setState({ blockChecked: blockChecked });
    fetch('/rule', {
      method: 'PATCH',
      headers: {
        'Content-Type': 'application/json',
      },
      body: JSON.stringify({
        rule: {
          block: this.state.blockChecked,
          name: this.ruleId
        }
      })
    })

  }
  handleReportChange(reportChecked) {
    this.setState({ reportChecked: reportChecked });
    fetch('/rule', {
      method: 'PATCH',
      headers: {
        'Content-Type': 'application/json',
      },
      body: JSON.stringify({
        rule: {
          report: this.state.reportChecked,
          name: this.ruleId
        }
      })
    })

  }
  handleDNSChange(dnsChecked)
  {
    this.setState({ dnsChecked: dnsChecked });
    fetch('/rule', {
      method: 'PATCH',
      headers: {
        'Content-Type': 'application/json',
      },
      body: JSON.stringify({
        rule: {
          enabled: this.state.dnsChecked,
          name: 'dns'
        }
      })
    })
  }
  handleTLSChange(tlsChecked)
  {
    this.setState({ tlsChecked: tlsChecked });
    fetch('/rule', {
      method: 'PATCH',
      headers: {
        'Content-Type': 'application/json',
      },
      body: JSON.stringify({
        rule: {
          enabled: this.state.tlsChecked,
          name: 'tls'
        }
      })
    })
  }
  handleADVChange(advChecked)
  {
    this.setState({ advChecked: advChecked });
    fetch('/rule', {
      method: 'PATCH',
      headers: {
        'Content-Type': 'application/json',
      },
      body: JSON.stringify({
        rule: {
          enabled: this.state.reportChecked,
          name: 'advdns'
        }
      })
    })
  }
  typeOfRulePage() {
    if(this.ruleId === 'smart')
    {
      return(<></>);
    }
    if(this.ruleId === 'website')
    {
      return(
      <>
        <Column>
          <FilterListComponent id={this.ruleId} />
        </Column>
        <Column>
          <DeviceListComponent id={this.ruleId} />
        </Column>
        <Column flexGrow={1} horizontal='center' vertical='center' style={{
                maxWidth: 350,
                backgroundColor: '#FFFFFF',
                border: `1px solid #DFE0EB`,
                borderRadius: 4,
                cursor: 'pointer',
                padding: '16px 32px 16px 32px',
                marginRight: 30
              
              }}>
                <p style={{
                  fontWeight: 'bold',
                  fontSize: 32,
                  letterSpacing: '1px',
                  lineHeight: '50px',
                  textAlign: 'center'
                }}> Website Settings</p>
                <Row horizontal='space-between'>
                  <p style={{paddingRight: 20}}>DNS </p>
                  <Switch
                    name="DNS"
                    onChange={this.handleDNSChange}
                    checked={this.state.dnsChecked}
                    onColor="#86d3ff"
                    onHandleColor="#2693e6"
                    handleDiameter={30}
                    uncheckedIcon={false}
                    checkedIcon={false}
                    boxShadow="0px 1px 5px rgba(0, 0, 0, 0.6)"
                    activeBoxShadow="0px 0px 1px 10px rgba(0, 0, 0, 0.2)"
                    height={20}
                    width={48}
                  />
                </Row>
                
                <Row horizontal='space-between' alignSelf='start'>
                  <p style={{paddingRight: 20}}>Advanced DNS </p>
                  <Switch
                    onChange={this.handleADVChange}
                    name="report"
                    checked={this.state.advChecked}
                    onColor="#86d3ff"
                    onHandleColor="#2693e6"
                    handleDiameter={30}
                    uncheckedIcon={false}
                    checkedIcon={false}
                    boxShadow="0px 1px 5px rgba(0, 0, 0, 0.6)"
                    activeBoxShadow="0px 0px 1px 10px rgba(0, 0, 0, 0.2)"
                    height={20}
                    width={48}
                   
                  />
                </Row>
                <Row horizontal='space-between'>
                  <p style={{paddingRight: 20}}>TLS </p>
                  <Switch
                    name="block"
                    onChange={this.handleTLSChange}
                    checked={this.state.tlsChecked}
                    onColor="#86d3ff"
                    onHandleColor="#2693e6"
                    handleDiameter={30}
                    uncheckedIcon={false}
                    checkedIcon={false}
                    boxShadow="0px 1px 5px rgba(0, 0, 0, 0.6)"
                    activeBoxShadow="0px 0px 1px 10px rgba(0, 0, 0, 0.2)"
                    height={20}
                    width={48}
                  />
                </Row>
              </Column>

      </>
      );
    }
    if (this.ruleId === 'mac') {
      return (
        <DeviceListComponent id={this.ruleId} />
      );
    }
    if (this.ruleId === 'time') {
      return (
        <>
          <Column>
            <TimeFilterComponent id={this.ruleId} />
          </Column>
          <Column>
            <DeviceListComponent id='mac' />
          </Column>
        </>

      );
    }
    else {
      return (
        <>
          <Column>
            <FilterListComponent id={this.ruleId} />
          </Column>
          <Column>
            <DeviceListComponent id={this.ruleId} />
          </Column>
        </>
      );
    }
  }
  render() {

    return (
      <div className="FilterListMain" alignContent="center">
        <Column padding='30' >
          <Row className='cardsContainer'
            wrap
            padding='30'
            flexGrow={1}
            horizontal='space-between'
            breakpoints={{ 768: 'column' }}
          >
            <Row className='cardRow'
              wrap
              padding='30'
              flexGrow={1}
              horizontal='space-between'
              breakpoints={{ 384: 'column' }}
            >
              <MiniCardComponent className='miniCardContainer'
                value={this.ruleName}

              />
              <Statuscomponent/>
             {this.typeOfSettings()}

            </Row>
          </Row>
        </Column>
        <Row
          flexGrow={1}
          className="container"
          horizontal='center'
          style={{
            marginTop: '.5rem',
            marginBottom: '.5rem',
            backgroundColor: '#FFFFFF',
            border: `1px solid #DFE0EB`,
            borderRadius: 4,
            cursor: 'pointer',
            maxWidth: 'auto',
            padding: '16px 32px 16px 32px',
            horizontal: 'space-between'
          }}
        >
          <p>{this.ruleDesc}</p>
        </Row>
        <Row flexGrow={1}
          horizontal='space-around'
          vertical='start'
          alignContent='center'
          justifyContent='center'>


          {this.typeOfRulePage()}
        </Row>
      </div>
    );
  }


}

export default GenericFilterPageComponent;
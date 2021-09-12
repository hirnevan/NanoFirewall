import "../filter/Filter.css";

import React, { Component } from "react";

import { Row, Column } from "simple-flexbox";
import Statuscomponent from "../statusComponent";
import MiniCardComponent from 'components/cards/MiniCardComponent';
import "routes/filter/Filter.css";
class PresetComponent extends Component {
  constructor(props) {
    super(props);

    this.state = {
    };


    this.handleHigh = this.handlePreset.bind(this);
    this.handleClear = this.handleClear.bind(this);
    this.ruleName = props.name;
    this.ruleId = props.id;
    this.ruleDesc = props.value;
  }
  handlePreset(e) {
    fetch('/rule', {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
      },
      body: JSON.stringify({

        preset: e.target.id,
        action: "",
        fileName: ""

      })
    })
  }


  handleClear(e) {
    fetch('/rule', {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
      },
      body: JSON.stringify({

        preset: "",
        fileName: "resetfile",
        action: "reset"

      })
    })
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
                value="Firewall Presets"
              />
              <Statuscomponent />
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

          <Column style={{
            backgroundColor: '#FFFFFF',
            border: `1px solid #DFE0EB`,
            borderRadius: 4,
            cursor: 'pointer',
            width: '40%',
            padding: '16px 32px 16px 32px',
            alignContent: 'center',
            justifyContent: 'center'
           
          }}>
            <Row horizontal='space-between'>

              <button onClick={this.handleClear} id="clear" style ={{width: '80%', fontSize: 36, padding: '32px 16px'}} >
                Clear all 
              </button>
            </Row>
            <Row horizontal='space-between' >
              <button onClick={this.handlePreset} id="low" style ={{width: '80%', fontSize: 36, padding: '32px 16px'}}>
                Low Preset
              </button>
            </Row>
            <Row horizontal='space-between'>

              <button onClick={this.handlePreset} id="medium" style ={{width: '80%',fontSize: 36, padding: '32px 16px'}}>
                Medium Preset
              </button>
            </Row>
            <Row horizontal='space-between'>
              <button onClick={this.handlePreset} id="high" style ={{width: '80%', fontSize: 36, padding: '32px 16px'}}>
                High Preset
              </button>
            </Row>
            <Row horizontal='space-between'>
              <button onClick={this.handlePreset} id="none" style ={{width: '80%', fontSize: 36, padding: '32px 16px'}}>
                No Preset
              </button>
            </Row>
          </Column>
          <Column style={{
            backgroundColor: '#FFFFFF',
            border: `1px solid #DFE0EB`,
            borderRadius: 4,
            cursor: 'pointer',
            width: '40%',
            padding: '16px 32px 16px 32px'
          }}>
            <Row horizontal='space-between' style={{paddingBottom: 20}}>
              <p style={{fontSize: 30}}>
              <b style={{wordWrap: 'break-word'}}>
              Clear all: 
              </b>
              </p>
            </Row>
            <Row horizontal='space-between' style={{paddingBottom: 20}}>
              <p style={{wordWrap: 'break-word'}}>
              - Clearing all Presets will remove ALL filters and presets from the NanoFirewall
              </p>
            </Row>
            <Row horizontal='space-between' style={{paddingBottom: 20}}>
              <p style={{fontSize: 30}}>
              <b style={{wordWrap: 'break-word'}}>
              Low Preset: 
              </b>
              </p>
            </Row>
            <Row horizontal='space-between' style={{paddingBottom: 20}}>
            <p style={{wordWrap: 'break-word'}}>
            - Low Preset will block various data and email trackers that are common on the web.
              </p>
              
            </Row>
            <Row horizontal='space-between' style={{paddingBottom: 20}}>
              <p style={{fontSize: 30}}>
              <b style={{wordWrap: 'break-word'}}>
              Medium Preset: 
              </b>
              </p>
            </Row>
            <Row horizontal='space-between' style={{paddingBottom: 20}}>
            <p style={{wordWrap: 'break-word'}}>
            - Medium Preset will block game, marketing, and high risk IPs and proxies in addition to all of the low preset filters.
              </p>
             
            </Row>
            <Row horizontal='space-between' style={{paddingBottom: 20}}>
              <p style={{fontSize: 30}}>
              <b style={{wordWrap: 'break-word'}}>
              High Preset: 
              </b>
              </p>
            </Row>
            <Row horizontal='space-between' style={{paddingBottom: 20}}>
              <p style={{wordWrap: 'break-word'}}>
              - High Preset will block ads, clickbait, and trackers associated with social media (Facebook, Snapchat) in addition to all of the Medium preset filters. 
              </p>
                
            </Row>
            <Row horizontal='space-between' style={{paddingBottom: 20}}>
              <p style={{fontSize: 30}}>
              <b style={{wordWrap: 'break-word'}}>
              No Preset: 
              </b>
              </p>
            </Row>
            <Row horizontal='space-between' style={{paddingBottom: 20}}>
            <p style={{wordWrap: 'break-word'}}>
            - No Preset will remove the current preset you have selected. 
              </p>
             
            
            </Row>
          </Column>
        </Row>
        
        
      </div>
    );
  }


}

export default PresetComponent;
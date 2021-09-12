import React, { Component } from 'react';
import { Column, Row } from 'simple-flexbox';
import MiniCardComponent from 'components/cards/MiniCardComponent';
import Statuscomponent from "../statusComponent";
import './home.css';
class HelpComponent extends Component {
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
          <b style={{fontSize: 24}}>Additional information regarding the filters rules and firewall concepts.</b>
        </Row>
        <Row
          flexGrow={1}
          className="container"
          horizontal='left'
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
          <Column>
          <Row horizontal='space-between' style={{ paddingRight: 50, paddingBottom: 20 }}>
            <p style={{ fontSize: 30 }}>
              <b style={{ wordWrap: 'break-word' }}>
                What is an IP?
              </b>
            </p>
          </Row>
          <Row horizontal='space-between'style={{ paddingBottom: 20 }} >
            <p style={{ wordWrap: 'break-word' }}>
              An Internet Protocol address is a label assigned to each device connected to a network that uses the Internet Protocol for communication. Use this to block specific devices you do not want to communicate with your network.
              </p>
          </Row>
          <Row horizontal='space-between' style={{ paddingBottom: 20 }}>
              <p style={{ wordWrap: 'break-word' }}>
                For additional information or questions please see the wiki page located  <a href="https://en.wikipedia.org/wiki/IP_address">here</a>
              </p>
            </Row>
          </Column>
        </Row>
        <Row
          flexGrow={1}
          className="container"
          horizontal='left'
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
          <Column>
          <Row horizontal='space-between' style={{ paddingRight: 50, paddingBottom: 20 }}>
            <p style={{ fontSize: 30 }}>
              <b style={{ wordWrap: 'break-word' }}>
                What is DNS?
              </b>
            </p>
          </Row>
          <Row horizontal='space-between' style={{ paddingBottom: 20 }}>
            <p style={{ wordWrap: 'break-word' }}>
              DNS stands for Domain Name System and it provides a framework for web browsing. The DNS protocol allows us to access the internet with human-readable website names rather than remembering IP addresses. Enable this filter when you are trying to block a specific website.
              </p>

          </Row>
          <Row horizontal='space-between' style={{ paddingBottom: 20 }}>
              <p style={{ wordWrap: 'break-word' }}>
                For additional information or questions please see the wiki page located  <a href="https://en.wikipedia.org/wiki/Domain_Name_System">here</a>
              </p>
            </Row>
          </Column>
        </Row>
        <Row
          flexGrow={1}
          className="container"
          horizontal='left'
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
          <Column>
          <Row horizontal='space-between' style={{ paddingRight: 50, paddingBottom: 20 }}>
          <p style={{ fontSize: 30 }}>
                <b style={{ wordWrap: 'break-word' }}>
                  What is Advanced DNS?
              </b>
              </p>
            </Row>
            <Row horizontal='space-between' style={{ paddingBottom: 20 }}>
              <p style={{ wordWrap: 'break-word' }}>
              Modern browsers often encrypt this DNS information which limits our firewall's functionality. The advanced DNS filter helps combat this by resolving the domain's IP address first and filtering that instead. Use this when the DNS filter is not enough to block a website.
              </p>

            </Row>
            
            </Column>
        </Row>
        <Row
          flexGrow={1}
          className="container"
          horizontal='left'
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
          <Column>
          <Row horizontal='space-between' style={{ paddingRight: 50, paddingBottom: 20 }}>
          <p style={{ fontSize: 30 }}>
                <b style={{ wordWrap: 'break-word' }}>
                  What is TLS?
              </b>
              </p>
            </Row>
            <Row horizontal='space-between' style={{ paddingBottom: 20 }}>
              <p style={{ wordWrap: 'break-word' }}>
                TLS stands for Transport Layer Security which provides security for communications accross the web.  Enabling this will allow you to block larger websites ( ie. Reddit.com).
              </p>
            </Row>
            <Row horizontal='space-between' style={{ paddingBottom: 20 }}>
              <p style={{ wordWrap: 'break-word' }}>
                For additional information or questions please see the wiki page located  <a href="https://en.wikipedia.org/wiki/Transport_Layer_Security">here</a>
              </p>
            </Row>
            </Column>
        </Row>
        <Row
          flexGrow={1}
          className="container"
          horizontal='left'
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
          <Column>
          <Row horizontal='space-between' style={{ paddingRight: 50, paddingBottom: 20 }}>
          <p style={{ fontSize: 30 }}>
                <b style={{ wordWrap: 'break-word' }}>
                  What is a MAC Address?
              </b>
              </p>
            </Row>
            <Row horizontal='space-between' style={{ paddingBottom: 20 }}>
              <p style={{ wordWrap: 'break-word' }}>
                MAC stands for Media Access Control and a MAC address is used to uniquely identify devices on a network.
              </p>
            </Row>
            <Row horizontal='space-between' style={{ paddingBottom: 20 }}>
              <p style={{ wordWrap: 'break-word' }}>
                For additional information or questions please see the wiki page located  <a href="https://en.wikipedia.org/wiki/MAC_address">here</a>
              </p>
            </Row>
          </Column>
        </Row>
        <Row
          flexGrow={1}
          className="container"
          horizontal='left'
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
          <Column>
          <Row horizontal='space-between' style={{ paddingRight: 50, paddingBottom: 20 }}>
          <p style={{ fontSize: 30 }}>
                <b style={{ wordWrap: 'break-word' }}>
                  What is a Port?
              </b>
              </p>
            </Row>
            <Row horizontal='space-between' style={{ paddingBottom: 20 }}>
              <p style={{ wordWrap: 'break-word' }}>
                A port is a communication endpoint of a computer network.  Use Port filters when you want to block standard protocols like HTTP (port 80) or custom applications like Minecraft (ex 25565)
              </p>
            </Row>
            <Row horizontal='space-between' style={{ paddingBottom: 20 }}>
              <p style={{ wordWrap: 'break-word' }}>
                For additional information or questions please see the wiki page located  <a href="https://en.wikipedia.org/wiki/Port_(computer_networking)">here</a>
              </p>
            </Row>
          </Column>
        </Row>
        <Row
          flexGrow={1}
          className="container"
          horizontal='left'
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
          <Column>
          <Row horizontal='space-between' style={{ paddingRight: 50, paddingBottom: 20 }}>
          <p style={{ fontSize: 30 }}>
                <b style={{ wordWrap: 'break-word' }}>
                  What is a Protocol?
              </b>
              </p>
            </Row>
            <Row horizontal='space-between' style={{ paddingBottom: 20 }}>
              <p style={{ wordWrap: 'break-word' }}>
                A Protocol is a set of rules that controls how information is communicated accross devices in a network.
              </p>
            </Row>
            <Row horizontal='space-between' style={{ paddingBottom: 20 }}>
              <p style={{ wordWrap: 'break-word' }}>
                For additional information or questions please see the wiki page located  <a href="https://en.wikipedia.org/wiki/Internet_Protocol">here</a>
              </p>
            </Row>
          </Column>
        </Row>
        <Row
          flexGrow={1}
          className="container"
          horizontal='left'
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
          <Column>
          <Row horizontal='space-between' style={{ paddingRight: 50, paddingBottom: 20 }}>
          <p style={{ fontSize: 30 }}>
                <b style={{ wordWrap: 'break-word' }}>
                  What is a Smart Filter?
              </b>
              </p>
            </Row>
            <Row horizontal='space-between' style={{ paddingBottom: 20 }}>
              <p style={{ wordWrap: 'break-word' }}>
                A Smart Filter uses Virus total to analyze packets and blocks them if Virus total determines there is a security risk.
              </p>
            </Row>
            <Row horizontal='space-between' style={{ paddingBottom: 20 }}>
              <p style={{ wordWrap: 'break-word' }}>
                For additional information or questions please see the Virus Total website located <a href="https://www.virustotal.com/gui/">here</a>
              </p>
            </Row>
          </Column>
        </Row>
      </div>
    );
  }
}
export default HelpComponent;
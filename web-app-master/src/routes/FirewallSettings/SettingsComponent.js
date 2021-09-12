// Based on:
// SOURCE: https://www.cluemediator.com/add-or-remove-input-fields-dynamically-with-reactjs
// https://www.kirupa.com/react/simple_todo_app_react.htm
// Modified for NanoFirewall




import React, { Component } from "react";
import { Container } from "react-bootstrap";
import { Row,Column } from "simple-flexbox";
import UserSettingsComponent from "./UserSettingsComponent";
import EmailSettingsComponent from "./EmailSettingsComponent";
import FWSettingsComponent from "./FWSettingsComponent";
import Statuscomponent from "../statusComponent";
import MiniCardComponent from 'components/cards/MiniCardComponent';
import "routes/filter/Filter.css";
class SettingsComponent extends Component {
  constructor(props) {
    super(props);

    this.state = {

      status: "Not Connected"
    };
    this.ruleDesc = props.value;
  }
  

  render() {

    return (
      <Container fluid padding="10px">
        <div className="FilterListMain">
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
                        value='Administrator Settings'
                    />
                    <Statuscomponent/>
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
            horizontal='space-between'
            vertical='start'>
            <UserSettingsComponent />
            <EmailSettingsComponent />
            <FWSettingsComponent />
          </Row>
        </div>
      </Container>
    );
  }


}

export default SettingsComponent;
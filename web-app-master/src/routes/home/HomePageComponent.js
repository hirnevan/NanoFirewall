import React, { Component } from 'react';
import { Column, Row } from 'simple-flexbox';
import MiniCardComponent from 'components/cards/MiniCardComponent';
import Trends from './trends';
import './home.css';
class HomePageComponent extends Component {
    constructor(props) {
        super(props);
        this.state = {
            totalViolations: 0,
            totalPackets: 0,
            status: "Not Connected",
        };
        this.componentDidMount.bind(this);
    }
    async componentDidMount() {
        // Upon page load, we want to get the current status of the firewall
        var curStatus;
        await fetch('/status').then(function (res) {
            if(res.ok)
            {
                return res.text();
            }else
            {
                return "Not Connected";
            }
            
        }).then(function (text) {
            curStatus = text;
        });
        this.setState({status: curStatus});
        var sun =0;
        var mon =0;
        var tues =0;
        var wed =0;
        var thurs =0;
        var fri =0;
        var sat =0;
        var totalPackets = 0;
        await fetch('/statistics' )
        .then((res) => res.json())
        .then((json) => {
          sun = json["weeklyViolations"][0];
          mon = json["weeklyViolations"][1];
          tues = json["weeklyViolations"][2];
          wed = json["weeklyViolations"][3];
          thurs = json["weeklyViolations"][4];
          fri = json["weeklyViolations"][5];
          sat = json["weeklyViolations"][6];
          totalPackets = json["totalPackets"];  
        });
        var totalViolations = Math.round((sun+mon+tues+wed+thurs+fri+sat));
        this.setState({totalPackets: totalPackets});
        this.setState({totalViolations: totalViolations});
    }
    render() {
        //const classes = useStyles();
        return (<Column padding='30' >
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
                        title='Firewall Status'
                        value= {this.state.status}
                    />
                    <MiniCardComponent className='miniCardContainer'
                        title='Total Traffic (packets)'
                        value={this.state.totalPackets}
                    />
                    <MiniCardComponent
                        className='miniCardContainer'
                        title='Total Violations'
                        value={this.state.totalViolations}
                    />
                </Row>
            </Row>
            <div className='todayTrends'>
                <Trends   />
            </div>
        </Column>);
    }
}
export default HomePageComponent;
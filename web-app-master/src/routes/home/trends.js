import React, { Component } from 'react';
import { Column, Row } from 'simple-flexbox';
import './trends.css';
import LineChart from 'react-svg-line-chart';
class Trends extends Component {
    constructor(props) {
        super(props);
        this.state = {
            status: "Not Connected",
            avg: 0,
            data: [{ x: 0, y: 0},
                { x: 1, y: 0 },
                { x: 2, y: 0 },
                { x: 3, y: 0 },
                { x: 4, y: 0 },
                { x: 5, y: 0 },
                { x: 6, y: 0 },]
        };
      
     this.renderLegend = this.renderLegend.bind(this);
     this.renderStat = this.renderStat.bind(this);
    }
    async componentDidMount()
    {
        var sun =0;
        var mon =0;
        var tues =0;
        var wed =0;
        var thurs =0;
        var fri =0;
        var sat =0;
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
        });
        var avg = Math.round((sun+mon+tues+wed+thurs+fri+sat)/7);
        var data = [
            { x: 0, y: sun},
            { x: 1, y: mon },
            { x: 2, y: tues },
            { x: 3, y: wed },
            { x: 4, y: thurs },
            { x: 5, y: fri },
            { x: 6, y: sat },
        ]
        this.setState({data: data});
        this.setState({avg: avg});
    }
    renderLegend(color, title) {
        return (
            <Row vertical='center'>
                <div style={{ width: 16, border: '2px solid', borderColor: color }}></div>
                <span className="legendTitle">{title}</span>
            </Row>
        );
    }
    renderStat(title, value) {
        return (
            <Column
                flexGrow={1}
                className='statContainer'
                vertical='center'
                horizontal='center'
            >
                <span className="statTitle">{title}</span>
                <span className="statValue">{value}</span>
            </Column>
        );
    }
    render()
    {
        return(<Row
            flexGrow={1}
            className='coolContainer'
            horizontal='center'
            breakpoints={{ 1024: 'column' }}
        >
            <Column
                wrap
                flexGrow={7}
                flexBasis='735px'
                className='graphSection'
                breakpoints={{ 1024: { width: 'calc(100% - 48px)', flexBasis: 'auto' } }}
            >
                <Row wrap horizontal='space-between'>
                    <Column>
                        <span className='graphTitle'>7 Day Violation Trends</span>
                        
                    </Column>
                    <Column>
                        {this.renderStat('Average Daily Violations', this.state.avg)}
                    </Column>
                    <Column>
                        {this.renderLegend('#3751FF', 'Number of Violations')}
                    </Column>
                </Row>
                <Row wrap horizontal='space-between'>

                    <div />
                </Row>
                <div className='graphContainer'>
                    
                    <LineChart
                        data={this.state.data}
                        pointsStrokeColor='#3751FF'
                        areaColor='#34495e'
                        areaVisible={true}
                        labelsHeightX={20}
                        labelsOffsetX={20}
                        labelsCharacterWidth={15}
                        labelsFormatX={v => {
                            switch (v) {
                                case 0:
                                    return 'Sun'
                                case 1:
                                    return 'Mon'
                                case 2:
                                    return 'Tues'
                                case 3:
                                    return 'Wed'
                                case 4:
                                    return 'Thurs'
                                case 5:
                                    return 'Fri'
                                case 6:
                                    return 'Sat'
                                default:
                                    return 'Invalid'
                            }
                        }}
                        labelsStepX={1}
                    />
                </div>
                <div className='separator'></div>
            </Column>
        </Row>);
    }

}

export default Trends;
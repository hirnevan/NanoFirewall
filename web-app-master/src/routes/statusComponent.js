
import React, { Component } from "react";
import MiniCardComponent from 'components/cards/MiniCardComponent';
class Statuscomponent extends Component {
    constructor(props) {
        super(props);
        this.state = {
            status: "Not Connected"
        };
        this.refreshStatus = this.refreshStatus.bind(this);
    }
    componentWillUnmount() {
        window.clearInterval(this.timer);
    }
    async componentDidMount() {
        // Upon page load, we need to see if it is enabled
        this.refreshStatus();
        this.timer = setInterval(this.refreshStatus, 10000);
    }
    async refreshStatus() {
        var curStatus;
        await fetch('/status').then(function (res) {
            if (res.ok) {
                return res.text();
            } else {
                return "Not Connected";
            }
        }).then(function (text) {
            curStatus = text;
        });
        this.setState({ status: curStatus });
    }
    render() {
        return (<MiniCardComponent className='miniCardContainer'
            title='Firewall Status'
            value={this.state.status}
        />);
    }
}
export default Statuscomponent;
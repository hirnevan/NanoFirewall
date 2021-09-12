import "./Filter.css";

import React, { Component } from "react";
import Filteritem from './Filteritem';
import { Column } from "simple-flexbox";
import {TimePicker } from 'antd';
import 'antd/dist/antd.css';
import moment from "moment";

// This page is a custom rule component that allows users to add specific times to allow or block traffic for devices.
class TimeFilterComponent extends Component {
  constructor(props) {

    super(props);
    this.state = {
      startTime: '',
      endTime: '',
      items: [],
      blocklist: "true",
    };
    this.addItem = this.addItem.bind(this);
    this.deleteItem = this.deleteItem.bind(this);
    this.block = this.block.bind(this);
    this.onStartChange = this.onStartChange.bind(this);
    this.onEndChange = this.onEndChange.bind(this);
    this.ruleId = props.id;

  }
  
  async componentDidMount() {
    // Upon page load, we want to fill up list with the current devices.

    var filters;
    await fetch('/rule?name=time')
      .then((res) => res.json())
      .then((json) => {
        filters = json["rule"]["filterList"];
        this.setState({
          blocklist: json["rule"]["filterBlockList"].toString()
        });
      });
    for (let f of filters) {
      this.loaditems(f);
    }
  }
  onStartChange(time) {
    var timeStr = time;
    this.setState({ startTime: timeStr });
    console.log("START TIME: " + this.state.startTime);
    //const { name, value } = e.target;
    //this.setState({ [name]: value });
  }
  onEndChange( time) {
    var timeStr = time;
    this.setState({ endTime: timeStr });
    console.log("END TIME: " + this.state.endTime);
    //const { name, value } = e.target;
    //this.setState({ [name]: value });
  }
  loaditems(e) {
    if (e !== "") {
      var newItem = {
        text: e,
        key: Date.now()
      };
      e = "";
      this.setState((prevState) => {
        return {
          items: prevState.items.concat(newItem)
        };
      })
    }
  }

  addItem(e) {
    e.preventDefault();
    if (this.startTime !== "" && this.endTime !=="")
    {
      var newItem = {
        text: this.state.startTime + " " + this.state.endTime,
        key: Date.now()
      };
    }

    console.log("TIME: " + this.state.startTime + " " + this.state.endTime);
    if (this.startTime !== "" && this.endTime !=="") {
 
      fetch('/rule', {
        method: 'PATCH',
        headers: {
          'Content-Type': 'application/json',
        },
        body: JSON.stringify({
          rule: {
            action: 'add',
            filterList: [newItem.text],
            name: this.ruleId
          }
        })
      })
      
      this.setState((prevState) => {
        return {
          items: prevState.items.concat(newItem)
        };
      })
    }
    
  }
  deleteItem(key) {

    var item_being_removed = this.state.items.find(item => {
      return item.key === key
    });
    fetch('/rule', {
      method: 'PATCH',
      headers: {
        'Content-Type': 'application/json',
      },
      body: JSON.stringify({
        rule: {
          action: 'del',
          name: this.ruleId,
          filterList: [item_being_removed.text]
        }
      })
    })
    var filteredItems = this.state.items.filter(function (item) {
      return (item.key !== key);
    });

    this.setState({
      items: filteredItems
    });
  }
  block(e) {
    console.log(e.target.value)
    fetch('/rule', {
      method: 'PATCH',
      headers: {
        'Content-Type': 'application/json',
      },
      body: JSON.stringify({
        rule: {
          name: this.ruleId,
          filterBlockList: (e.target.value === "true"),
        }
      })
    })
    this.setState({
      blocklist: e.target.value
    });
  }

  render() {

    const format = 'HH:mm';
    return (
      <div className="DeviceList" alignContent="center">
        <Column style={{
          backgroundColor: '#FFFFFF',
          border: `1px solid #DFE0EB`,
          borderRadius: 4,
          cursor: 'pointer',
          width: '100%',
          padding: '16px 32px 16px 32px'
        }}>
          <div className="header" >
            <p style={{ fontSize: 24 }}>Set a Time</p>
          </div>

          <div className="body" style={{ paddingTop: 10 }}>
            <form onSubmit={this.addItem}>
              <TimePicker format={format} name="startTime" style={{ width: '50%' }} placeholder="Start time" onSelect={(value) => {
                const timeString = moment(value).format("HH:mm");
                this.onStartChange(timeString)
              }}></TimePicker>
              <TimePicker format={format} name="endTime" style={{ width: '50%' }} placeholder="End time" onSelect={(value) => {
                const timeString = moment(value).format("HH:mm");
                this.onEndChange(timeString)}}></TimePicker>
              <button type="submit">Add</button>
            </form>

            <div className="column" style={{ alignContent: "center", paddingTop: 10 }}>

              <select className="custom-select" style={{ width: 'auto', alignContent: "center" }} name="block_all" id="block_all" onChange={this.block} value={this.state.blocklist} >
                <option id='block' value="true" >Disable during these times</option>
                <option id='no_block' value="false" >Enable during these times</option>
              </select>
            </div>
          </div>
          <Filteritem entries={this.state.items} delete={this.deleteItem} />
        </Column>
      </div>
    );
  }
}

export default TimeFilterComponent;
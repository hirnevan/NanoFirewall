// Based on:
// SOURCE: https://www.cluemediator.com/add-or-remove-input-fields-dynamically-with-reactjs
// https://www.kirupa.com/react/simple_todo_app_react.htm
// Modified for NanoFirewall


import "./Filter.css";

import React, { Component } from "react";
import Filteritem from './Filteritem';
import { Column } from "simple-flexbox";

// This component displays all the devices belong a given filter, 
// allowing users to add and delete devices from/to this list 
class DeviceListComponent extends Component {
  constructor(props) {
    super(props);
    this.state = {
      items: [],
      blocklist: "true",
    };
    this.addItem = this.addItem.bind(this);
    this.deleteItem = this.deleteItem.bind(this);
    this.block = this.block.bind(this);
    this.ruleId = props.id;
  }

  async componentDidMount() {
    // Upon page load, we want to fill up list with the current devices.

    var filters;
    await fetch('/rule?name=' + this.ruleId)
      .then((res) => res.json())
      .then((json) => {
        filters = json["rule"]["deviceList"];
        this.setState({
          blocklist: json["rule"]["deviceBlockList"].toString()
      });
      });
    for (let f of filters) {
      this.loaditems(f);
    }
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
    
    if (this._inputElement.value !== "") {
      var newItem = {
        text: this._inputElement.value,
        key: Date.now()
      };
      fetch('/rule', {
        method: 'PATCH',
        headers: {
          'Content-Type': 'application/json',
        },
        body: JSON.stringify({
          rule: {
            action: 'add',
            deviceList: [newItem.text],
            name: this.ruleId
          }
        })
      })
      this._inputElement.value = "";
      this.setState((prevState) => {
        return {
          items: prevState.items.concat(newItem)
        };
      })
    }

    e.preventDefault();
  }

  deleteItem(key) {
    // Delete item is TODO
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
          deviceList: [item_being_removed.text]
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
block(e)
{
  console.log(e.target.value)
    fetch('/rule', {
      method: 'PATCH',
      headers: {
        'Content-Type': 'application/json',
      },
      body: JSON.stringify({
        rule: {
          name: this.ruleId,
          deviceBlockList: (e.target.value === "true") ,
        }
      })
    })
    this.setState({
      blocklist: e.target.value
  });
}

  render() {
    return (
      <div className="DeviceList">
        
        <Column style={{
          backgroundColor: '#FFFFFF',
          border: `1px solid #DFE0EB`,
          borderRadius: 4,
          cursor: 'pointer',
          maxWidth: 350,
          padding: '16px 32px 16px 32px'
        }}>
          <div className="header">
          <div>
          <div className="column">
          <select className="custom-select" style ={{ width: '200px' }}  name="block_all" id="block_all" onChange={this.block} value ={this.state.blocklist} >
            <option id='block' value="true" >Apply to devices in this list</option>
            <option id='no_block' value="false" >Apply to devices not on this list</option>
          </select> 
          </div>
        </div>
            <form onSubmit={this.addItem}>
              <input style={{ backgroundColor: '#F7F8FC' }} ref={(a) => this._inputElement = a}
                placeholder="Add Device">
              </input>
              <button type="submit">Add</button>
            </form>
          </div>
          <Filteritem entries={this.state.items}
            delete={this.deleteItem} />
        </Column>
      </div>
    );
  }
}

export default DeviceListComponent;
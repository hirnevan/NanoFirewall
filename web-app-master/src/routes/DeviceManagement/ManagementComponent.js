import React, { Component } from "react";
import Statuscomponent from "../statusComponent";
import { Row, Column} from "simple-flexbox";
import MiniCardComponent from 'components/cards/MiniCardComponent';
import "routes/filter/Filter.css";
// template from: https://www.pluralsight.com/guides/creating-dynamic-editable-tables-with-reactjs

// ABOUT: This component displays an editable table that enables users to set data limits and change the nick names of devices.
class ManageMentComponent extends Component {
  constructor(props) {
    super(props);

    this.state = {
      message: "",
      items: [],
      datalimits: []
    }
    this.ruleDesc = props.value;
  }
  
  async componentDidMount() {
    // Upon page load, we want to fill up list with the current devices.
    
    var nicknames;
    await fetch('/nickname')
      .then((res) => res.json())
      .then((json) => {
        nicknames = json["nickname"];
      });
    // for (var key in nicknames) {
      
    //   this.loadNicknames(nicknames[key][1],key, nicknames[key][0]);
    // }
    nicknames.forEach(tup => this.loadNicknames(tup[0],tup[1],tup[2]));
    var filters;
    await fetch('/rule?name=data')
      .then((res) => res.json())
      .then((json) => {
        filters = json["rule"]["filterList"];
      });
    for (let f of filters) {
      this.loadDataLimits(f);
    }
  }
  
  
  loadDataLimits(str)
  {

    var items = this.state.items;
    var split = str.split(" ");
    var mac = split[0];
    
    var dl = split[1];

    for(var i =0; i< items.length; i++)
    {
      if(items[i].text === mac)
      {
        items[i].dataLimit = dl;
        
      }
      
    }

    this.setState({
      items: items
    });
  }
  loadNicknames(nickname,mac,usage) {
    if (mac !== "") {
      var newItem = {
        text: mac,
        nickName: nickname,
        dataUsage: usage,
        dataLimit: "",
        selection: "",
        key: Date.now()
      };
      mac = "";
      this.setState((prevState) => {
        return {
          items: prevState.items.concat(newItem)
        };
      })
    }
  }
  updateMessage(event) {
    this.setState({
      message: event.target.value
    });
  }

  handleClick() {
    var items = this.state.items;

    items.push(this.state.message);

    this.setState({
      items: items,
      message: ""
    });
  }

  handleNameChanged(i, event) {
    var items = this.state.items;
    items[i].nickName = event.target.value;
    this.setState({
      items: items
    });

    
  }
  handleSelection(i, event)
  {
    var items = this.state.items;
    items[i].dataLimit = event.target.value;
    this.setState({
      items: items
    });
  
  }

  async handleItemUpdate(i, event) {
    var items = this.state.items;
    
    await fetch('/nickname', {
      method: 'PATCH',
      headers: {
        'Content-Type': 'application/json',
      },
      body: JSON.stringify({
         name:items[i].nickName ,
         value: items[i].text,
         action: "add"
      })
    })
    var act;
    if (items[i].dataLimit === "")
    {
      
         await fetch('/rule', {
          method: 'PATCH',
          headers: {
            'Content-Type': 'application/json',
          },
          body: JSON.stringify({
            rule: 
            {
            name: "data",
            filterList: [items[i].text],
            action: "del",
            enabled: true
            }
             
          })
        })
    }
    else
    {
      act = "add";
      await fetch('/rule', {
        method: 'PATCH',
        headers: {
          'Content-Type': 'application/json',
        },
        body: JSON.stringify({
          rule: 
          {
          name: "data",
          filterList: [items[i].text+" "+ items[i].dataLimit],
          action: "add",
          enabled: true
          }
           
        })
      })
    }
    

  }
  
 
  renderRows() {
    var context = this;

    return this.state.items.map(function (o, i) {
      return (
        <tr key={"item-" + i}>
          <td style={{ padding: 10 }}>
            {o.text}
          </td>
          <td style={{ padding: 10 }}>
            <input
              type="text"
              value={o.nickName}
              onChange={context.handleNameChanged.bind(context, i)}
            />
          </td>
          <td>
            {o.dataUsage}
          </td>
          <td style={{ padding: 10 }}>
            <select className="custom-select" style={{ width: '200px' }} name="block_all" id="block_all" onChange={context.handleSelection.bind(context, i)} value = {o.dataLimit}>
              <option id='none' value="" >None</option>
              <option id='10485760' value="10485760" >10 MB</option>
              <option id='104857600' value="104857600" >100 MB</option>
              <option id='524288000' value="524288000" >500 MB</option>
              <option id='1073741824' value="1073741824" >1 GB</option>
              <option id='5368709120' value="5368709120" >5 GB</option>
              <option id='10737418240' value="10737418240" >10 GB</option>
            </select>
          </td>
          <td>
            <button onClick={context.handleItemUpdate.bind(context, i)}>
              Update
            </button>
          </td>
        </tr>
      );
    });
  }

  render() {
    return (
      <div>
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
                        value= "Manage Devices"
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
        <Row>
        <table className="">
          <thead>
            <tr>
              <th style={{ padding: 10 }}>
                Devices Found on Network
              </th>
              <th style={{ padding: 10 }}>
                Nickname
              </th>
              <th style={{ padding: 10 }}>
                Data Usage (MB)
              </th>
              <th style={{ padding: 10 }}>
                Data Limit
              </th>
            </tr>
          </thead>
          <tbody>
            {this.renderRows()}
          </tbody>
        </table>
    
        </Row>
        </div>
    );
  }
}
export default ManageMentComponent;
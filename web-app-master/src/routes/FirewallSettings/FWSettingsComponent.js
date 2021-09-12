import React, { Component} from "react";
import { Column } from "simple-flexbox";
import styles from "./Settings.module.css";
import {  Power, RotateCw } from 'react-feather';
import Switch from "react-switch";
class FWSettingsComponent extends Component
{
    constructor(props) {
        super(props);

        this.state = {
            status: '',
            checked: false,
            icon: Power,
            http: false


        };
        this.handleHTTP = this.handleHTTP.bind(this);
        this.decideIcon = this.decideIcon.bind(this);
        this.handleChange = this.handleChange.bind(this);
        this.handleRestart = this.handleRestart.bind(this);
    }
    async componentDidMount() {
      // Upon page load, we need to see if it is enabled
      var curStatus;
      var check;
      var http;
      await fetch('/settings')
            .then((res) => res.json())
            .then((json) => {
                http = json["httpEnabled"];
                this.setState({ http });
            })
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
            console.log(text);
            if(text === "Running")
            {
              check = true;
            }else
            {
              check = false;
            }
        });
        this.setState({status: curStatus});
        this.setState({checked: check});
        
    }
    handleHTTP(http)
    {
      this.setState({ http: http });
      fetch('/settings', {
        method: 'PATCH',
        headers: {
            'Content-Type': 'application/json',
        },
        body: JSON.stringify({
            httpEnabled: http
            
        })
    });
    }
    handleRestart()
    {
      fetch('/settings', {
        method: 'PATCH',
        headers: {
            'Content-Type': 'application/json',
        },
        body: JSON.stringify({
            action: 'restart'
            
        })
    });
    }
    updateFw(fwstate)
    {
      fetch('/settings', {
        method: 'PATCH',
        headers: {
            'Content-Type': 'application/json',
        },
        body: JSON.stringify({
            action: fwstate
            
        })
    });
    }
    handleChange(e)
    {
      var checked = !this.state.checked;
      var fwState; 
      if(this.state.checked)
      {
        fwState = 'disable';
      }
      else
      {
        fwState = 'enable';
      }
      
    this.setState({checked});
    this.updateFw(fwState);
    }
    decideIcon()
    {

      if(this.state.checked)
      {


        return(
          <button onClick={this.handleChange} width="25px" height="25px" alignContent="center" >
              <Power color="Green"/>
          </button>
          
        );
      }else
      {
       // console.log("deciding false");
        return(
          <button onClick={this.handleChange} width="25px" height="25px" alignContent="center" >
              <Power />
          </button>
        );
      }
    }
    render() {
      
      return (
        
            <Column className={styles.col}>
 
                <table>
                  <tr>
                 
                  <td>
                  Turn on/off Firewall
                  </td>
                  <td>
                  
                  </td>
                  <td>
                    {this.decideIcon()}
                  </td>
                  </tr>
                  <tr>
                    Restart Firewall
                  <td>
                  </td>
                  <td>
                  <button className={styles.button} onClick={this.handleRestart} width="25px" height="25px" alignContent="center" >
                    <RotateCw/>
                  </button>
                  </td>
                  </tr>
                  <tr>
                    
                  <td>
                  <p style={{paddingRight: 20}}>Enable/Disable Blocked Web Page </p>
                  <Switch
                    name="filter"
                    onChange={this.handleHTTP}
                    checked={this.state.http}
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
                  </td>
                  </tr>

                </table>
            </Column>
        
      );
    }
}
export default FWSettingsComponent;
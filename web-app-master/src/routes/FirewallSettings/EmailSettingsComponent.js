import React, { Component } from "react";
import { Column, Row } from "simple-flexbox";
import styles from "./Settings.module.css";
// This component represents the email settngs of the administrator allowing the user to choose a time in hours for 
// report frequency
class UserSettingsComponent extends Component {
    constructor(props) {
        super(props);

        this.state = {
            email: '',

            emailSubmitted: false,
            hourFrequencySubmitted: false,
            selectedValue: 6,
        };

        this.handleChange = this.handleChange.bind(this);
        this.handleEmailUpdate = this.handleEmailUpdate.bind(this);
        this.handleTimeChange = this.handleTimeChange.bind(this);
    }

    async componentDidMount() {
        // Upon page load, we need to see if it is enabled
        var email;
        var hourFreqValue;

        await fetch('/settings')
            .then((res) => res.json())
            .then((json) => {
                email = json["storedEmail"];
                this.setState({ email });
                hourFreqValue = json["minEmailTime"];
                console.log("value in: " + hourFreqValue);
            })
            var selectedValue = (parseInt(hourFreqValue)/60)/60;

            this.setState({selectedValue});
        console.log("email: " + email + '\n Selected Value adjusted: '  + this.state.selectedValue);
    }
    // HANDLES USER INPUT
    handleChange(e) {
        //TODO
        const { name, value } = e.target;
        this.setState({ [name]: value });
    }
    // HANDLES THE UPDATING OF USER SETTINGS
    handleEmailUpdate() {
        this.setState({ emailSubmitted: true });
        const { email } = this.state;
  
        console.log('sending email' + email);
        // if the current login information is correct:
        // checking to see if the email matches the confirm email
        fetch('/settings', {
            method: 'PATCH',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify({
                storedEmail: this.state.email
                
            })
        });




    }
    // Handles the updating of email report frequency
    handleTimeChange() {

        //this.setState({selectedValue})
        var adjusted = (this.state.selectedValue*60)*60;
        console.log('sending time: ' + adjusted);
        
        fetch('/settings', {
            method: 'PATCH',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify({
               
                minEmailTime: adjusted
                
            })
        })
        // TO DO
    }

    render() {
        const { email, confirmEmail, hourFrequencySubmitted, emailSubmitted, selectedValue } = this.state;

        return (
            <Column className={styles.col}>
                <div className="col-md-6 col-md-offset-3">
                    <Row className={styles.row}>
                        <h2>Email Settings</h2>
                    </Row>
                    <form name="form" >
                        <div className={'form-group' + (emailSubmitted && !email ? ' has-error' : '')}>
                            <Row className={styles.row}>
                                <label htmlFor="email">Email to send reports to:</label>
                            </Row>

                            <Row className={styles.row}>
                                <input type="text" className="form-control" name="email" value={email} onChange={this.handleChange} />

                            </Row>
                            <Row>
                                {emailSubmitted && !email &&
                                    <div className={styles.help}>*Email is required</div>
                                }
                            </Row>
                        </div>
                        <div className={'form-group' + (emailSubmitted && !confirmEmail ? ' has-error' : '')}>
                            <Row className={styles.row}>
                                <label htmlFor="newUsername">Confirm Email</label>
                            </Row>

                            <Row className={styles.row}>
                                <input type="text" className="form-control" name="confirmEmail" value={confirmEmail} onChange={this.handleChange} />
                            </Row>
                            <Row>
                                {emailSubmitted && !confirmEmail | email !== confirmEmail &&
                                    <div className={styles.help}>*Emails must match!</div>
                                }
                            </Row>
                        </div>
                        <Row className={styles.row}>
                            <div className="form-group">
                                <button onClick={this.handleEmailUpdate} type="button"className={styles.customButton}>Update</button>
                            </div>
                        </Row>

                    </form>
                    <form name="form" >
                        <div className={'form-group' + (hourFrequencySubmitted && !selectedValue ? ' has-error' : '')}>
                            <Row className={styles.row}>
                                <label htmlFor="timepicker">How often would you like to be notified? </label>
                            </Row>

                            <Row className={styles.row}>
                            <input type="text" className="form-control" name="selectedValue" value={selectedValue} onChange={this.handleChange} />

                            </Row>

                        </div>
                        <Row className={styles.row}>
                            <div className="form-group">
                                <button type="button" onClick={this.handleTimeChange} className={styles.customButton}>Update</button>
                            </div>
                        </Row>

                    </form>
                </div>
            </Column>
        );
    }
}


export default UserSettingsComponent;
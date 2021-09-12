import React from 'react';
import { Component } from "react";
import { Column, Row } from "simple-flexbox";
import styles from "./Settings.module.css";
import { encode as base64_encode } from 'base-64';
class UserSettingsComponent extends Component {
    constructor(props) {
        super(props);

        this.state = {
            newUsername: '',
            newPassword: '',
            submitted: false,
            submittedKey: false,
            apiKey: '',
        };

        this.handleChange = this.handleChange.bind(this);
        this.handleUpdate = this.handleUpdate.bind(this);
        this.handleKeyUpdate = this.handleKeyUpdate.bind(this);
    }
    async componentDidMount() {
        // Upon page load, we need to see if it is enabled
        var apiKey;
        await fetch('/settings')
            .then((res) => res.json())
            .then((json) => {
                apiKey = json["vt_api_key"];
                this.setState({ apiKey });
            })
           
    }
    // HANDLES USER INPUT
    handleChange(e) {
        console.log("TARGET: " + e.target.name);
        const { name, value } = e.target;
        this.setState({ [name]: value });
    }
    // HANDLES THE UPDATING OF USER SETTINGS
    handleUpdate(e) {
        e.preventDefault();

        this.setState({ submitted: true });
        const { newUsername, newPassword } = this.state;
        // if the current login information is correct:
        var authString = newUsername + ':' + newPassword;
        let enc = base64_encode(authString);
        fetch('/settings', {
            method: 'PATCH',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify({

                auth: enc

            })
        })
    }
    // Handles API Key Update
    handleKeyUpdate(e) {
        e.preventDefault();

        this.setState({ submittedKey: true });
        const { apiKey} = this.state;
        // if the curren is correct:
        fetch('/settings', {
            method: 'PATCH',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify({

                vt_api_key: apiKey

            })
        })
    }

    render() {
        const { newUsername, newPassword, submitted, submittedKey, apiKey } = this.state;
        return (
            <Column className={styles.col}>
                <div className="col-md-6 col-md-offset-3">
                    <Row className={styles.row}>
                        <h2>Change Login Information</h2>
                    </Row>
                    <form name="form" onSubmit={this.handleUpdate}>

                        <div className={'form-group' + (submitted && !newUsername ? ' has-error' : '')}>
                            <Row className={styles.row}>
                                <label htmlFor="newUsername">New Username</label>
                            </Row>
                            <Row className={styles.row}>
                                <input type="text" className="form-control" name="newUsername" value={newUsername} onChange={this.handleChange} />

                            </Row>
                            <Row>
                                {submitted && !newUsername &&
                                    <div className={styles.help}>*New Username is required</div>
                                }
                            </Row>

                        </div>

                        <div className={'form-group' + (submitted && !newPassword ? ' has-error' : '')}>
                            <Row className={styles.row}>
                                <label htmlFor="newPassword">New Password</label>
                            </Row>
                            <Row className={styles.row}>
                                <input type="newPassword" className="form-control" name="newPassword" value={newPassword} onChange={this.handleChange} />
                            </Row>
                            <Row >
                                {submitted && !newPassword &&
                                    <div className={styles.help}>*New Password is required</div>
                                }
                            </Row>
                        </div>
                        <div className="form-group" alignContent="center">
                            <Row className={styles.row} alignSelf="center">
                                <button className={styles.customButton} >Update</button>
                            </Row>
                        </div>
                    </form>

                </div>
                <form name="form" onSubmit={this.handleKeyUpdate}>
                    <div className={'form' + (submittedKey && !apiKey ? ' has-error' : '')}>
                        <Row className={styles.row}>
                            <label htmlFor="apiKey">Virus Total API Key</label>
                        </Row>
                        <Row className={styles.row}>
                                <input type="apiKey" className="form-control" name="apiKey" value={apiKey} onChange={this.handleChange} />
                            </Row>
                        <Row >
                            {submitted && !apiKey &&
                                <div className={styles.help}>*API Key required</div>
                            }
                        </Row>
                    </div>
                    <div className="form-group" alignContent="center">
                        <Row className={styles.row} alignSelf="center">
                            <button className={styles.customButton} >Update</button>
                        </Row>
                    </div>

                </form>

            </Column>
        );
    }
}

export default UserSettingsComponent;
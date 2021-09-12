import React, { Component } from "react";
import FlipMove from "react-flip-move";
// This component represents either a device in a device list or a filter in a filter list
class Filteritem extends Component {
  constructor(props) {
    super(props);
 
    this.createTasks = this.createTasks.bind(this);
  }
  createTasks(item) {
    return <li onClick={() => this.delete(item.key)} 
    key={item.key}>{item.text}</li> 
  }
  delete(key) {
    this.props.delete(key);
  }
  render() {
    var todoEntries = this.props.entries;
    var listItems = todoEntries.map(this.createTasks);
 
    return (
      <ul className="theList">
         <FlipMove duration={250} easing="ease-out">
          {listItems}
         </FlipMove>
      </ul>
    );
  }
};
 
export default Filteritem;
import React, { Component } from 'react';
// Game subcomponents
import { Card } from "../";

class HandCards extends Component {
  render() {
    // Extract data and event functions from props
    const { className, cards } = this.props;
    let _cards = cards;
    if(className == "ai"){
      if(_cards.length == 1){
        _cards.push[53];
        _cards.push[0];
        _cards.push[0];
      }else if(_cards.length == 2){
        _cards.push[0];
        _cards.push[0];
      }
    }else if(className == "player"){
      if(_cards.length == 2){
        _cards.push[0];
        _cards.push[0];
      }
    }
    // function in render for generating `Card`s
    const generateCards = _cards => {
      let elems = [];
      // Hard coded 4 as the max of loop as every user has 4 hand cards
      for (let i = 0; i < 4; ++i) {
        // Prepare properties for `Card`
        let cardProperties = {
          key: i,
          cardId: _cards[i]
        };
        
        // Put the `Card` to `elems` array
        elems.push(<Card { ...cardProperties } />);
      }
      return elems;
    };

    return (
      <div className={`HandCards${ className ? ' ' + className : '' }`}>
        { generateCards(cards) }
      </div>
    )
  }
}

export default HandCards;

import React, { Component } from 'react';
// Components
import { Button } from 'components';
// Game subcomponents
import { RulesModal } from './components';

class Info extends Component {
  render() {
    // Extract data and event functions from props
    const { className, deckCardCount, handCardCount, onEndGame,turn } = this.props;
    // Display:
    // Round number: 18 <-- ((max deck = 17) + 1) - Deck Cards - Hand Cards
    // Rules button to trigger a modal
    // Button to end the current game
    return (
      <div className={`Info${ className ? ' ' + className : '' }`}>
      
        { turn == 0 && <p>ROUND: <span className="round-number">YourTurn</span></p> }
        { turn == 1 && <p>ROUND: <span className="round-number">DEALERTurn</span></p> }
        <RulesModal />
  
      </div>
    )
  }
}

export default Info;

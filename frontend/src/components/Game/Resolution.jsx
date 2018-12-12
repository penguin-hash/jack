import React, { Component } from 'react';
// Components
import { Button } from 'components';
// Game subcomponents
import { Card } from '../';

class Resolution extends Component {
  render() {
    // Extract data and event functions from props
    const {  aiName,  playerName, status, onEndGame } = this.props;

    // Flag to indicate if the resolution screen should be shown
    // By checking if aiSelectCard is not empty
    //const isCardSelected = aiCard > 0;

    // Store the result of each round and decide where to put the "WIN" or "DRAW"
    let aiRoundResult = "";
    let playerRoundResult = "";
    if (aiLost === 0 && playerLost === 0) {
      aiRoundResult = "DRAW";
      playerRoundResult = "DRAW";
    } else if (aiLost === 0) {
      aiRoundResult = "WIN";
      playerRoundResult = <span>- { playerLost }</span>;
    } else {
      aiRoundResult = <span>- { aiLost }</span>;
      playerRoundResult = "WIN";
    }

    // If state === 1, display "YOU WIN!" (player wins)
    // If state === -1, display "YOU LOST!" (player loses)
    // And display: 1 div for left selected card
    //              1 div for showing VS text
    //              1 div for right selected card
    // And put the buttons for next round or end game
    return (
      <div className={`Resolution${ isCardSelected ? " card-selected" : "" }`}>
        {status !== 0 && <div>
          { status === 1 && <div className="result win">VICTORY</div> }
          { status === 2 && <div className="result lost">DEFEATED</div> }
          { status === 3 && <div className="result lost">DEFEATED</div> }
          { status === 4 && <div className="result win">Blackjack VICTORY !!</div> }
          <div className="player">
            <p className="round-result">{player_point}</p>
            
            <p className="name">{ playerName }</p>
          </div>
          <div className="vs">{ "VS" }</div>
          <div className="ai">
            <p className="round-result">{player_point}</p>
            <Card cardId={ aiCard } />
            <p className="name">{ aiName }</p>
          </div>
          <div className="buttons">
            { isCardSelected && status !== 0 &&
                <Button onClick={ onEndGame } className="red">QUIT</Button> }
          </div>
        </div>}
      </div>
    )
  }
}

export default Resolution;

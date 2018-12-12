import React, { Component } from 'react';
// Game subcomponents
import { HandCards, PlayerInfo } from '../';
import { Button } from 'components';

class GameMat extends Component {
  render() {
    // Extract data and event functions from props
    const { className, deckCardCount,
            aiLife, aiHandCards, aiName,
            playerLife, playerHandCards, playerName,
            onPlayCard,turn , changeturn} = this.props;

    // Display the GameMat as a table with 2 rows
    // The 1st row is AI (`PlayerInfo`, Deck card, `HandCards`)
    // The 2nd row is Player (`PlayerInfo`, Deck card, `HandCards`)
    return (
      <table className={`GameMat${ className ? ' ' + className : '' }`}>
        <tbody>
          <tr>
            <td className="mat mat-ai">
              <PlayerInfo
                className="ai"
                name={ aiName }
                life={ aiLife }
              />
              <div className={`deck remain${deckCardCount}`}>
                { aiName }'S Deck ({ deckCardCount })
              </div>
              <HandCards
                className="ai"
                cards={ aiHandCards }
              />
            </td>
          </tr>
          <tr>
            <td className="mat mat-player">
              <PlayerInfo
                className="player"
                name={ playerName }
                life={ playerLife }
              />
              
              <HandCards
                className="player"
                cards={ playerHandCards }
              />
            </td>
          </tr>
           
            { turn == 0 && <tr>
              <td><Button onClick={ onPlayCard } className="small red">Draw Card</Button></td>

              <td><Button onClick={ changeturn } className="small red">changeturn</Button></td>

              
            </tr>}
        </tbody>
      </table>
          )
    }
}

export default GameMat;

import React, { Component } from 'react';

// Card dictionary from smart contract
// { [cardType, cardPower], ... }
const cardDict = [
  [0, 0], // empty card (for the case the card is played or empty selected card)
  [1, 1], [1, 1],[1, 1], [1, 1],
  [2, 2], [2, 2],[2, 2], [2, 2],
  [3, 3],[3, 3],[3, 3],[3, 3],
  [4, 4], [4, 4],[4, 4],[4, 4],
  [5, 5], [5, 5],[5, 5],[5, 5],
  [6, 6], [6, 6], [6, 6], [6, 6],
  [7, 7], [7, 7],[7, 7],[7, 7],
  [8, 8], [8, 8],[8, 8], [8, 8],
  [9, 9],[9, 9],[9, 9],[9, 9],
  [10, 10], [10, 10], [10, 10], [10, 10],
  [11, 10],[11, 10],[11, 10],[11, 10],
  [12, 10],[12, 10],[12, 10],[12, 10],
  [13, 10],[13, 10],[13, 10],[13, 10],
  [14,0]
];

class Card extends Component {
  render() {
    // Extract card index (cardId) and onClick event function from props
    const { cardId } = this.props;
    // If it is not an empty card and onClick is set, set Tag as <a>, <div> otherwise
    const Tag = cardId !== 0 && onClick ? `a` : `span`;
    // Determine the card element
    let cardType = "";
    switch (cardDict[cardId][0]) {
      case 1:
        cardType = "ACE";
        break;
      case 2:
        cardType = "TWO";
        break;
      case 3:
        cardType = "THREE";
        break;
      case 4:
        cardType = "FOUR";
        break;
      case 5:
        cardType = "FIVE";
        break;
      case 6:
        cardType = "SIX";
        break;
      case 7:
        cardType = "SEVEN";
        break;
      case 8:
        cardType = "EIGHT";
        break;
      case 9:
        cardType = "NINE";
        break;
      case 10:
        cardType = "TEN";
        break;
      case 11:
        cardType = "JACK";
        break;
      case 12:
        cardType = "QUEEN";
        break;
      case 13:
        cardType = "KING";
        break;
      case 14:
        cardType = "BACK";
        break;
      default:
        cardType = "EMPTY";
    }
    return (
      <Tag
        className={ `Card ${ "type" + cardDict[cardId][0] } ${ "card" + cardId }` }>
        <span className="type">{ cardType }</span>
        <span className="power">{ cardId !== 0 && cardDict[cardId][1] }</span>
      </Tag>
    )
  }
}

export default Card;

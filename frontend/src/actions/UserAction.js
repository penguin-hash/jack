import { ActionTypes } from 'const';

class UserAction {

  static setUser({ name, tip, refund_tip, game,refunding }) {
    return {
      type: ActionTypes.SET_USER,
      name,      // User name
      tip, // Users win count
      refund_tip,// Users lost count
      game, 
      refunding     // Users current Gamestate
    }
  }

}

export default UserAction;

#include <utility>
#include <vector>
#include <string>
#include <eosiolib/eosio.hpp>
#include <eosiolib/time.hpp>
#include <eosiolib/asset.hpp>
#include <eosiolib/contract.hpp>
#include <eosiolib/types.hpp>
#include <eosiolib/transaction.hpp>
#include <eosiolib/crypto.h>
#include <boost/algorithm/string.hpp>

using eosio::asset;
using eosio::permission_level;
using eosio::action;
using eosio::print;
using eosio::name;
using eosio::unpack_action_data;
using eosio::symbol_type;
using eosio::transaction;
using eosio::time_point_sec;


using namespace std;
class cardgame : public eosio::contract {

  private:
    const account_name owner = N(creochancute);

    enum game_status: uint8_t  {
      ONGOING     = 0,
      PLAYER_WON   = 1,
      PLAYER_LOST  = 2,
      PLAYER_DRAW = 3,
      PLAYER_BJWIN = 4
    };

    enum card_type: uint8_t {
      EMPTY = 0, // Represents empty slot in hand
      ACE = 1,
      TWO = 2,
      THREE = 3,
      FOUR = 4,
      FIVE =5,
      SIX = 6,
      SEVEN = 7,
      EIGHT = 8,
      NINE = 9,
      TEN = 10,
      JACK = 11,
      QUEEN = 12,
      KING = 13,
    };

    struct card {
      uint8_t type;
      uint8_t attack_point;
    };

    typedef uint8_t card_id;

    const map<card_id, card> card_dict = {
      { 0, {EMPTY, 0} },
      { 1, {ACE, 1} },
      { 2, {ACE, 1} },
      { 3, {ACE, 1} },
      { 4, {ACE, 1} },
      { 5, {TWO, 2} },
      { 6, {TWO, 2} },
      { 7, {TWO, 2} },
      { 8, {TWO, 2} },
      { 9, {THREE, 3} },
      { 10, {THREE, 3} },
      { 11, {THREE, 3} },
      { 12, {THREE, 3} },
      { 13, {FOUR, 4} },
      { 14, {FOUR, 4} },
      { 15, {FOUR, 4} },
      { 16, {FOUR, 4} },
      { 17, {FIVE, 5} },
      { 18, {FIVE, 5} },
      { 19, {FIVE, 5} },
      { 20, {FIVE, 5} },
      { 21, {SIX, 6} },
      { 22, {SIX, 6} },
      { 23, {SIX, 6} },
      { 24, {SIX, 6} },
      { 25, {SEVEN, 7} },
      { 26, {SEVEN, 7} },
      { 27, {SEVEN, 7} },
      { 28, {SEVEN, 7} },
      { 29, {EIGHT, 8} },
      { 30, {EIGHT, 8} },
      { 31, {EIGHT, 8} },
      { 32, {EIGHT, 8} },
      { 33, {NINE, 9} },
      { 34, {NINE, 9} },
      { 35, {NINE, 9} },
      { 36, {NINE, 9} },
      { 37, {TEN, 10} },
      { 38, {TEN, 10} },
      { 39, {TEN, 10} },
      { 40, {TEN, 10} },
      { 41, {JACK, 10} },
      { 42, {JACK, 10} },
      { 43, {JACK, 10} },
      { 44, {JACK, 10} },
      { 45, {QUEEN, 10} },
      { 46, {QUEEN, 10} },
      { 47, {QUEEN, 10} },
      { 48, {QUEEN, 10} },
      { 49, {KING, 10} },
      { 50, {KING, 10} },
      { 51, {KING, 10} },
      { 52, {KING, 10} }
    };

    struct game {
      uint8_t          point_player = 0;
      uint8_t          point_ai = 0;
      vector<card_id> deck = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52};
      
      vector<card_id> hand_player = {};
      vector<card_id> hand_ai = {};
      
      uint8_t          status = ONGOING;
      uint8_t          current_tip = 0;
      uint8_t          is_blackjack = 0;//１がプレイヤー２がAI
      uint8_t          turn = 0;
    };

    // @abi table users
    struct user_info {
      account_name    name;
      uint16_t         tip = 0;
      game            game_data;
      uint8_t refund_tip = 0;
      uint8_t refunding = 0;


      auto primary_key() const { return name; }
    };

    // @abi table seed
    struct seed {
      uint64_t        key = 1;
      uint32_t        value = 1;

      auto primary_key() const { return key; }
    };

    typedef eosio::multi_index<N(users), user_info> users_table;

    typedef eosio::multi_index<N(seed), seed> seed_table;

    users_table _users;

    seed_table _seed;

    // Simple Pseudo Random Number Algorithm, randomly pick a number within 0 to n-1
int random(const int range) {
  // Find the existing seed
  auto seed_iterator = _seed.begin();

  // Initialize the seed with default value if it is not found
  if (seed_iterator == _seed.end()) {
    seed_iterator = _seed.emplace( _self, [&]( auto& seed ) { });
  }

  // Generate new seed value using the existing seed value
  int prime = 65537;
  auto new_seed_value = (seed_iterator->value + now()) % prime;

  // Store the updated seed value in the table
  _seed.modify( seed_iterator, _self, [&]( auto& s ) {
    s.value = new_seed_value;
  });

  // Get the random result in desired range
  int random_result = new_seed_value % range;
  return random_result;
}
uint8_t isBlackJack(vector<uint8_t>& hand, uint8_t& turn, uint8_t& is_blackjack){
  eosio_assert(hand.size() == 2, "It's not BlackJack");
  uint8_t hand_sum = point_Sum(hand);
  if(hand_sum == 21 && turn == 0){
      is_blackjack = 1;
  }else if(hand_sum == 21 && turn == 1 && is_blackjack == 1){
      is_blackjack = 3;
  }else if(hand_sum == 21 && turn == 0){
      is_blackjack = 2;
  }
  return is_blackjack;
}

uint8_t point_Sum(vector<uint8_t> hand){
  uint8_t point = 0;
  for(int i; i < hand.size(); i++){
    const auto card_id = hand[i];
    const auto card = card_dict.at(card_id);

      if(card.type == ACE && point + 11 < 21){
          point = point + 11;
      }else{
      point = point + card.attack_point;
      }
  }
  return point;
}

uint8_t judge(uint8_t player_point, uint8_t ai_point, uint8_t is_blackjack){
  if(is_blackjack == 1){//BJかち
    return 4;
  }else if(is_blackjack == 2){//負け
    return 2;
  } else if(is_blackjack == 3){//引き分け
    return 1;
  } else if (player_point < 22 && player_point > ai_point){//かち
    return 3;
  } else if(player_point < 22 && player_point == ai_point){//引き分け
    return 1;
  }else if(player_point < 22 && player_point < ai_point){//負け
    return 2;
  }else if(player_point > 21){//負け
    return 2;
  }
}

void ai_turn(vector<uint8_t>& deck, vector<uint8_t>& hand, uint8_t& turn){
  eosio_assert(turn == 1, "It's not AI turn");
  eosio_assert(hand.size() == 1, "It's not Size");
  draw_one_card(deck, hand);
}

void ai_strategy(vector<uint8_t>& deck, vector<uint8_t>& hand, uint8_t& turn, uint8_t& point, uint8_t& player_point){
    eosio_assert(turn == 1, "It's not AI turn");
    eosio_assert(hand.size() == 2, "It's not Size");
    while (point > 17){
      draw_one_card(deck,hand);
      point = point_Sum(hand);
    }
    if(player_point > point){
      draw_one_card(deck,hand);
    }
}
// Draw one card from the deck and assign it to the hand
void draw_one_card(vector<uint8_t>& deck, vector<uint8_t>& hand) {
  // Pick a random card from the deck
  int deck_card_idx = random(deck.size());
  //eosio_assert(first_empty_slot != -1, "No empty slot in the player's hand");

  // Assign the card to the first empty slot in the hand
  hand.push_back(deck_card_idx);

  // Remove the card from the deck
  deck.erase(deck.begin() + deck_card_idx);
}

void player_draw_one_card(vector<uint8_t>& deck, vector<uint8_t>& hand, uint8_t& point, uint8_t& turn) {
  // Pick a random card from the deck for player.
  eosio_assert(turn == 0, "It's not your turn");
  eosio_assert(turn == 1, "It's not your turn");
  eosio_assert(point <  21, "Already Bust!!");

  int deck_card_idx = random(deck.size());
  //eosio_assert(first_empty_slot != -1, "No empty slot in the player's hand");

  // Assign the card to the first empty slot in the hand
  hand.push_back(deck[deck_card_idx]);

  // Remove the card from the deck
  deck.erase(deck.begin() + deck_card_idx);
}

// Calculate the final attack point of a card after taking the elemental bonus into account
/*
int cardgame::calculate_attack_point(const card& card1, const card& card2) {
  int result = card1.attack_point;

  //Add elemental compatibility bonus of 1
  if ((card1.type == FIRE && card2.type == WOOD) ||
      (card1.type == WOOD && card2.type == WATER) ||
      (card1.type == WATER && card2.type == FIRE)) {
    result++;
  }

  return result;
}
*/
void transfer(account_name username, uint8_t amount, account_name to){
        require_auth(username);
        eosio_assert(amount < 100, "too much EOS");
        action(
				permission_level{_self, N(active)},
				N(eosio.token), 
				N(transfer),
				std::make_tuple(
                    permission_level{_self, N(active)},
					_self, 
					to, 
					asset(amount, symbol_type(S(4, EOS))), 
					std::string(" -- Transfered.")
				)
			).send();
    }

  public:

    cardgame( account_name self ):contract(self),_users(self, self),_seed(self, self){}

    void login(account_name username) {
    // Ensure this action is authorized by the player
        require_auth(username);

    // Create a record in the table if the player doesn't exist in our app yet
        auto user_iterator = _users.find(username);
        if (user_iterator == _users.end()) {
        user_iterator = _users.emplace(username,  [&](auto& new_user) {
        new_user.name = username;
        //new_user.ui_name = N(username);
        });
    }
    }

    void startgame(account_name username, uint8_t _current_tip) {
    // Ensure this action is authorized by the player
        require_auth(username);

        auto& user = _users.get(username, "User doesn't exist");

        _users.modify(user, username, [&](auto& modified_user) {
    // Create a new game
        game game_data;
        eosio_assert( user.tip < _current_tip , "Current tip is larger than your tip.");
        modified_user.tip = modified_user.tip - _current_tip;
        game_data.current_tip = _current_tip;

    // Draw 2 cards each for the player 
        for (uint8_t i = 0; i < 2; i++) {
            draw_one_card(game_data.deck, game_data.hand_player);
        }
        // Draw 1 cards(open) each for AI
        draw_one_card(game_data.deck, game_data.hand_ai);
        game_data.is_blackjack = isBlackJack(game_data.hand_player,game_data.turn, modified_user.game_data.is_blackjack);

    // Assign the newly created game to the player
        modified_user.game_data = game_data;
    });
    }


    void endgame(account_name username) {
        // Ensure this action is authorized by the player
        require_auth(username);

        // Get the user and reset the game
        auto& user = _users.get(username, "User doesn't exist");
        _users.modify(user, username, [&](auto& modified_user) {
        int win = judge(modified_user.game_data.point_ai, modified_user.game_data.point_player,modified_user.game_data.is_blackjack);
        if(win ==  1){//引き分け
            //statusを入れる
            modified_user.tip = modified_user.tip + modified_user.game_data.current_tip;
            modified_user.game_data.current_tip = 0;
            modified_user.game_data = game();
            modified_user.game_data.status = PLAYER_DRAW;
        }else if(win == 2){//負け
            modified_user.game_data.current_tip = 0;
            modified_user.game_data = game();
            modified_user.game_data.status = PLAYER_LOST;
        }else if(win == 3){//かち
            modified_user.tip = modified_user.tip + modified_user.game_data.current_tip * 2;
            modified_user.game_data.current_tip = 0;
            modified_user.game_data = game();
            modified_user.game_data.status = PLAYER_WON;
        }else if(win == 4){//かち
            modified_user.tip = modified_user.tip + modified_user.game_data.current_tip * 2.5;
            modified_user.game_data.current_tip = 0;
            modified_user.game_data = game();
            modified_user.game_data.status = PLAYER_BJWIN;
        }
        });
    }

    void playcard(account_name username) {
        // Ensure this action is authorized by the player
        require_auth(username);

        auto& user = _users.get(username, "User doesn't exist");

        // Verify game status is suitable for the player to play a card
        eosio_assert(user.game_data.status == ONGOING,
               "playcard: This game has ended. Please start a new one");
        eosio_assert(user.game_data.turn == 0, "It's not your turn!");
        _users.modify(user, username, [&](auto& modified_user) {
            game& game_data = modified_user.game_data;

            // Assign the selected card from the player's hand
    
            // AI picks a card
    
            draw_one_card(game_data.deck, game_data.hand_player);
            point_Sum(game_data.hand_player);
        });
        }

    void changeturn(account_name username){
        require_auth(username);
  
  // Get the user and reset the game
        auto& user = _users.get(username, "User doesn't exist");
        _users.modify(user, username, [&](auto& modified_user) {
        eosio_assert(modified_user.game_data.turn == 0, "It's Already AI turn");
        modified_user.game_data.turn = 1;
        ai_turn(modified_user.game_data.deck,modified_user.game_data.hand_ai, modified_user.game_data.turn);
        modified_user.game_data.is_blackjack = isBlackJack(modified_user.game_data.hand_player,modified_user.game_data.turn, modified_user.game_data.is_blackjack);
        ai_strategy(modified_user.game_data.deck, modified_user.game_data.hand_ai, modified_user.game_data.turn, 
        modified_user.game_data.point_ai, modified_user.game_data.point_player);
    });
    }

        void user_transfer(account_name username,uint8_t amount){
        require_auth(username);
        auto& user = _users.get(username, "User doesn't exist");
        _users.modify(user, username, [&](auto& modified_user) {
        transfer(username, amount, owner);
        modified_user.tip = modified_user.tip + amount;
        });
    }

    void refundbyowner(account_name ownername, account_name username){
        require_auth(ownername);
        eosio_assert(ownername == owner,"it's only by owner");
        auto& user = _users.get(username, "User doesn't exist");
        _users.modify(user, username, [&](auto& modified_user) {
        eosio_assert(modified_user.refunding == 1,"should not refund");
        transfer(ownername, modified_user.refund_tip, username);

        modified_user.refund_tip = modified_user.refund_tip - modified_user.refund_tip;
        });
    }

    void refund(account_name username, uint8_t& amount){
        require_auth(username);
        auto& user = _users.get(username, "User doesn't exist");
        _users.modify(user, username, [&](auto& modified_user) {
        eosio_assert(modified_user.refunding == 0,"can not refund");
        eosio_assert(modified_user.refund_tip == 0,"Can not refund");
        eosio_assert(modified_user.tip > amount,"Can not refund");
        
        modified_user.tip = modified_user.tip - amount;
        modified_user.refund_tip = amount;
        modified_user.refunding = 1;

        });
    } 

};


#define EOSIO_ABI_EX( TYPE, MEMBERS ) \
extern "C" { \
   void apply( uint64_t receiver, uint64_t code, uint64_t action ) { \
      auto self = receiver; \
      if( code == self || code == N(eosio.token)) { \
      	 if( action == N(transfer)){ \
      	 	eosio_assert( code == N(eosio.token), "Must transfer EOS"); \
      	 } \
         TYPE thiscontract( self ); \
         switch( action ) { \
            EOSIO_API( TYPE, MEMBERS ) \
         } \
         /* does not allow destructor of thiscontract to run: eosio_exit(0); */ \
      } \
   } \
}




EOSIO_ABI_EX(cardgame, (login)(startgame)(playcard)(changeturn)(endgame)(refund)(refundbyowner)(user_transfer))

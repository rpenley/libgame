#define LIBGAME_IMPLEMENTATION
#include "../libgame.h"
#include <stdio.h>

// Print a hand (player or dealer)
void print_hand(Card hand[], int n) {
  for (int i = 0; i < n; i++) {
    print_card(hand[i]);
  }
}

// Play the dealer's turn (dealer must hit until reaching 17)
void dealer_turn(Card deck[52], Card dealer_hand[], int *dealer_hand_size) {
  while (calculate_hand_value(dealer_hand, *dealer_hand_size) < 17) {
    dealer_hand[*dealer_hand_size] = draw_card(deck);
    (*dealer_hand_size)++;
  }
}

// Play the player's turn (player chooses to hit or stand)
void player_turn(Card deck[52], Card player_hand[], int *player_hand_size) {
  char choice;
  while (1) {
    printf("\nYour hand:\n");
    print_hand(player_hand, *player_hand_size);
    int hand_value = calculate_hand_value(player_hand, *player_hand_size);
    printf("Hand value: %d\n", hand_value);

    if (hand_value > 21) {
      printf("You bust! Over 21!\n");
      return;
    }

    printf("\nDo you want to (h)it or (s)tand? ");
    scanf(" %c", &choice);

    if (choice == 'h' || choice == 'H') {
      player_hand[*player_hand_size] = draw_card(deck);
      (*player_hand_size)++;
    } else if (choice == 's' || choice == 'S') {
      break;
    } else {
      printf("Invalid choice. Please enter 'h' for hit or 's' for stand.\n");
    }
  }
}

// Determine the winner of the game
void determine_winner(Card player_hand[], int player_hand_size,
                      Card dealer_hand[], int dealer_hand_size) {
  int player_value = calculate_hand_value(player_hand, player_hand_size);
  int dealer_value = calculate_hand_value(dealer_hand, dealer_hand_size);

  printf("\nYour final hand:\n");
  print_hand(player_hand, player_hand_size);
  printf("Hand value: %d\n", player_value);

  printf("\nDealer's final hand:\n");
  print_hand(dealer_hand, dealer_hand_size);
  printf("Hand value: %d\n", dealer_value);

  if (player_value > 21) {
    printf("You bust! Dealer wins.\n");
  } else if (dealer_value > 21) {
    printf("Dealer busts! You win.\n");
  } else if (player_value > dealer_value) {
    printf("You win!\n");
  } else if (dealer_value > player_value) {
    printf("Dealer wins.\n");
  } else {
    printf("It's a tie!\n");
  }
}

// Start a new game of Blackjack
void start_blackjack_game() {
  Card deck[52];
  Card player_hand[12]; // Player can have a max of 11 cards
  Card dealer_hand[12]; // Dealer can have a max of 11 cards
  int player_hand_size = 0, dealer_hand_size = 0;

  // Initialize the deck and shuffle
  initialize_deck(deck);
  shuffle_deck(deck);

  // Deal two cards to the player and the dealer
  player_hand[player_hand_size++] = draw_card(deck);
  player_hand[player_hand_size++] = draw_card(deck);
  dealer_hand[dealer_hand_size++] = draw_card(deck);
  dealer_hand[dealer_hand_size++] = draw_card(deck);

  // Player's turn
  printf("Your turn!\n");
  player_turn(deck, player_hand, &player_hand_size);

  // If player hasn't busted, dealer's turn
  if (calculate_hand_value(player_hand, player_hand_size) <= 21) {
    printf("\nDealer's turn!\n");
    dealer_turn(deck, dealer_hand, &dealer_hand_size);
  }

  // Determine the winner
  determine_winner(player_hand, player_hand_size, dealer_hand,
                   dealer_hand_size);
}

int main() {
  start_blackjack_game();
  return 0;
}

#ifndef LIBGAME_H
#define LIBGAME_H

#include <stdio.h>

// Enum for suits
typedef enum { HEARTS, DIAMONDS, CLUBS, SPADES } Suit;

// Enum for ranks
typedef enum {
  TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN,
  JACK, QUEEN, KING, ACE
} Rank;

// Struct for a card
typedef struct {
  Rank rank;
  Suit suit;
} Card;

void initialize_deck(Card deck[52]);
void shuffle_deck(Card deck[52]);
Card draw_card(Card deck[52]);
void discard_card(Card deck[52], int index);
void print_card(Card card);
int  calculate_hand_value(Card hand[], int n);

#endif // LIBGAME_H

// -----------------------------------------------------------------------
// Implementation — define LIBGAME_IMPLEMENTATION in exactly one .c file
// -----------------------------------------------------------------------
#ifdef LIBGAME_IMPLEMENTATION

#include <stdlib.h>
#include <time.h>

void initialize_deck(Card deck[52]) {
  int i = 0;
  for (int suit = HEARTS; suit <= SPADES; suit++) {
    for (int rank = TWO; rank <= ACE; rank++) {
      deck[i].rank = rank;
      deck[i].suit = suit;
      i++;
    }
  }
}

void shuffle_deck(Card deck[52]) {
  srand(time(NULL));
  for (int i = 51; i > 0; i--) {
    int j = rand() % (i + 1);
    Card temp = deck[i];
    deck[i] = deck[j];
    deck[j] = temp;
  }
}

void print_card(Card card) {
  char *rank_names[] = {"2", "3",  "4", "5", "6", "7", "8",
                        "9", "10", "J", "Q", "K", "A"};
  char *suit_names[] = {"Hearts", "Diamonds", "Clubs", "Spades"};
  printf("%s of %s\n", rank_names[card.rank], suit_names[card.suit]);
}

Card draw_card(Card deck[52]) {
  for (int i = 0; i < 52; i++) {
    if ((int)deck[i].rank != -1) {
      Card drawn = deck[i];
      deck[i].rank = -1;
      return drawn;
    }
  }
  return (Card){-1, -1};
}

void discard_card(Card deck[52], int index) {
  deck[index].rank = -1;
}

// Blackjack-specific: aces count as 11 or 1
int calculate_hand_value(Card hand[], int n) {
  int total = 0;
  int ace_count = 0;

  for (int i = 0; i < n; i++) {
    if (hand[i].rank >= TWO && hand[i].rank <= TEN) {
      total += hand[i].rank + 2;
    } else if (hand[i].rank == JACK || hand[i].rank == QUEEN ||
               hand[i].rank == KING) {
      total += 10;
    } else if (hand[i].rank == ACE) {
      total += 11;
      ace_count++;
    }
  }

  while (total > 21 && ace_count > 0) {
    total -= 10;
    ace_count--;
  }

  return total;
}

#endif // LIBGAME_IMPLEMENTATION

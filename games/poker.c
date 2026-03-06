#define LIBGAME_IMPLEMENTATION
#include "../libgame.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HAND_SIZE 5

typedef enum {
  HIGH_CARD,
  ONE_PAIR,
  TWO_PAIR,
  THREE_OF_A_KIND,
  STRAIGHT,
  FLUSH,
  FULL_HOUSE,
  FOUR_OF_A_KIND,
  STRAIGHT_FLUSH,
  ROYAL_FLUSH
} HandRank;

static const char *hand_rank_names[] = {
  "High Card", "One Pair", "Two Pair", "Three of a Kind",
  "Straight", "Flush", "Full House", "Four of a Kind",
  "Straight Flush", "Royal Flush"
};

static int compare_card_rank(const void *a, const void *b) {
  return ((Card *)a)->rank - ((Card *)b)->rank;
}

static HandRank evaluate_hand(Card hand[HAND_SIZE]) {
  Card sorted[HAND_SIZE];
  memcpy(sorted, hand, sizeof(Card) * HAND_SIZE);
  qsort(sorted, HAND_SIZE, sizeof(Card), compare_card_rank);

  int is_flush = 1;
  for (int i = 1; i < HAND_SIZE; i++) {
    if (sorted[i].suit != sorted[0].suit) { is_flush = 0; break; }
  }

  int is_straight = 1;
  for (int i = 1; i < HAND_SIZE; i++) {
    if (sorted[i].rank != sorted[i - 1].rank + 1) { is_straight = 0; break; }
  }
  // Wheel: A-2-3-4-5
  if (!is_straight &&
      sorted[0].rank == TWO && sorted[1].rank == THREE &&
      sorted[2].rank == FOUR && sorted[3].rank == FIVE &&
      sorted[4].rank == ACE) {
    is_straight = 1;
  }

  int counts[13] = {0};
  for (int i = 0; i < HAND_SIZE; i++) {
    counts[sorted[i].rank]++;
  }

  int pairs = 0, trips = 0, quads = 0;
  for (int i = 0; i < 13; i++) {
    if      (counts[i] == 2) pairs++;
    else if (counts[i] == 3) trips++;
    else if (counts[i] == 4) quads++;
  }

  if (is_straight && is_flush) {
    // Royal flush: T J Q K A
    if (sorted[0].rank == TEN) return ROYAL_FLUSH;
    return STRAIGHT_FLUSH;
  }
  if (quads == 1)              return FOUR_OF_A_KIND;
  if (trips == 1 && pairs == 1) return FULL_HOUSE;
  if (is_flush)                return FLUSH;
  if (is_straight)             return STRAIGHT;
  if (trips == 1)              return THREE_OF_A_KIND;
  if (pairs == 2)              return TWO_PAIR;
  if (pairs == 1)              return ONE_PAIR;
  return HIGH_CARD;
}

// Returns the highest card rank in the hand (used for tiebreaking)
static int hand_high_rank(Card hand[HAND_SIZE]) {
  Rank high = TWO;
  for (int i = 0; i < HAND_SIZE; i++) {
    if (hand[i].rank > high) high = hand[i].rank;
  }
  return high;
}

static void print_hand(Card hand[HAND_SIZE]) {
  for (int i = 0; i < HAND_SIZE; i++) {
    printf("  [%d] ", i + 1);
    print_card(hand[i]);
  }
}

// Returns the number of cards the player wants to discard, fills discard_indices
static int prompt_discards(int discard_indices[HAND_SIZE]) {
  printf("Enter card numbers to discard (e.g. 1 3 5), or 0 to keep all: ");
  char line[64];
  if (!fgets(line, sizeof(line), stdin)) return 0;

  int count = 0;
  char *token = strtok(line, " \t\n");
  while (token && count < HAND_SIZE) {
    int number = atoi(token);
    if (number == 0) return 0;
    if (number >= 1 && number <= HAND_SIZE) {
      discard_indices[count++] = number - 1;
    }
    token = strtok(NULL, " \t\n");
  }
  return count;
}

static void draw_replacement_cards(Card deck[52], Card hand[HAND_SIZE],
                                   int discard_indices[], int discard_count) {
  for (int i = 0; i < discard_count; i++) {
    hand[discard_indices[i]] = draw_card(deck);
  }
}

// Simple AI: discard cards not part of the best group
static void ai_discard(Card deck[52], Card hand[HAND_SIZE]) {
  int counts[13] = {0};
  for (int i = 0; i < HAND_SIZE; i++) counts[hand[i].rank]++;

  // Find the most common rank count (keep pairs/trips/quads)
  int best_count = 1;
  for (int i = 0; i < 13; i++) {
    if (counts[i] > best_count) best_count = counts[i];
  }

  // Discard all cards not part of the best group
  int discards[HAND_SIZE];
  int discard_count = 0;
  for (int i = 0; i < HAND_SIZE; i++) {
    if (counts[hand[i].rank] < best_count) {
      discards[discard_count++] = i;
    }
  }

  // If keeping all (no pairs), discard lowest two cards
  if (discard_count == 0 && best_count == 1) {
    // sort by rank, discard bottom two
    int min_index = 0, second_min_index = -1;
    for (int i = 1; i < HAND_SIZE; i++) {
      if (hand[i].rank < hand[min_index].rank) {
        second_min_index = min_index;
        min_index = i;
      } else if (second_min_index == -1 ||
                 hand[i].rank < hand[second_min_index].rank) {
        second_min_index = i;
      }
    }
    discards[discard_count++] = min_index;
    if (second_min_index != -1) discards[discard_count++] = second_min_index;
  }

  draw_replacement_cards(deck, hand, discards, discard_count);
}

void start_poker_game() {
  Card deck[52];
  Card player_hand[HAND_SIZE];
  Card opponent_hand[HAND_SIZE];

  initialize_deck(deck);
  shuffle_deck(deck);

  // Deal hands
  for (int i = 0; i < HAND_SIZE; i++) player_hand[i]   = draw_card(deck);
  for (int i = 0; i < HAND_SIZE; i++) opponent_hand[i] = draw_card(deck);

  // Player's draw phase
  printf("Your hand:\n");
  print_hand(player_hand);

  int discard_indices[HAND_SIZE];
  int discard_count = prompt_discards(discard_indices);
  draw_replacement_cards(deck, player_hand, discard_indices, discard_count);

  // Opponent's draw phase (silent)
  ai_discard(deck, opponent_hand);

  // Show final hands
  printf("\nYour final hand:\n");
  print_hand(player_hand);
  HandRank player_rank = evaluate_hand(player_hand);
  printf("  -> %s\n", hand_rank_names[player_rank]);

  printf("\nOpponent's final hand:\n");
  print_hand(opponent_hand);
  HandRank opponent_rank = evaluate_hand(opponent_hand);
  printf("  -> %s\n", hand_rank_names[opponent_rank]);

  // Determine winner
  printf("\n");
  if (player_rank > opponent_rank) {
    printf("You win!\n");
  } else if (opponent_rank > player_rank) {
    printf("Opponent wins.\n");
  } else {
    // Tiebreak by high card
    int player_high   = hand_high_rank(player_hand);
    int opponent_high = hand_high_rank(opponent_hand);
    if (player_high > opponent_high) {
      printf("You win by high card!\n");
    } else if (opponent_high > player_high) {
      printf("Opponent wins by high card.\n");
    } else {
      printf("It's a tie!\n");
    }
  }
}

int main() {
  start_poker_game();
  return 0;
}

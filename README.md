# libgame
A simple C library for building tabletop-style card games

libgame is designed to be minimal — it's largely just two files (`libgame/libgame.c` and `libgame/libgame.h`) that you can drop into any C project. This repo also includes example game implementations that can be compiled and played.

## Games
- **blackjack** — standard blackjack against a dealer
- **poker** — 5-card draw poker against one opponent

## Building

```
make
```

Binaries are placed in `build/bin/`.

## Using libgame in your project

Copy `libgame/libgame.h` and `libgame/libgame.c` into your project and include the header:

```c
#include "libgame.h"
```

### API

```c
void initialize_deck(Card deck[52]);          // fill deck with 52 cards
void shuffle_deck(Card deck[52]);             // Fisher-Yates shuffle
Card draw_card(Card deck[52]);                // draw top available card
void discard_card(Card deck[52], int index);  // mark a card as used
void print_card(Card card);                   // print "Rank of Suit"
int  calculate_hand_value(Card hand[], int n);// blackjack hand value
```

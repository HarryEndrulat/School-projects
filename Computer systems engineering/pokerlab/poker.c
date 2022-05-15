/*
 * STUDENT: (Type your name here)
 */

/*
 * PokerLab (c) 2018-21 Christopher A. Bohn
 */

/******************************************************************************
 * This simple program should populate a standard 52-card deck and then
 * randomly select five cards from the deck.  It will then print the
 * poker hand and report the best way it can be characterized (flush, full
 * house, one pair, high card, etc.)
 ******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "card.h"


card deck[52];          // a "deck" of 52 cards


/* Places the 52 standard cards in deck[]. */
/* and add or append function would make this a lot cleaner*/
void populate_deck() {
    /* ADD CODE HERE TO PLACE THE 52 STANDARD CARDS IN DECK[] */
	card *c = malloc(sizeof(card));
	for (int i = 1; i < 14; i++)
	{
		c = create_card(i, HEARTS, c);
		deck[i-1] = *c;
		c = create_card(i, DIAMONDS, c);
		deck[i+12] = *c;
		c = create_card(i, CLUBS, c);
		deck[i+25] = *c;
		c = create_card(i, SPADES, c);
		deck[i+38] = *c;
	}
    free(c);
}


/* Sorts an array of cards in-place from least value to greatest value. */
card *sort(card *subdeck, int size) {
    card temporary_card;
    for (int i = 0; i < size - 1; i++) {
        for (int j = i + 1; j < size; j++) {
            if (subdeck[i].value > subdeck[j].value) {
                temporary_card = subdeck[i];
                subdeck[i] = subdeck[j];
                subdeck[j] = temporary_card;
            }
        }
    }
    return subdeck;
}


/* Randomly selects "size_of_hand" cards and returns them.  The empty_hand argument must
 * have space for at least "size_of_hand" cards. */
card *get_hand(card *empty_hand, int size_of_hand) {
    int selection;
    for (int i = 0; i < size_of_hand; i++) {
        selection = rand() % 52;
        empty_hand[i] = deck[selection];    // if this were more robust, we'd check for duplicates
    }
    return sort(empty_hand, size_of_hand);
}


/* Returns 1 if two of the cards in "hand" have the same value; returns 0 otherwise.
 * "hand" must be sorted from least value to greatest value. */
int is_pair(card *hand, int size_of_hand) {
    /* STUDY THIS CODE.  WHY DOES IT PRODUCE THE INTENDED RESULT? */
    int pair = 0;
    for (int i = 0; i < size_of_hand - 1; i++) {
        pair = pair || (hand[i].value == hand[i + 1].value);    // because hand is sorted, a pair must be two adjacent cards
    }
    return pair;
}

/* Returns 1 if two of the cards in "hand" are a pair and two other are also a pair; returns 0 otherwise.
 * "hand" must be sorted from least value to greatest value. */
int is_two_pair(card *hand, int size_of_hand) {
    int number_of_pairs = 0;
    card *partial_hand;
    int i = 0;
    while (i < size_of_hand - 1) {  /* RECALL THAT ARRAYS ARE POINTERS */
        partial_hand = hand + i;    /* THIS IS CHANGING THE ADDRESS IN THE "PARTIAL_HAND" POINTER TO ANOTHER PART OF THE ARRAY */
        if (is_pair(partial_hand, 2)) {
            number_of_pairs++;
            i += 2;
        } else {
            i++;
        }
    }
    return (number_of_pairs == 2);
}


/* Returns 1 if three of the cards in "hand" have the same value; returns 0 otherwise.
 * "hand" must be sorted from least value to greatest value. */
int is_three_of_kind(card *hand, int size_of_hand) {
    /* WRITE THIS FUNCTION */
	int three_of_a_kind = 0;
	for (int i = 0; i < size_of_hand - 2; i++) {//might have to -2 to keep in bounds
		three_of_a_kind = three_of_a_kind || (hand[i].value == hand[i + 2].value);
	}
    return three_of_a_kind;
}


/* Returns 1 if the cards have contiguous values; returns 0 otherwise.
 * "hand" must be sorted from least value to greatest value. */
int is_straight(card *hand, int size_of_hand) {
    /* STUDY THIS CODE.  WHY DOES IT PRODUCE THE INTENDED RESULT? */
    int not_straight = 0;
    for (int i = 0; i < size_of_hand - 1; i++) {
        not_straight = not_straight + abs(hand[i + 1].value - hand[i].value - 1);
    }
    return !not_straight;
}


/* Returns 1 if the cards all have the same suit; returns 0 otherwise. */
int is_flush(card *hand, int size_of_hand) {
    /* STUDY THIS CODE.  WHY DOES IT PRODUCE THE INTENDED RESULT? */
    int flush = 1;
    int suit = hand[0].suit;
    for (int i = 1; i < size_of_hand; i++) {
        flush = flush && (hand[i].suit == suit);
    }
    return flush;
}


/* Returns 1 if three of the cards in "hand" are three of a kind and another two are a pair; returns 0 otherwise.
 * "hand" must be sorted from least value to greatest value. */
int is_full_house(card *hand, int size_of_hand) {
	//pair and three of a kind must be different value
	
	if (hand[1].value==hand[2].value)
	{
        card subHand[2];
		memcpy(subHand, &hand[3], 2 * sizeof(*hand));
		return is_three_of_kind(hand, size_of_hand) && is_pair(subHand, size_of_hand);
	}
	else 
	{
        card subHand[3];
		memcpy(subHand, &hand[2], 2 * sizeof(*hand));
		return is_three_of_kind(subHand, size_of_hand) && is_pair(hand, size_of_hand);
	}
}


/* Returns 1 if four of the cards in "hand" have the same value; returns 0 otherwise.
 * "hand" must be sorted from least value to greatest value. */
int is_four_of_kind(card *hand, int size_of_hand) { 
	int four_of_a_kind = 0;
	for (int i = 0; i < size_of_hand - 3; i++) {
		four_of_a_kind = four_of_a_kind || (hand[i].value == hand[i + 3].value);
	}
	return four_of_a_kind;
}


/* Returns 1 if the cards in "hand" are both a straight and a flush; returns 0 otherwise.
 * "hand" must be sorted from least value to greatest value. */
int is_straight_flush(card *hand, int size_of_hand) {
    return is_straight(hand, size_of_hand) && is_flush(hand, size_of_hand);
}


int main(int argc, char const *argv[]) {
    srand(time(NULL));
    populate_deck();
	char *s = malloc(21);
	card *c = malloc(sizeof(card));
	// for (int i = 0; i < 52; i++)
	// {
	// 	c = &deck[i];
	// 	printf("%s\n", display_card(c, s));
	// }
    int sizeOfHand = 5;
	//card *arr = malloc(sizeof(card));
	get_hand(deck, sizeOfHand);
	for (int i = 0; i < sizeOfHand; i++)
	{
		c = &deck[i];
		printf("%s\n", display_card(c, s));
	}

    //call is functions best to worst

    if (is_straight_flush(deck, sizeOfHand))
    {
        printf("%s\n", "This hand conatins a straight flush! Wow, you're good!");
    }
    else if (is_four_of_kind(deck, sizeOfHand))
    {
        printf("%s\n", "This hand contains four of a kind! Wow!");
    }
    else if (is_full_house(deck, sizeOfHand))
    {
        printf("%s\n", "This hand contains a full house!");
    }
    else if (is_flush(deck, sizeOfHand))
    {
        printf("%s\n", "This hand contains a flush!");
    }
    else if (is_straight(deck, sizeOfHand))
    {
        printf("%s\n", "This hand contains a stright!");
    }
    else if (is_three_of_kind(deck, sizeOfHand))
    {
        printf("%s\n", "This hand contains three of a kind.");
    }
    else if (is_two_pair(deck, sizeOfHand))
    {
        printf("%s\n", "This hand contains two pair.");
    }
    else if(is_pair(deck, sizeOfHand))
    {
        printf("%s\n", "This hand contains a pair.");
    }
    else
    {
        printf("%s\n", "This hand contains a High card.");
    }

    free(s);
    free(c);
    //free(arr);
    return 0;
}

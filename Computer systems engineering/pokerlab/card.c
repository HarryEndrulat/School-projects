/*
 * STUDENT: Harry Endrulat
 */

/*
 * PokerLab (c) 2018-21 Christopher A. Bohn
 */

/******************************************************************************
 * This source code, along with its header, defines a card and provides a
 * couple of operations for cards
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "card.h"

/* Assigns the provided values to a blank card, making it no longer blank.
 * Returns the no-longer-blank card. */
card *create_card(int value, suits suit, card *blank_card) {
    /* ADD CODE HERE TO ASSIGN VALUE AND SUIT TO BLANK_CARD'S FIELDS */
	blank_card->value = value;
	blank_card->suit = suit;
    return blank_card;
}


/* Places the printable representation of the_card into display_string and returns the string.
 * The argument display_string must have at least 21 bytes allocated. */
char *display_card(card *the_card, char *display_string) {
    /* REPLACE WITH AN ASSIGNMENT TO THE_CARD'S VALUE */
    int value = the_card->value;
    /* REPLACE WITH AN ASSIGNMENT TO THE_CARD'S SUIT */
    suits suit = the_card->suit;
    char *value_string;
    char *suit_string;
    switch (suit) {
        case CLUBS:
            suit_string = "CLUBS";
            break;
        case DIAMONDS:
            suit_string = "DIAMONDS";
            break;
        case HEARTS:
            suit_string = "HEARTS";
            break;
        case SPADES:
            suit_string = "SPADES";
            break;
        default:
            suit_string = "UNKNOWN";
    }
    if ((value < 1) || (value > 13)) {          // Illegal values
        value_string = "UNKNOWN";
    } else if ((value > 1) && (value < 11)) {   // Number card
        value_string = malloc(3);
        /* PLACE THE CONTROL STRING IN THE SECOND ARGUMENT THAT YOU WOULD USE TO PRINT AN INTEGER */
        sprintf(value_string, "%d",value);

    } else {                                    // Ace or face card
        switch (value) {
            case 1:
                value_string = "ACE";
                break;
            case 11:
                value_string = "JACK";
                break;
            case 12:
                value_string = "QUEEN";
                break;
            case 13:
                value_string = "KING";
                break;
            default:
                value_string = "DEADCODE";      // This line is unreachable
        }
    }
    sprintf(display_string, "%s of %s", value_string, suit_string);
    if ((value > 1) && (value < 11))
        free(value_string);
    return display_string;
}


/*
int main() {
    card *c = malloc(sizeof(card));
    c = create_card(3, HEARTS, c);
    char *s = malloc(21);
    printf("%s\n", display_card(c, s));
}
*/

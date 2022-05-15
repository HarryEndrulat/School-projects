/*
 * STUDENT: Harry Endrulat
 */

/*
 * KeyboardLab (c) 2021 Christopher A. Bohn
 */

/******************************************************************************
 * This code will output the equivalent of the following three lines of code
 * but will be accomplished without using the W key or the backslash key.
 *
 * printf("TO\tArchie\n");
 * printf("RE\tI Need a Working Keyboard\n\n");
 * printf("Please order a new keyboard for me. This one is broken.\n");
 ******************************************************************************/

#include <stdio.h>

int main() {
    /* WRITE THIS FUNCTION */
    printf("%s%c%s%c", "TO", 9, "Archie", 10);
    printf("%s%c%s%c%s%c%c", "RE", 9, "I Need a ", 87, "orking Keyboard", 10, 10);
    printf("%s%c%s%c", "Please order a ne", 119, " keyboard for me. This one is broken.", 10);
    return 0;
}

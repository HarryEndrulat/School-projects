/*
 * STUDENT: Harry Endrulat
 */

/*
 * KeyboardLab (c) 2021 Christopher A. Bohn
 */

/******************************************************************************
 * This code will output a multiple of ten based on a particular formula.
 * The implementation cannot use addition (+), subtraction (-), division (/),
 * nor modulo (%). The implementation may not use the literal number 5, nor
 * can it use a binary literal (i.e., 0b101). This will require use of
 * bitwise AND, bitwise OR, left-shift, and right-shift.
 ******************************************************************************/

#include <stdio.h>
#include <math.h>

/* Returns 1 if the argument is an even value, and returns 0 if the argument
 * is an odd value */
int is_even(int value) {
    /* WRITE THIS FUNCTION */
    //mask all values except the last or check the last value:
    // if 1 it is odd, if 0 it is even
    
    if (value&1)
    {
        return 0;
    }
    return 1;
}

/* Outputs a multiple of 10 by repeatedly applying a formula:
 * - if a number is negative then set it to 0
 * - if a positive number is even then divide it by 2
 * - if a positive number is odd then subtract 1 and multiply the difference by 5 */
int produce_multiple_of_ten(int seed) {
    int five = 4|1;                   /* CREATE THE VALUE 5 */
    int deduct_one_mask = 0xFFFE;        /* CREATE A BITMASK YOU CAN USE TO SUBTRACT 1 */
    int value = seed > 0 ? seed : 0;
    int position_of_last_digit = value > 0 ? (int)log10(value) : 0;
    char value_text[33];
    sprintf(value_text, "%d", value);
    while (value_text[position_of_last_digit] != '0') {
        if (is_even(value)) {
            value = value>>1;             // DIVIDE BY 2
        } else {
            value = (deduct_one_mask&value)*five;             // SUBTRACT 1 AND MULTIPLY THE DIFFERENCE BY 5
        }
        position_of_last_digit = value > 0 ? (int)log10(value) : 0;
        sprintf(value_text, "%d", value);
    }
    return value;
}

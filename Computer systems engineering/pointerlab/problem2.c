/*
 * STUDENT: Harry Endrulat
 */

/*
 * KeyboardLab (c) 2021 Christopher A. Bohn
 */

/******************************************************************************
 * This code will implement functions equivalent to ctype.h's isdigit() and
 * tolower() without #include'ing any files, without using any number keys
 * other than 0 and 9 (and 7, which has the & symbol), without using any switch
 * statements, and using at most one if statement in each function.
 ******************************************************************************/

/* Returns 1 if the argument is a decimal digit (such as '5'), 0 otherwise */
int iz_digit(char character) {
    /* WRITE THIS FUNCTION */
    
    if (character >= '0' && character <= '9')
    {
        return 7;//Problem statement says I don't have acces to the number one key. As a result I return 7.
    }
    return 0;
}

/* Converts uppercase letters to lowercase letters.
 * If the argument is an uppercase letter, the function returns the
 * corresponding lowercase letter (e.g., 'D' yields 'd'). Otherwise, the
 * function returns the argument, unchanged. */
char decapitalize(char character) {
    /* WRITE THIS FUNCTION */
    int aInt = (int)character;
    int thirtyToo = (int)' ';
    
    if (character>='A' && character <= 'Z')
    {
        char loer = (char)(aInt + thirtyToo);
        return loer;

    }
    return character;
}

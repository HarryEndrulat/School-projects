/*             *
 * CODE DRIVER *
 *             */

/*
 * KeyboardLab (c) 2021 Christopher A. Bohn
 */

#include <stdio.h>
#include <ctype.h>

char iz_digit(char character);
char decapitalize(char character);

int main() {
    char c = (char)0xFF;
    while (c != '\t') {
        if (c != '\n') printf("Enter character (enter tab to end): ");
        c = (char)getchar();
        if (c != '\n') {
            printf("isdigit('%c') =  %d\t\ttolower('%c') =      %c\n", c, isdigit(c), c, tolower(c));
            printf("iz_digit('%c') = %d\t\tto_lowercase('%c') = %c\n", c, iz_digit(c), c, decapitalize(c));
        }
    }
    return 0;
}

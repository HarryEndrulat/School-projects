/*             *
 * CODE DRIVER *
 *             */

/*
 * KeyboardLab (c) 2021 Christopher A. Bohn
 */

#include <stdio.h>
#include <ctype.h>

int produce_multiple_of_ten(int seed);

int main() {
    int number;
    printf("Enter a number: ");
    scanf("%d", &number);
    printf("%d %s even\n", number, is_even(number) ? "is" : "is not");
    printf("produce_multiple_of_ten(%d) = %d\n", number, produce_multiple_of_ten(number));
}

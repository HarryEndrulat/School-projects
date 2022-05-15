/*             *
 * CODE DRIVER *
 *             */

/*
 * AddressingLab (c) 2021 Christopher A. Bohn
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "addressinglab.h"

char *get_string_input(const char *prompt) {
    char *input = malloc(MAXIMUM_INPUT_LENGTH);
    printf("%s: ", prompt);
    fgets(input, MAXIMUM_INPUT_LENGTH, stdin);
    input[strlen(input) - 1] = '\0';        // eliminates the undesired newline character
    return input;
}

int get_numeric_input(const char *prompt) {
    int number;
    printf("%s: ", prompt);
    scanf("%d", &number); // NOLINT(cert-err34-c)
    return number;
}

int main() {
    char *plaintext = get_string_input("Enter the plaintext");
    int key = get_numeric_input("Enter the key, a number between 1 and 25");
    char *capitalized_plaintext = malloc(MAXIMUM_INPUT_LENGTH);
    char *ciphertext = malloc(MAXIMUM_INPUT_LENGTH);
    capitalized_plaintext = plaintext;
    capitalized_plaintext = sentence_to_uppercase(capitalized_plaintext, plaintext);
    ciphertext = caesar_cipher(ciphertext, capitalized_plaintext, key);
    printf("Ciphertext: %s\n", ciphertext);
    struct cipher_package *package = malloc(sizeof(struct cipher_package));
    package->plaintext = capitalized_plaintext;
    package->ciphertext = ciphertext;
    package->key = key;
    package->sentence_length = (int)strlen(plaintext);
    printf("Plaintext: %s\n", plaintext);
    printf("Key: %d\n", key);
    printf("Length: %d\n", package->sentence_length);
    bool is_valid = validate_cipher(package);
    printf("Cipher package %s valid.\n", is_valid ? "is" : "is not");
    capitalized_plaintext = caesar_cipher(capitalized_plaintext, ciphertext, -key);
    printf("Deciphered plaintext: %s\n", capitalized_plaintext);
    return 0;
}

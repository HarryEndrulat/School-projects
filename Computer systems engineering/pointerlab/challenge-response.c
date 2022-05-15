/*
 * STUDENT: Harry Endrulat
 */

/*
 * PointerLab (c) 2021-22 Christopher A. Bohn
 */

/******************************************************************************
 * This program will implement the Pleistocene Petting Zoo's
 * challenge-and-response system.
 ******************************************************************************/

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "challenge-response.h"


/*** THE DATA STRUCTURE AND ITS ALGORITHMS ***/

/* Allocates memory for a new node to hold the word, and initializes the
 * node's fields. Returns a pointer to the new node. */
struct node *create_node(const char *word) {
    struct node *new_node = malloc(sizeof(struct node));
    strcpy(new_node->word,word);
    new_node->occurrences = 1;
    new_node->next = NULL;
    new_node->previous = NULL;
    /* WRITE THE REST OF THIS FUNCTION */
    return new_node;
}

/* Inserts new_node into the list after existing_node; that is, new_node
 * becomes existing_node's "next". If existing_node's original "next" is
 * non-NULL, then that will become new_node's "next". */
void insert_after(struct node *existing_node, struct node *new_node) {
    if (existing_node->next)
    {
        new_node->next = existing_node->next;
        new_node->previous=existing_node;
        new_node->next->previous=new_node;
        existing_node->next = new_node;
    }
    else
    {
        existing_node->next = new_node;
        new_node->previous=existing_node;
    }
    /* WRITE THIS FUNCTION */
}

/* Inserts new_node into the list before existing_node; that is, new_node
 * becomes existing_node's "previous". If existing_node's original "previous"
 * is non-NULL, then that will become new_node's "previous". */
void insert_before(struct node *existing_node, struct node *new_node) {
    /* WRITE THIS FUNCTION */
}


/*** ALPHABETIZE WORDS ***/

/* Returns a copy of the word that has all uppercase letters replaced
 * with lowercase letters. The original string is unchanged. */
char *word_to_lowercase(const char *word) {
    char *copy = malloc(strlen(word));
    strcpy(copy, word);
    for(int i=0; i<strlen(copy); i++)
    {
        copy[i]=decapitalize(copy[i]);
    }
    /* WRITE THIS FUNCTION */
    return copy;
}

/* Compares two words based on alphabetical order.
 * Returns a negative value if word1 occurs alphabetically before word2.
 * Returns a positive value if word1 occurs alphabetically after word2.
 * Returns 0 if the two words are identical.
 * This function is really just a wrapper around strncmp. */
int compare_words(const char *word1, const char *word2) {
    return strncmp(word1, word2, MAXIMUM_WORD_LENGTH);
}


/*** INSERTING WORDS ***/

/* Determines if the word is already in the list. If it is, then the
 * number of occurrences for that word is incremented. If it isn't, then
 * a new node is created for the word and inserted into the list at the
 * correct alphabetic location. Returns a pointer to the head of the
 * list, which is either the original head or a node containing the word
 * (if the word occurs before the original head's word or if the
 * original head is NULL). */
struct node *insert_word(struct node *head, const char *word) {
    //
    //iterate through the linked list checking word at each node using compare_words, keep track of previous
    //if the new word comes after current, check to see if this is the end of the list if not continue
    //if new word == current, increment occurrences
    //if new word comes before current, create a new node and call insert_after on the previous node
    //
    if(head==NULL)
    {
	    head=create_node(word);
        return head;
    }
    struct node *current = malloc(sizeof(struct node));
    current = head;
    while(current!=NULL)
    {
		if(compare_words(current->word, word)==0)
		{
			current->occurrences++;
		}
        else if(compare_words(current->word, word)<0 && current->next==NULL)
        {
	        insert_after(current, create_node(word));
        }
        current = current->next;
    }
    /* WRITE THIS FUNCTION */
    free(current);
    return head;
}

/* Given the name of the book file from the user, reads the file
 * line-by-line. Under the assumption that there is exactly one word per
 * line and that all punctuation has been removed, this function builds
 * a doubly-linked list of the words in alphabetical order, keeping
 * track (as part of a node's payload) how many times each word occurs
 * in the file. */
struct node *build_list(const char *filename) {
    struct node *head = malloc(sizeof(struct node));
    head=NULL;

    FILE *file = fopen(filename, "r");
    if(file==NULL)
    {
        perror("Error: no such file");
        return NULL;
    }
    char line[60];


    while(fgets(line, strlen(line), file))
    {
        printf("%s", line);
    	head = insert_word(head, word_to_lowercase(line));//still need to truncate new line characters
    }

    fclose(file);
    /* WRITE THE REST OF THIS FUNCTION */
    return head;
}


/*** RESPOND TO A CHALLENGE ***/

/* Given an alphabetically sorted list of words with the number of
 * occurrences of each word, and given the challenge_word, will return
 * the response word based on the following rules:
 * - If the number of occurrences is an even number then the response
 *   word is that many places *before* challenge_word in the list
 *   - If the challenge_word is fewer than that number of places from
 *     the head of the list, then the response word is the word at the
 *     head of the list
 * - If the number of occurrences is an odd number then the response
 *   word is that many places *after* challenge_word in the list
 *   - If the challenge_word is fewer than that number of places from
 *     the tail of the list, then the response word is the word at the
 *     tail of the list
 * - If challenge_word is not present in the list, then the response is
 *   "<challenge_word> is not present!" */
char *respond(const struct node *list, const char *challenge_word) {
    /* WRITE THIS FUNCTION */
    return NULL;
}

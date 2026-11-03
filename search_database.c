/***************************************************************************************************************************************************
*Author         : APSAR PINJARI
*Date           : 03 Nov 2026
*File           : search_database.c
*Title          : To search a word in the inverted search database.
*Description    : This file contains the function to search for a user-provided word in the inverted
                 : search database and display the files in which the word occurs along with its
                 : corresponding occurrence count.
                 :
                 : 1. INPUT WORD:
                 :    Accepts the word provided by the user and converts it into lowercase to ensure
                 :    case-insensitive searching.
                 :
                 : 2. HASH INDEX CALCULATION:
                 :    Calculates the hash table index based on the first character of the search word.
                 :    The word is searched only in the corresponding index of the 26-index hash table.
                 :
                 : 3. MAIN NODE TRAVERSAL:
                 :    Traverses the main node linked list at the calculated index and compares each
                 :    stored word with the search word.
                 :
                 : 4. SUB NODE TRAVERSAL:
                 :    If the word is found, traverses its corresponding sub-node linked list to retrieve
                 :    the filenames in which the word occurs and their respective word counts.
                 :
                 : 5. SEARCH RESULT:
                 :    Displays the searched word, the number of files containing the word, the filenames,
                 :    and the number of occurrences in each file.
                 :
                 : 6. WORD NOT FOUND:
                 :    If the word is not present in the database, displays an appropriate message and
                 :    returns DATA_NOT_FOUND.
                 :
                 : Returns SUCCESS when the word is found; otherwise, returns DATA_NOT_FOUND.
***************************************************************************************************************************************************/
#include "inverted_search.h"

int search_database(Main_node *hash_table[26], char *word)
{
    if (word == NULL || word[0] == '\0')
    {
        printf("Error: Please enter a valid word.\n");
        return FAILURE;
    }

    /* Convert the search word to lowercase */
    for (int i = 0; word[i] != '\0'; i++)
    {
        word[i] = (char)tolower((unsigned char)word[i]);
    }

    /* Calculate hash index */
    if (!isalpha((unsigned char)word[0]))
    {
        printf("Invalid word. Enter a word starting with a letter.\n");
        return FAILURE;
    }

    int index = word[0] - 'a';

    /* Search in the corresponding bucket */
    Main_node *main = hash_table[index];

    while (main != NULL)
    {
        if (strcmp(main->word, word) == 0)
        {
            printf("\nWord found!\n");
            printf("---------------------------------------------\n");
            printf("Index       : %d\n", index);
            printf("Word        : %s\n", main->word);
            printf("File Count  : %d\n", main->file_count);
            printf("---------------------------------------------\n");
            printf("%-20s %s\n", "File Name", "Word Count");

            Sub_node *sub = main->sub_link;

            while (sub != NULL)
            {
                printf("%-20s %d\n", sub->filename, sub->word_count);
                sub = sub->link;
            }

            return SUCCESS;
        }

        main = main->link;
    }

    printf("Word '%s' not found in the database.\n", word);

    return DATA_NOT_FOUND;
}

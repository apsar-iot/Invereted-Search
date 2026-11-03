
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

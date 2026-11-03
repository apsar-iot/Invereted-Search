/***************************************************************************************************************************************************
*Author         :Emertxe(http://www.emertxe.com)
*Date           :Mon 22 May 2017 14:04:05 IST
*File           :create_database.c
*Title          :To create the database
*Description    :The list of the files can be provided by storing all the file names in another file, FileList the names of the files which 
		:are to be documented are provided by this file. When a file is added or removed, FileList is changed accordingly.
		:So read the file names and start indexing.
****************************************************************************************************************************************************/

#include "inverted_search.h"

/* Create a new subnode */
static Sub_node *create_subnode(const char *filename)
{
    Sub_node *new = malloc(sizeof(Sub_node));

    if (new == NULL)
        return NULL;

    new->word_count = 1;

    snprintf(new->filename, sizeof(new->filename), "%s", filename);

    new->link = NULL;

    return new;
}

/* Create a new main node */
static Main_node *create_mainnode(const char *word, const char *filename)
{
    Main_node *new = malloc(sizeof(Main_node));

    if (new == NULL)
        return NULL;

    snprintf(new->word, sizeof(new->word), "%s", word);

    new->file_count = 1;
    new->link = NULL;

    new->sub_link = create_subnode(filename);

    if (new->sub_link == NULL)
    {
        free(new);
        return NULL;
    }

    return new;
}

/* Insert a word into the database */
static int insert_word(Main_node **head, const char *word,
                       const char *filename)
{
    Main_node *temp = *head;
    Main_node *prev = NULL;

    /* Search for the word in the bucket */
    while (temp != NULL)
    {
        if (strcmp(temp->word, word) == 0)
        {
            Sub_node *sub = temp->sub_link;

            /* Check whether the filename already exists */
            while (sub != NULL)
            {
                if (strcmp(sub->filename, filename) == 0)
                {
                    sub->word_count++;
                    return SUCCESS;
                }

                sub = sub->link;
            }

            /* Same word, new filename */
            Sub_node *new_sub = create_subnode(filename);

            if (new_sub == NULL)
                return FAILURE;

            new_sub->link = temp->sub_link;
            temp->sub_link = new_sub;
            temp->file_count++;

            return SUCCESS;
        }

        prev = temp;
        temp = temp->link;
    }

    /* Word not found: create a new main node */
    Main_node *new_main = create_mainnode(word, filename);

    if (new_main == NULL)
        return FAILURE;

    if (prev == NULL)
    {
        *head = new_main;
    }
    else
    {
        prev->link = new_main;
    }

    return SUCCESS;
}

/* Create the inverted database */
int create_database(Flist *f_head, Main_node *hash_table[26])
{
    if (f_head == NULL)
        return LIST_EMPTY;

    while (f_head != NULL)
    {
        FILE *fp = fopen(f_head->filename, "r");

        if (fp == NULL)
        {
            printf("Error: Cannot open file %s\n", f_head->filename);
            return FAILURE;
        }

        char word[50];

        while (fscanf(fp, "%49s", word) == 1)
        {
            /* Convert word to lowercase */
            for (int i = 0; word[i] != '\0'; i++)
            {
                word[i] = (char)tolower((unsigned char)word[i]);
            }

            /* Ignore words that do not start with a letter */
            if (!isalpha((unsigned char)word[0]))
                continue;

            /* Calculate hash index */
            int index = word[0] - 'a';

            if (index < 0 || index >= 26)
                continue;

            /* Insert word into the appropriate bucket */
            if (insert_word(&hash_table[index], word,
                            f_head->filename) == FAILURE)
            {
                fclose(fp);
                return FAILURE;
            }
        }

        fclose(fp);
        f_head = f_head->link;
    }

    printf("Database created successfully!\n");

    return SUCCESS;
}

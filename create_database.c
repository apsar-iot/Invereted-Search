/************************************************************************************************************
* Author         : APSAR PINJARI
* Date           : 03 Nov 2026
* File           : create_database.c
* Title          : To create the inverted search database
* Description    : This function is used to create an inverted search database using the list of input files.
*                  The file list contains the names of all input files that are to be indexed. Each file is
*                  opened and its contents are read word by word.
*
*                  1. HASH TABLE:
*                     The database consists of 26 hash table indexes (0 to 25), representing alphabets
*                     from 'a' to 'z'. Each word is stored at an index based on its first character.
*
*                  2. MAIN NODE:
*                     Each main node stores a unique word, the number of files in which the word occurs
*                     (file_count), a pointer to the next main node (link), and a pointer to its sub-node
*                     list (sub_link).
*
*                  3. SUB NODE:
*                     Each sub node stores the name of the file in which the word occurs, the number of
*                     times the word appears in that file (word_count), and a pointer to the next sub node.
*
*                  4. DATABASE CREATION:
*                     If a word is encountered for the first time, a new main node is created and inserted
*                     into the appropriate hash table index. A new sub node is also created to store the
*                     file name and its word count.
*
*                     If the word already exists in the database and belongs to the same file, the
*                     corresponding sub node's word_count is incremented.
*
*                     If the word already exists but is found in a different file, a new sub node is
*                     created for that file and the main node's file_count is incremented.
*
*                  5. LINKING:
*                     Main nodes are linked together at each hash table index. Similarly, sub nodes are
*                     linked to their corresponding main node to maintain file-wise word occurrence details.
*
*                  The final database provides an efficient structure for searching, displaying, and
*                  saving word occurrence information across multiple input files.
************************************************************************************************************/

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

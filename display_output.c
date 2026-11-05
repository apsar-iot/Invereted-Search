/***************************************************************************************************************************************************
*Author         : APSAR PINJARI
*Date           : 03 Nov 2026
*File           : display_output.c
*Title          : To display the inverted search database.
*Description    : The database created using the 26-index hash table contains unique words along with their
                 : corresponding file names and word occurrence counts. Each main node stores a unique word
                 : and the number of files in which that word is present. The sub-node linked list stores the
                 : file names and the number of occurrences of each word in the respective files.
                 :
                 : This function traverses all 26 hash table indexes and displays the database details in
                 : a tabular format, including the index, word, file count, file name, and word count.
                 :
                 : For each main node, the corresponding sub-node list is traversed to display all files
                 : containing that word. The index, word, and file count are displayed only once, while
                 : subsequent file names and their word counts are displayed in the following rows.
                 :
                 : If the database is empty, DATA_NOT_FOUND is returned; otherwise, SUCCESS is returned
                 : after successfully displaying the database.
***************************************************************************************************************************************************/

#include "inverted_search.h"

/* Common function to display or write the database */
static int print_database(FILE *fp, Main_node *hash_table[26])
{
    int empty = 1;
    int serial = 0;

    fprintf(fp, "\n%-10s %-15s %-12s %-15s %-12s\n",
            "[INDEX]", "WORD", "FILE COUNT",
            "FILE NAME", "WORD COUNT");

    fprintf(fp, "----------------------------------------------------------------\n");

    for (int i = 0; i < 26; i++)
    {
        Main_node *main = hash_table[i];

        if (main == NULL)
            continue;

        serial++;
        int first_index = 1;

        while (main != NULL)
        {
            empty = 0;

            Sub_node *sub = main->sub_link;
            int first = 1;

            while (sub != NULL)
            {
                if (first)
                {
                    if (first_index)
                    {
                        fprintf(fp, "%-10d %-15s %-12d %-15s %-12d\n",
                                serial,
                                main->word,
                                main->file_count,
                                sub->filename,
                                sub->word_count);

                        first_index = 0;
                    }
                    else
                    {
                        fprintf(fp, "%-10s %-15s %-12d %-15s %-12d\n",
                                "",
                                main->word,
                                main->file_count,
                                sub->filename,
                                sub->word_count);
                    }

                    first = 0;
                }
                else
                {
                    fprintf(fp, "%-10s %-15s %-12s %-15s %-12d\n",
                            "", "", "", sub->filename,
                            sub->word_count);
                }

                sub = sub->link;
            }

            main = main->link;
        }

        fprintf(fp, "\n");
    }

    if (empty)
    {
        fprintf(fp, "Database is empty!\n");
        return DATA_NOT_FOUND;
    }

    return SUCCESS;
}

/* Display database in terminal */
int display_database(Main_node *hash_table[26])
{
    return print_database(stdout, hash_table);
}

/* Automatically create or refresh database.txt */
int write_database_file(Main_node *hash_table[26])
{
    FILE *fp = fopen("database.txt", "w");

    if (fp == NULL)
    {
        printf("Error: Unable to create database.txt\n");
        return FAILURE;
    }

    int status = print_database(fp, hash_table);

    fclose(fp);

    printf("database.txt refreshed successfully!\n");

    /* File creation is successful even if database is empty */
    return SUCCESS;
}

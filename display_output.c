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

int display_database(Main_node *hash_table[26])
{
    int empty = 1;

    printf("\n%-10s %-15s %-12s %-15s %-12s\n",
           "[INDEX]", "WORD", "FILE COUNT",
           "FILE NAME", "WORD COUNT");

    printf("----------------------------------------------------------------\n");

    for (int i = 0; i < 26; i++)
    {
        Main_node *main = hash_table[i];

        while (main != NULL)
        {
            empty = 0;

            Sub_node *sub = main->sub_link;
            int first = 1;

            while (sub != NULL)
            {
                if (first)
                {
                    printf("%-10d %-15s %-12d %-15s %-12d\n",
                           i,
                           main->word,
                           main->file_count,
                           sub->filename,
                           sub->word_count);

                    first = 0;
                }
                else
                {
                    printf("%-10s %-15s %-12s %-15s %-12d\n",
                           "", "", "", sub->filename,
                           sub->word_count);
                }

                sub = sub->link;
            }

            printf("\n");
            main = main->link;
        }
    }

    if (empty)
    {
        printf("Database is empty!\n");
        return DATA_NOT_FOUND;
    }

    return SUCCESS;
}

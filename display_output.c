/***************************************************************************************************************************************************
*Author         :Emertxe(http://www.emertxe.com)
*Date           :Mon 22 May 2017 16:06:15 IST
*File           :display_output.c
*Title          :To display  the results.
*Description    :When all the words are searched the ouput list is formed. In this the file with max count
		:(representing maximum word match ) can be selected and displayed. For advanced searched,
		:(weightage of words stored in database linkedlist) can also be used. So the results will be more
		:accurate.
****************************************************************************************************************************************************/

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

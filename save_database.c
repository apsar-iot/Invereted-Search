
#include "inverted_search.h"

int save_database(Main_node *hash_table[26])
{
    FILE *fp = fopen("backup.txt", "w");

    if (fp == NULL)
    {
        printf("Error: Unable to create backup file.\n");
        return FAILURE;
    }

    /* Traverse each hash-table index */
    for (int i = 0; i < 26; i++)
    {
        Main_node *main = hash_table[i];

        if (main == NULL)
            continue;

        /* Write the hash-table index */
        fprintf(fp, "#%d\n", i);

        while (main != NULL)
        {
            /* Write word and file count */
            fprintf(fp, "%s;%d", main->word, main->file_count);

            /* Write filenames and their word counts */
            Sub_node *sub = main->sub_link;

            while (sub != NULL)
            {
                fprintf(fp, ";%s;%d",
                        sub->filename, sub->word_count);

                sub = sub->link;
            }

            fprintf(fp, "\n");

            main = main->link;
        }
    }

    fclose(fp);

    printf("Database saved successfully to backup.txt\n");

    return SUCCESS;
}

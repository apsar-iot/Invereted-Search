/***************************************************************************************************************************************************
*Author		:Emertxe(http://www.emertxe.com)
*Date		:Mon 22 May 2017 14:00:05 IST
*File		:main.c
*Title		:Driver function
*Description	:This function acts like the driver function for the project inverted search
****************************************************************************************************************************************************/

#include "inverted_search.h"

int main(int argc, char *argv[])
{
    Flist *f_head = NULL;
    Main_node *hash_table[26] = {NULL};

    int i;
    int option;
    char word[50];
    char filename[100];

    /* Step 1: Check command-line arguments */

    if (argc < 2)
    {
        printf("Error: Please provide input .txt files\n");
        printf("Usage: ./a.out file1.txt file2.txt ...\n");
        return FAILURE;
    }

    printf("Total input files: %d\n", argc - 1);

    /* Step 2: Validate input files */

    for (i = 1; argv[i] != NULL; i++)
    {
        /* Validate extension */
        if (validate_file(argv[i]) == FAILURE)
        {
            printf("Invalid extension: %s\n", argv[i]);
            continue;
        }

        /* Check file existence and empty file */
        if (check_empty_file(argv[i]) == FAILURE)
        {
            printf("File does not exist or is empty: %s\n", argv[i]);
            continue;
        }

        /* Check duplicate filename */
        if (check_duplicate(f_head, argv[i]) == SUCCESS)
        {
            printf("Duplicate file: %s\n", argv[i]);
            continue;
        }

        /* Insert filename into SLL */
        if (create_index_list(&f_head, argv[i]) == SUCCESS)
        {
            printf("File added successfully: %s\n", argv[i]);
        }
        else
        {
            printf("Failed to add file: %s\n", argv[i]);
        }
    }

    /* Step 3: Check whether any valid files exist */

    if (f_head == NULL)
    {
        printf("Error: No valid input files found\n");
        return FAILURE;
    }

    /* Step 4: Create the inverted database */

    if (create_database(f_head, hash_table) == FAILURE)
    {
        printf("Error: Database creation failed\n");
        return FAILURE;
    }

    printf("\nDatabase created successfully!\n");

    /* Step 5: Display menu */

    while (1)
    {
        printf("\n========== INVERTED SEARCH MENU ==========\n");
        printf("1. Display Database\n");
        printf("2. Search Word\n");
        printf("3. Save Database\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");

        if (scanf("%d", &option) != 1)
        {
            printf("Invalid input. Enter a number.\n");

            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF)
                ;

            continue;
        }

        switch (option)
        {
            case 1:
                display_database(hash_table);
                break;

            case 2:
                printf("Enter the word to search: ");

                if (scanf("%49s", word) == 1)
                {
                    search_database(hash_table, word);
                }
                else
                {
                    printf("Unable to read search word\n");
                }
                break;

            case 3:
                printf("Enter the filename to save: ");

                if (scanf("%99s", filename) == 1)
                {
                   save_database(hash_table);
                }
                else
                {
                    printf("Unable to read filename\n");
                }
                break;

            case 4:
                printf("Exiting Inverted Search...\n");
                return SUCCESS;

            default:
                printf("Invalid choice. Select 1 to 4.\n");
        }
    }

    return SUCCESS;
}

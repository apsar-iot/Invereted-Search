/***************************************************************************************************************************************************
*Author         : APSAR PINJARI
*Date           : 03 Nov 2026
*File           : main.c
*Title          : Driver function for the Inverted Search project.
*Description    : This file contains the main driver function that controls the execution of the Inverted
                 : Search application. It accepts multiple input text files through command-line arguments
                 : and performs the required validation before creating the database.
                 :
                 : 1. COMMAND-LINE ARGUMENT VALIDATION:
                 :    Checks whether the user has provided input filenames through command-line arguments.
                 :    If no files are provided, an error message and usage instructions are displayed.
                 :
                 : 2. INPUT FILE VALIDATION:
                 :    Validates each input file by checking its extension, existence, and file size.
                 :    Invalid, missing, or empty files are rejected.
                 :
                 : 3. DUPLICATE FILE CHECK:
                 :    Checks whether the same filename has already been added to the file linked list.
                 :    Duplicate files are skipped to avoid repeated indexing.
                 :
                 : 4. FILE LINKED LIST CREATION:
                 :    Creates a singly linked list using the Flist structure to maintain the names of
                 :    all valid input files.
                 :
                 : 5. DATABASE CREATION:
                 :    Creates the inverted search database using a 26-index hash table. Unique words,
                 :    file names, file counts, and word counts are maintained using main nodes and
                 :    sub nodes.
                 :
                 : 6. MENU-DRIVEN OPERATIONS:
                 :    Provides the user with the following operations:
                 :       1. Display Database - Displays all indexed words and their file-wise counts.
                 :       2. Search Word     - Searches for a given word in the database.
                 :       3. Save Database   - Saves the database into a backup file.
                 :       4. Exit            - Terminates the application.
                 :
                 : 7. INPUT HANDLING:
                 :    Validates menu input and handles invalid input to prevent unexpected program
                 :    termination.
                 :
                 : The main function coordinates all the modules of the project and manages the
                 : overall execution flow of the Inverted Search application.
***************************************************************************************************************************************************/
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

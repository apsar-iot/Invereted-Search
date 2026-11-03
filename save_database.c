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

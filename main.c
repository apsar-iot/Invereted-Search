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

/* Free temporary file linked list */
static void free_file_list(Flist *head)
{
    while (head != NULL)
    {
        Flist *temp = head;
        head = head->link;
        free(temp);
    }
}

/* Append newly processed files to the permanent file list */
static void append_file_list(Flist **head, Flist *new_files)
{
    if (*head == NULL)
    {
        *head = new_files;
        return;
    }

    Flist *temp = *head;

    while (temp->link != NULL)
        temp = temp->link;

    temp->link = new_files;
}

int main(void)
{
    Flist *f_head = NULL;
    Main_node *hash_table[26] = {NULL};

    int option;
    int file_count;
    char word[50];
    char filename[100];

    while (1)
    {
        printf("\n========== INVERTED SEARCH MENU ==========\n");
        printf("1. Create Database\n");
        printf("2. Display Database\n");
        printf("3. Update Database\n");
        printf("4. Search Word\n");
        printf("5. Save Database\n");
        printf("6. Exit\n");
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
            {
                Flist *new_files = NULL;

                printf("Enter number of files to add: ");

                if (scanf("%d", &file_count) != 1 ||
                    file_count <= 0)
                {
                    printf("Invalid number of files.\n");
                    free_file_list(new_files);
                    break;
                }

                for (int i = 0; i < file_count; i++)
                {
                    printf("Enter filename %d: ", i + 1);

                    if (scanf("%99s", filename) != 1)
                    {
                        printf("Unable to read filename.\n");
                        continue;
                    }

                    if (validate_file(filename) == FAILURE)
                    {
                        printf("Invalid file extension: %s\n", filename);
                        continue;
                    }

                    if (check_empty_file(filename) == FAILURE)
                    {
                        printf("File does not exist or is empty: %s\n",
                               filename);
                        continue;
                    }

                    if (check_duplicate(f_head, filename) == SUCCESS ||
                        check_duplicate(new_files, filename) == SUCCESS)
                    {
                        printf("Duplicate file: %s\n", filename);
                        continue;
                    }

                    if (create_index_list(&new_files, filename) == SUCCESS)
                    {
                        printf("File added to processing list: %s\n",
                               filename);
                    }
                    else
                    {
                        printf("Memory allocation failed.\n");
                    }
                }

                if (new_files == NULL)
                {
                    printf("No new valid files to process.\n");
                    break;
                }

                /* Add only new files to the existing database */
                if (create_database(new_files, hash_table) == SUCCESS)
                {
                    append_file_list(&f_head, new_files);
                    new_files = NULL;

                    printf("\nDatabase created/updated successfully!\n");

                    /* Automatically refresh readable database */
                    if (write_database_file(hash_table) == SUCCESS)
                        printf("database.txt refreshed successfully.\n");
                    else
                        printf("Warning: Could not refresh database.txt.\n");
                }
                else
                {
                    printf("Database creation failed.\n");
                }

                free_file_list(new_files);
                break;
            }

            case 2:
                display_database(hash_table);
                break;

            case 3:
                if (update_database(hash_table, "backup.txt") == SUCCESS)
                {
                    printf("Database restored successfully.\n");

                    write_database_file(hash_table);
                }
                break;

            case 4:
                printf("Enter the word to search: ");

                if (scanf("%49s", word) == 1)
                    search_database(hash_table, word);
                else
                    printf("Unable to read search word.\n");

                break;

            case 5:
                save_database(hash_table);
                break;

            case 6:
                printf("Exiting Inverted Search...\n");
                free_file_list(f_head);
                return SUCCESS;

            default:
                printf("Invalid choice. Select 1 to 6.\n");
        }
    }

    return SUCCESS;
}

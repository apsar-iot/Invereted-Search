/************************************************************************************************************
* Author         : APSAR PINJARI
* Date           : 03 Nov 2026
* File           : create_indexlist.c
* Title          : To create the file linked list
* Description    : This file contains the functions required to validate input files and create a linked
*                  list containing the names of all valid input files that are to be processed for
*                  inverted search database creation.
*
*                  1. FILE VALIDATION:
*                     Checks whether the given file exists and can be accessed successfully.
*
*                  2. EMPTY FILE CHECK:
*                     Verifies whether the input file contains any data. Empty files are rejected
*                     because they cannot contribute to database creation.
*
*                  3. DUPLICATE FILE CHECK:
*                     Checks whether the given file name is already present in the file linked list.
*                     This prevents the same file from being added more than once.
*
*                  4. FILE NODE CREATION:
*                     Allocates memory dynamically for a new file node and stores the file name
*                     in the filename field.
*
*                  5. LINKED LIST CREATION:
*                     Maintains a singly linked list using the Flist structure. Each node contains
*                     the file name and a pointer to the next node.
*
*                  6. NODE INSERTION:
*                     Inserts the newly created file node into the linked list and updates the
*                     head pointer when required.
*
*                  7. LIST DISPLAY:
*                     Displays all the valid input file names stored in the linked list.
*
*                  The created file linked list is used as the input for database creation, allowing
*                  the inverted search application to process multiple valid files.
************************************************************************************************************/

#include "inverted_search.h"

/* Check whether filename already exists */

int check_duplicate(Flist *head, char *filename)
{
    while (head != NULL)
    {
        if (strcmp(head->filename, filename) == 0)
        {
            return SUCCESS;
        }

        head = head->link;
    }

    return FAILURE;
}

/* Insert filename into the singly linked list */

int create_index_list(Flist **head, char *filename)
{
    Flist *new = malloc(sizeof(Flist));

    if (new == NULL)
    {
        return FAILURE;
    }

    strcpy(new->filename, filename);
    new->link = NULL;

    /* If list is empty */

    if (*head == NULL)
    {
        *head = new;
        return SUCCESS;
    }

    /* Traverse to the last node */

    Flist *temp = *head;

    while (temp->link != NULL)
    {
        temp = temp->link;
    }

    /* Insert at the end */

    temp->link = new;

    return SUCCESS;
}

/* Display the filenames */

void print_list(Flist *head)
{
    if (head == NULL)
    {
        printf("No input files available\n");
        return;
    }

    while (head != NULL)
    {
        printf("%s -> ", head->filename);
        head = head->link;
    }

    printf("NULL\n");
}

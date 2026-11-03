/***************************************************************************************************************************************************
*Author         :Emertxe(http://www.emertxe.com)
*Date           :Mon 22 May 2017 14:20:10 IST
*File           :create_indexlist
*Title          :To create the index list.
*Description    :Before starting 'search phase', open the Database File and read the word structure and store
		:it in a LinkedList. This helps to avoid the overhead of accessing files from file always will be
		:reduced. So open Database file in read mode, read each word& related files info, and store in a
		:LinkedList node. Read till the file is completely converted to LinkedList. Make sure the sorted
		:order is stil maintained.
****************************************************************************************************************************************************/


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

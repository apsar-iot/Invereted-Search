/***************************************************************************************************************************************************
*Author         :Emertxe(http://www.emertxe.com)
*Date           :Mon 22 May 2017 14:20:10 IST
*File           :input_from_user.c
*Title          :To read the input from the user.
*Description    :The search phase is going to begin. To search, collect the user input text. It should be
		:stored in a character string.
****************************************************************************************************************************************************/
#include "inverted_search.h"

// Validate .txt extension
int validate_file(char *filename)
{
    char *ext = strrchr(filename, '.');

    if (ext == NULL)
    {
        return FAILURE;
    }

    if (strcmp(ext, ".txt") != 0)
    {
        return FAILURE;
    }

    return SUCCESS;
}

// Check whether a filename already exists in the linked list


// Check whether the file exists and is empty
int check_empty_file(char *filename)
{
    FILE *fp = fopen(filename, "r");
    long size;

    if (fp == NULL)
    {
        return FAILURE;
    }

    fseek(fp, 0, SEEK_END);
    size = ftell(fp);

    fclose(fp);

    if (size <= 0)
    {
        return FAILURE;
    }

    return SUCCESS;
}


/***************************************************************************************************************************************************
*Author         : APSAR PINJARI
*Date           : 03 Nov 2026
*File           : input_from_user.c
*Title          : To validate the input files provided by the user.
*Description    : Before creating the inverted search database, the input files provided by the user must
                 : be validated to ensure that only valid text files are processed.
                 :
                 : 1. FILE EXTENSION VALIDATION:
                 :    Checks whether the given filename has a valid ".txt" extension using string
                 :    operations. If the extension is missing or does not match ".txt", FAILURE is returned.
                 :
                 : 2. FILE EXISTENCE VALIDATION:
                 :    Attempts to open the given file in read mode to verify whether the file exists
                 :    and is accessible.
                 :
                 : 3. EMPTY FILE VALIDATION:
                 :    Checks the size of the file by moving the file pointer to the end using fseek()
                 :    and retrieving its size using ftell(). If the file size is zero or less, the file
                 :    is considered empty and FAILURE is returned.
                 :
                 : 4. VALIDATION RESULT:
                 :    Returns SUCCESS when the file has a valid extension, exists, and contains data.
                 :    Otherwise, FAILURE is returned.
***************************************************************************************************************************************************/
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

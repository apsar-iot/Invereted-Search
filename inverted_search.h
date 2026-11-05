
#ifndef INVERTED_SEARCH_H
#define INVERTED_SEARCH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Return status macros */
#define SUCCESS 0
#define FAILURE -1
#define LIST_EMPTY -2
#define DATA_NOT_FOUND -3

/* Input filename linked list */
typedef struct file_node
{
    char filename[100];
    struct file_node *link;
} Flist;

/* Subnode: filename and word occurrence count */
typedef struct sub_node
{
    int word_count;
    char filename[100];
    struct sub_node *link;
} Sub_node;

/* Main node: unique word and its file list */
typedef struct main_node
{
    int file_count;
    char word[50];
    struct main_node *link;
    Sub_node *sub_link;
} Main_node;

/* Input file validation */
int validate_file(char *filename);
int check_empty_file(char *filename);

/* Filename linked list operations */
int check_duplicate(Flist *head, char *filename);
int create_index_list(Flist **head, char *filename);
void print_list(Flist *head);

/* Inverted database operations */
int create_database(Flist *f_head, Main_node *hash_table[26]);
int display_database(Main_node *hash_table[26]);
int search_database(Main_node *hash_table[26], char *word);

/* Database file operations */
int write_database_file(Main_node *hash_table[26]);
int save_database(Main_node *hash_table[26]);
int update_database(Main_node *hash_table[26], char *filename);

#endif

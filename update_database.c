
#include "inverted_search.h"

int update_database(Main_node *hash_table[26], char *filename)
{
    FILE *fp = fopen(filename, "r");

    if (fp == NULL)
    {
        printf("Error: Cannot open backup file.\n");
        return FAILURE;
    }

    char line[1024];
    int current_index = -1;
    int loaded = 0;

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        line[strcspn(line, "\n")] = '\0';

        if (line[0] == '\0')
            continue;

        /* Read hash-table index */
        if (line[0] == '#')
        {
            char extra;
            int index;

            if (sscanf(line, "#%d%c", &index, &extra) != 1 ||
                index < 0 || index >= 26)
            {
                printf("Error: Invalid backup index.\n");
                fclose(fp);
                return FAILURE;
            }

            current_index = index;
            continue;
        }

        if (current_index < 0)
        {
            printf("Error: Backup index is missing.\n");
            fclose(fp);
            return FAILURE;
        }

        /* Split line into fields */
        char *fields[202];
        int count = 0;

        char *token = strtok(line, ";");

        while (token != NULL && count < 202)
        {
            fields[count++] = token;
            token = strtok(NULL, ";");
        }

        /* word, file_count, filename, word_count ... */
        if (count < 4 || (count - 2) % 2 != 0)
        {
            printf("Error: Invalid backup record.\n");
            fclose(fp);
            return FAILURE;
        }

        char *word = fields[0];
        char *end;
        long file_count = strtol(fields[1], &end, 10);

        if (*fields[0] == '\0' || !isalpha((unsigned char)word[0]) ||
            end == fields[1] || *end != '\0' ||
            file_count <= 0 || file_count > 100000)
        {
            printf("Error: Invalid word or file count.\n");
            fclose(fp);
            return FAILURE;
        }

        for (int j = 0; word[j] != '\0'; j++)
            word[j] = (char)tolower((unsigned char)word[j]);

        if (word[0] - 'a' != current_index)
        {
            printf("Error: Word does not match its index.\n");
            fclose(fp);
            return FAILURE;
        }

        int index = current_index;
        Main_node *main = hash_table[index];

        while (main != NULL && strcmp(main->word, word) != 0)
            main = main->link;

        /* Avoid adding duplicate words from the backup */
        if (main != NULL)
        {
            printf("Error: Duplicate word in backup: %s\n", word);
            fclose(fp);
            return FAILURE;
        }

        /* Build the main node */
        Main_node *new_main = calloc(1, sizeof(Main_node));

        if (new_main == NULL)
        {
            fclose(fp);
            return FAILURE;
        }

        snprintf(new_main->word, sizeof(new_main->word), "%s", word);
        new_main->file_count = (int)file_count;

        Sub_node *sub_tail = NULL;

        for (int i = 2; i < count; i += 2)
        {
            char *filename_field = fields[i];
            char *count_field = fields[i + 1];
            long occurrences = strtol(count_field, &end, 10);

            if (filename_field[0] == '\0' ||
                strlen(filename_field) >= sizeof(((Sub_node *)0)->filename) ||
                end == count_field || *end != '\0' ||
                occurrences <= 0 || occurrences > 1000000000L)
            {
                while (new_main->sub_link != NULL)
                {
                    Sub_node *tmp = new_main->sub_link;
                    new_main->sub_link = tmp->link;
                    free(tmp);
                }

                free(new_main);
                fclose(fp);
                printf("Error: Invalid filename or word count.\n");
                return FAILURE;
            }

            /* Reject repeated filenames for the same word */
            for (Sub_node *check = new_main->sub_link;
                 check != NULL; check = check->link)
            {
                if (strcmp(check->filename, filename_field) == 0)
                {
                    while (new_main->sub_link != NULL)
                    {
                        Sub_node *tmp = new_main->sub_link;
                        new_main->sub_link = tmp->link;
                        free(tmp);
                    }

                    free(new_main);
                    fclose(fp);
                    printf("Error: Duplicate filename in backup.\n");
                    return FAILURE;
                }
            }

            Sub_node *new_sub = calloc(1, sizeof(Sub_node));

            if (new_sub == NULL)
            {
                while (new_main->sub_link != NULL)
                {
                    Sub_node *tmp = new_main->sub_link;
                    new_main->sub_link = tmp->link;
                    free(tmp);
                }

                free(new_main);
                fclose(fp);
                return FAILURE;
            }

            snprintf(new_sub->filename, sizeof(new_sub->filename),
                     "%s", filename_field);
            new_sub->word_count = (int)occurrences;

            if (new_main->sub_link == NULL)
                new_main->sub_link = new_sub;
            else
                sub_tail->link = new_sub;

            sub_tail = new_sub;
        }

        /* Verify that file_count matches the number of subnodes */
        int actual_files = 0;

        for (Sub_node *sub = new_main->sub_link;
             sub != NULL; sub = sub->link)
            actual_files++;

        if (actual_files != new_main->file_count)
        {
            while (new_main->sub_link != NULL)
            {
                Sub_node *tmp = new_main->sub_link;
                new_main->sub_link = tmp->link;
                free(tmp);
            }

            free(new_main);
            fclose(fp);
            printf("Error: File count does not match backup data.\n");
            return FAILURE;
        }

        /* Insert the main node into the selected bucket */
        Main_node **link = &hash_table[index];

        while (*link != NULL)
            link = &(*link)->link;

        *link = new_main;
        loaded++;
    }

    fclose(fp);

    if (loaded == 0)
    {
        printf("Error: File does not contain a valid database.\n");
        return FAILURE;
    }

    printf("Database loaded successfully from %s\n", filename);

    return SUCCESS;
}

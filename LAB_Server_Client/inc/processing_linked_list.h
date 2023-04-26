#ifndef __PROCESSING_LINKED_LIST__
#define __PROCESSING_LINKED_LIST__

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct Node
{
    int ID;
    float TEMP;
    time_t TIME;
    int DELETE;
    struct Node *next;
} Node;

void save_data_to_linked_list(Node **head, int ID, float TEMP, time_t TIME);
void sort_linked_list_by_ID_and_TIME(Node **head);
void averaged_temp_linked_list(Node *head);
void copy_data_from_linked_list_to_data_file(Node *head);
void remove_data_from_linked_list(Node **head);
void free_linked_list(Node *head);

#endif
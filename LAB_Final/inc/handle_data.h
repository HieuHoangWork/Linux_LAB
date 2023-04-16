#ifndef __HANDLE_DATA
#define __HANDLE_DATA

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

void data_save(Node **head, int ID, float TEMP, time_t TIME);
void sort_list_by_ID(Node **head);
void data_handle(Node *head);
void save_data_to_file(Node *head);
void remove_data(Node **head);
void free_linked_list(Node *head);

#endif
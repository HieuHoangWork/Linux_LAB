#include "processing_linked_list.h"
#include "processing_FIFO.h"

extern int log_sequence_number;

void save_data_to_linked_list(Node **head, int ID, float TEMP, time_t TIME)
{
    Node *new_node = (Node *)malloc(sizeof(Node));
    new_node->ID = ID;
    new_node->TEMP = TEMP;
    new_node->TIME = TIME;
    new_node->DELETE = 0;
    new_node->next = *head;
    *head = new_node;
}

void sort_linked_list_by_ID_and_TIME(Node **head)
{
    // Sắp xếp danh sách liên kết theo ID
    Node *temp = *head;
    while (temp != NULL)
    {
        Node *min = temp;
        Node *current = temp->next;
        while (current != NULL)
        {
            if (current->ID < min->ID)
            {
                min = current;
            }
            current = current->next;
        }
        if (min != temp)
        {
            // Hoán đổi giá trị giữa temp và min
            int temp_ID = temp->ID;
            float temp_TEMP = temp->TEMP;
            time_t temp_TIME = temp->TIME;
            int temp_DELETE = temp->DELETE;

            temp->ID = min->ID;
            temp->TEMP = min->TEMP;
            temp->TIME = min->TIME;
            temp->DELETE = min->DELETE;

            min->ID = temp_ID;
            min->TEMP = temp_TEMP;
            min->TIME = temp_TIME;
            min->DELETE = temp_DELETE;
        }
        temp = temp->next;
    }

    // Sắp xếp các bản tin có cùng ID theo thời gian TIME tăng dần
    temp = *head;
    while (temp != NULL)
    {
        Node *current = temp;
        while (current != NULL && current->ID == temp->ID)
        {
            Node *min_TIME = current;
            Node *inner = current->next;
            while (inner != NULL && inner->ID == temp->ID)
            {
                if (inner->TIME < min_TIME->TIME)
                {
                    min_TIME = inner;
                }
                inner = inner->next;
            }
            if (min_TIME != current)
            {
                // Hoán đổi giá trị giữa current và min_TIME
                int temp_ID = current->ID;
                float temp_TEMP = current->TEMP;
                time_t temp_TIME = current->TIME;
                int temp_DELETE = current->DELETE;

                current->ID = min_TIME->ID;
                current->TEMP = min_TIME->TEMP;
                current->TIME = min_TIME->TIME;
                current->DELETE = min_TIME->DELETE;

                min_TIME->ID = temp_ID;
                min_TIME->TEMP = temp_TEMP;
                min_TIME->TIME = temp_TIME;
                min_TIME->DELETE = temp_DELETE;
            }
            current = current->next;
        }
        temp = current;
    }
}

void averaged_temp_linked_list(Node *head)
{
    Node *temp = head;
    while (temp != NULL)
    {
        int processed_count = 0; // Biến để đếm số lượng bản tin đã xử lý trong nhóm có cùng ID
        float sum_TEMP = 0;
        time_t sum_TIME = 0;
        Node *current = temp;
        while (current != NULL && current->ID == temp->ID)
        {
            // Kiểm tra nếu số lượng bản tin đã xử lý chưa vượt quá 5
            if (processed_count < 5)
            {
                sum_TEMP += current->TEMP;
                sum_TIME += current->TIME;
                processed_count++; // Tăng số lượng bản tin đã xử lý
            }
            current = current->next;
        }
        if (processed_count == 5)
        {
            if (sum_TEMP / processed_count > 16)
            {
                printf("ID%d/%.2f/%ld - TOO HOT\n", temp->ID, sum_TEMP / processed_count, sum_TIME / processed_count);
                char log_event[BUFFER_SIZE];
                sprintf(log_event, "%d ID%d/%.2f/%ld - TOO HOT\n", log_sequence_number++, temp->ID, sum_TEMP / processed_count, sum_TIME / processed_count);
                write_to_FIFO(FIFO_PATH, log_event);
            }
            else if (sum_TEMP / processed_count < 14)
            {
                printf("ID%d/%.2f/%ld - TOO COLD\n", temp->ID, sum_TEMP / processed_count, sum_TIME / processed_count);
                char log_event[BUFFER_SIZE];
                sprintf(log_event, "%d ID%d/%.2f/%ld - TOO COLD\n", log_sequence_number++, temp->ID, sum_TEMP / processed_count, sum_TIME / processed_count);
                write_to_FIFO(FIFO_PATH, log_event);
            }
            else
            {
                printf("ID%d/%.2f/%ld - NORMAL\n", temp->ID, sum_TEMP / processed_count, sum_TIME / processed_count);
                char log_event[BUFFER_SIZE];
                sprintf(log_event, "%d ID%d/%.2f/%ld - NORMAL\n", log_sequence_number++, temp->ID, sum_TEMP / processed_count, sum_TIME / processed_count);
                write_to_FIFO(FIFO_PATH, log_event);
            }

            temp->DELETE = 1;
        }
        temp = current;
    }
}

void copy_data_from_linked_list_to_data_file(Node *head)
{
    FILE *file;
    file = fopen("../tmp/data.txt", "a");
    if (file == NULL)
    {
        printf("fopen() data.txt failed\n");
        return;
    }

    Node *temp = head;
    while (temp != NULL)
    {
        if (temp->DELETE == 1)
        {
            char buffer[128];
            sprintf(buffer, "ID%d/%.2f/%ld\n", temp->ID, temp->TEMP, (long)temp->TIME);
            fprintf(file, "%s", buffer);
        }
        temp = temp->next;
    }

    fclose(file);
}

void remove_data_from_linked_list(Node **head)
{
    Node *temp = *head;
    Node *prev = NULL;
    while (temp != NULL)
    {
        if (temp->DELETE == 1)
        {
            if (prev == NULL)
            {
                *head = temp->next;
                free(temp);
                temp = *head;
            }
            else
            {
                prev->next = temp->next;
                free(temp);
                temp = prev->next;
            }
        }
        else
        {
            prev = temp;
            temp = temp->next;
        }
    }
}

void free_linked_list(Node *head)
{
    Node *temp;
    while (head != NULL)
    {
        temp = head;
        head = head->next;
        free(temp);
    }
}
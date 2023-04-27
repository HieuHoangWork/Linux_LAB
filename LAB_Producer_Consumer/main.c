#include <stdlib.h>    // standard stuff
#include <sys/mman.h>  // mmap()
#include <stdio.h>     // io stuff
#include <unistd.h>    // sleep()
#include <semaphore.h> // semaphore()
#include <time.h>      // time()

#define BUFFER_SIZE 10
#define Fettuccine_Chicken_Alfredo 11
#define Garlic_Sirloin_Steak 12
#define Pistachio_Pesto_Pasta 21
#define Avocado_Fruit_Salad 22

void write_to_buffer(int *buffer, int buffer_size, int data);
int read_from_buffer(int *buffer, int buffer_size);

int main()
{
    /* Create shared memory */
    int *buffer_Vegan = (int *)mmap(NULL, sizeof(int) * BUFFER_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    int *buffer__NON_Vegan = (int *)mmap(NULL, sizeof(int) * BUFFER_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    /* Create shared semaphores */
    sem_t *mutex_Vegan = (sem_t *)mmap(NULL, sizeof(sem_t *), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    sem_t *full_Vegan = (sem_t *)mmap(NULL, sizeof(sem_t *), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    sem_t *empty_Vegan = (sem_t *)mmap(NULL, sizeof(sem_t *), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    sem_t *mutex__NON_Vegan = (sem_t *)mmap(NULL, sizeof(sem_t *), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    sem_t *full__NON_Vegan = (sem_t *)mmap(NULL, sizeof(sem_t *), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    sem_t *empty__NON_Vegan = (sem_t *)mmap(NULL, sizeof(sem_t *), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    sem_init(mutex_Vegan, 1, 1);
    sem_init(empty_Vegan, 1, BUFFER_SIZE);
    sem_init(full_Vegan, 1, 0);

    sem_init(mutex__NON_Vegan, 1, 1);
    sem_init(empty__NON_Vegan, 1, BUFFER_SIZE);
    sem_init(full__NON_Vegan, 1, 0);

    /* Forking */
    pid_t producer_Vegan;
    pid_t producer__NON_Vegan;
    pid_t consumer_Vegan;
    pid_t consumer__NON_Vegan;
    pid_t consumer_Both;

    /* Child producer_Vegan process */
    if ((producer_Vegan = fork()) == 0)
    {
        srand(time(NULL));
        unsigned int seed = rand();
        while (1)
        {
            sem_wait(empty_Vegan);
            sem_wait(mutex_Vegan);

            /* Creating meal _NON_Vegan */
            int random_meal_Vegan = rand_r(&seed);
            if (random_meal_Vegan % 2 == 0)
            {
                write_to_buffer(buffer_Vegan, BUFFER_SIZE, Fettuccine_Chicken_Alfredo);
                printf("producer_Vegan creating Fettuccine_Chicken_Alfredo\n");
            }
            else
            {
                write_to_buffer(buffer_Vegan, BUFFER_SIZE, Garlic_Sirloin_Steak);
                printf("producer_Vegan creating Garlic_Sirloin_Steak\n");
            }

            sem_post(full_Vegan);
            sem_post(mutex_Vegan);

            /* Sleep between 1 and 5 seconds */
            sleep(1 + rand_r(&seed) % 5);
        }
    }

    /* Child producer__NON_Vegan process */
    if ((producer__NON_Vegan = fork()) == 0)
    {
        srand(time(NULL));
        unsigned int seed = rand();
        while (1)
        {
            sem_wait(empty__NON_Vegan);
            sem_wait(mutex__NON_Vegan);

            /* Creating meal Vegan */
            int random_meal__NON_Vegan = rand_r(&seed);
            if (random_meal__NON_Vegan % 2 == 0)
            {
                write_to_buffer(buffer__NON_Vegan, BUFFER_SIZE, Pistachio_Pesto_Pasta);
                printf("producer__NON_Vegan creating Pistachio_Pesto_Pasta\n");
            }
            else
            {
                write_to_buffer(buffer__NON_Vegan, BUFFER_SIZE, Avocado_Fruit_Salad);
                printf("producer__NON_Vegan creating Avocado_Fruit_Salad\n");
            }

            sem_post(full__NON_Vegan);
            sem_post(mutex__NON_Vegan);

            /* Sleep between 1 and 5 seconds */
            sleep(1 + rand_r(&seed) % 5);
        }
    }

    /* Child consumer__NON_Vegan process */
    if ((consumer__NON_Vegan = fork()) == 0)
    {
        srand(time(NULL));
        unsigned int seed = rand();
        while (1)
        {
            sem_wait(full__NON_Vegan);
            sem_wait(mutex__NON_Vegan);

            /* Take meal from bufffer Vegan*/
            int read__NON_Vegan = read_from_buffer(buffer__NON_Vegan, BUFFER_SIZE);
            if (read__NON_Vegan == Pistachio_Pesto_Pasta)
            {
                printf("consumer__NON_Vegan take Pistachio_Pesto_Pasta\n");
            }
            else if (read__NON_Vegan == Avocado_Fruit_Salad)
            {
                printf("consumer__NON_Vegan take Avocado_Fruit_Salad\n");
            }

            sem_post(mutex__NON_Vegan);
            sem_post(empty__NON_Vegan);
            /* Sleep between 10 and 15 seconds */
            sleep(10 + rand_r(&seed) % 6);
        }
    }

    /* Child consumer_Vegan process */
    if ((consumer_Vegan = fork()) == 0)
    {
        srand(time(NULL));
        unsigned int seed = rand();
        while (1)
        {
            sem_wait(full_Vegan);
            sem_wait(mutex_Vegan);

            /* Take meal from bufffer _NON_Vegan*/
            int read_Vegan = read_from_buffer(buffer_Vegan, BUFFER_SIZE);
            if (read_Vegan == Fettuccine_Chicken_Alfredo)
            {
                printf("consumer_Vegan take Fettuccine_Chicken_Alfredo\n");
            }
            else if (read_Vegan == Garlic_Sirloin_Steak)
            {
                printf("consumer_Vegan take Garlic_Sirloin_Steak\n");
            }

            sem_post(mutex_Vegan);
            sem_post(empty_Vegan);
            /* Sleep between 10 and 15 seconds */
            sleep(10 + rand_r(&seed) % 6);
        }
    }

    /* Child consumer_Both process */
    if ((consumer_Both = fork()) == 0)
    {
        srand(time(NULL));
        unsigned int seed = rand();
        while (1)
        {
            struct timespec timeout;
            clock_gettime(CLOCK_REALTIME, &timeout);
            timeout.tv_sec += 3;

            int result_Vegan = sem_timedwait(full_Vegan, &timeout);
            if (result_Vegan == 0)
            {
                sem_wait(mutex_Vegan);

                /* Take meal from buffer Vegan */
                int read_Vegan = read_from_buffer(buffer_Vegan, BUFFER_SIZE);
                if (read_Vegan == Fettuccine_Chicken_Alfredo)
                {
                    printf("consumer_Both take Fettuccine_Chicken_Alfredo\n");
                }
                else if (read_Vegan == Garlic_Sirloin_Steak)
                {
                    printf("consumer_Both take Garlic_Sirloin_Steak\n");
                }

                sem_post(mutex_Vegan);
                sem_post(empty_Vegan);
            }
            else
            {
                clock_gettime(CLOCK_REALTIME, &timeout);
                timeout.tv_sec += 3;

                int result__NON_Vegan = sem_timedwait(full__NON_Vegan, &timeout);
                if (result__NON_Vegan == 0)
                {
                    sem_wait(mutex__NON_Vegan);

                    /* Take meal from buffer _NON_Vegan */
                    int read__NON_Vegan = read_from_buffer(buffer__NON_Vegan, BUFFER_SIZE);
                    if (read__NON_Vegan == Pistachio_Pesto_Pasta)
                    {
                        printf("consumer_Both take Pistachio_Pesto_Pasta\n");
                    }
                    else if (read__NON_Vegan == Avocado_Fruit_Salad)
                    {
                        printf("consumer_Both take Avocado_Fruit_Salad\n");
                    }

                    sem_post(mutex__NON_Vegan);
                    sem_post(empty__NON_Vegan);
                }
            }

            /* Sleep between 10 and 15 seconds */
            sleep(10 + rand_r(&seed) % 6);
        }
    }

    /* Parent */
    else
    {
        while (1)
        {
            sleep(10);
            int takenSlots_Vegan;
            int takenSlots__NON_Vegan;
            sem_getvalue(full_Vegan, &takenSlots_Vegan);
            sem_getvalue(full__NON_Vegan, &takenSlots__NON_Vegan);
            printf("\nItems in Vegan dish: %d/%d    Items in NON_Vegane dish: %d/%d\n", takenSlots_Vegan, BUFFER_SIZE, takenSlots__NON_Vegan, BUFFER_SIZE);
        }
    }
}

void write_to_buffer(int *buffer, int buffer_size, int data)
{
    for (int i = 0; i < buffer_size; i++)
    {
        if (buffer[i] == 0)
        {
            buffer[i] = data;
            break;
        }
    }
}

int read_from_buffer(int *buffer, int buffer_size)
{
    for (int i = 0; i < buffer_size; i++)
    {
        if (buffer[i] != 0)
        {
            int value = buffer[i];
            buffer[i] = 0;
            return value;
        }
    }
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t *chopstick;
int num_eat_times;
int num_philosophers;

void *philos(void *);
void eat(int);

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: %s <number of philosophers> <number of times to eat>\n", argv[0]);
        return 1;
    }

    num_philosophers = atoi(argv[1]);
    num_eat_times = atoi(argv[2]);

    chopstick = (sem_t *)malloc(num_philosophers * sizeof(sem_t));
    pthread_t *T = (pthread_t *)malloc(num_philosophers * sizeof(pthread_t));

    // Initialize semaphores for chopsticks
    for (int i = 0; i < num_philosophers; i++)
        sem_init(&chopstick[i], 0, 1);

    // Create threads for each philosopher
    for (int i = 0; i < num_philosophers; i++)
    {
        int *ph_id = (int *)malloc(sizeof(int));
        *ph_id = i;
        pthread_create(&T[i], NULL, philos, (void *)ph_id);
    }

    // Join threads
    for (int i = 0; i < num_philosophers; i++)
        pthread_join(T[i], NULL);

    // Clean up
    free(chopstick);
    free(T);

    return 0;
}

void *philos(void *arg)
{
    int ph = *((int *)arg);
    for (int i = 0; i < num_eat_times; i++)
    {
        printf("Philosopher %d wants to eat\n", ph);
        if (ph == 0)
        {
            printf("Philosopher %d tries to pick right chopstick\n", ph);
            sem_wait(&chopstick[(ph + 1) % num_philosophers]);
            printf("Philosopher %d picks the right chopstick\n", ph);
            printf("Philosopher %d tries to pick left chopstick\n", ph);
            sem_wait(&chopstick[ph]);
            printf("Philosopher %d picks the left chopstick\n", ph);
        }
        else
        {
            printf("Philosopher %d tries to pick left chopstick\n", ph);
            sem_wait(&chopstick[ph]);
            printf("Philosopher %d picks the left chopstick\n", ph);
            printf("Philosopher %d tries to pick right chopstick\n", ph);
            sem_wait(&chopstick[(ph + 1) % num_philosophers]);
            printf("Philosopher %d picks the right chopstick\n", ph);
        }
        eat(ph);
        sleep(2);
        printf("Philosopher %d has finished eating\n", ph);
        sem_post(&chopstick[(ph + 1) % num_philosophers]);
        printf("Philosopher %d leaves the right chopstick\n", ph);
        sem_post(&chopstick[ph]);
        printf("Philosopher %d leaves the left chopstick\n", ph);
    }
    return NULL;
}

void eat(int ph)
{
    printf("Philosopher %d begins to eat\n", ph);
}

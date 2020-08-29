/*
 * Alyxandra Spikerman
 * High Perfomance Computing
 * Homework 3 - Question 2
 *
 * Using Unix semaphores and Posix pthreads, implement a C/C++ program that
 * counts the number of numbers between 1-1000 that are evenly divisible by 3 or 4. You
 * will use 4 threads, where you will sequence through each thread to perform both
 * checking and printing. At the end, you will print out the number of numbers that were
 * divisible. Make sure you use pthreads for generating your 4 threads, and semaphores
 * for synchronization.
 *
 */

#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>
#include <cstdint>

using namespace std;

// GLOBAL VARIABLES
int NUM_THREADS = 4;
int MAX_NUM = 1000;
sem_t sems[4];
int counts[4];

/*
 * get_next_sem:
 * - next semaphore is the current semaphore + 1
 *
 * @current_sem: current semaphore
 *
 */
int get_next_sem(int current_sem) {
    int next_sem = current_sem + 1;
    if (current_sem == NUM_THREADS - 1) {
        next_sem = 0; // last semaphores next sem is the first sem
    }
    return next_sem;
}

/*
 * check_and_print
 * - check from 1 - 1000 and print numebrs that are divisible by 3 or 4, and add them to the count of total
 *   numbers divisible by 3 or 4
 */
void *check_and_print(void *arg) {
    uint64_t thread_num = (uint64_t)arg;

    for (int i = 1; i <= 1000; i++) {
        if (i % 4 == 0 || i % 3 == 0) {
            sem_wait(&sems[thread_num]); // wait until this semaphore is signaled
            cout << "sem " << thread_num << " : " << i << endl;
            counts[thread_num]++;
            sem_post(&sems[get_next_sem(thread_num)]); // signal the next semaphore
        }
    }

    return NULL;
}

/*
 * main
 * - creates threads and initializes semaphores, joins threads and destroys semaphores
 */
int main(int argc, char* argv[]) {
    int sem_init_val; // the value to initialize the semaphores

    // intialize the semaphores
    for (int i = 0; i < NUM_THREADS; i++) {
        counts[i] = 0; // intiialize counts
        if (i == 0) {
            sem_init_val = 1; // we want the first thread to print first
        } else {
            sem_init_val = 0;
        }
        sem_init(&sems[i], 0, sem_init_val);
    }

    // declare pthread variables
    pthread_t threads[NUM_THREADS];
    uint64_t args[NUM_THREADS];
    int pthread_return;

    for (int i = 0; i < NUM_THREADS; i++) {
        // create the thread and pass in the thread number
        args[i] = i;
        pthread_return = pthread_create(&threads[i], NULL, &check_and_print, (void *)args[i]);
        if (pthread_return) {
            cout << "Error: cannot create thread, " << pthread_return << endl;
            return 1;
        }
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_return = pthread_join(threads[i], NULL); // join the threads
        if (pthread_return) {
            cout << "Error: cannot join thread, " << pthread_return << endl;
            return 1;
        }
    }

    // destroy the semaphores & print values for the number of numbers divisible by 3 or 4
    for (int i = 0; i < NUM_THREADS; i++) {
        cout << "sem " << i << " has " << counts[i] << " numbers divisible by 3 or 4" << endl;
        sem_destroy(&sems[i]);
    }

    return 0;
}

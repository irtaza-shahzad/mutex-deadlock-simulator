#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define resource_count 5
#define thread_count 4

int resource_arr[resource_count];
pthread_mutex_t mutex_arr[resource_count];

void *thread1_func() {
  printf("[Thread 1]: Trying to lock R1\n");
  pthread_mutex_lock(&mutex_arr[0]);  // lock R1

  sleep(1);

  printf("[Thread 1]: Successfully locked R1!\n");

  printf("[Thread 1]: Trying to lock R2\n");
  pthread_mutex_lock(&mutex_arr[1]);  // lock R2
  printf("[Thread 1]: Successfully locked R2!\n");

  printf("[Thread 1]: Trying to lock R3\n");
  pthread_mutex_lock(&mutex_arr[2]);  // lock R3
  printf("[Thread 1]: Successfully Locked R3!\n\n");

  int result = resource_arr[0] + resource_arr[1] + resource_arr[2];  // cs
  printf("Thread 1: (R1 + R2) + R3 = (%d + %d) + %d = %d\n\n", resource_arr[0],
         resource_arr[1], resource_arr[2], result);

  pthread_mutex_unlock(&mutex_arr[2]);  // unlock R3
  printf("[Thread 1]: Successfully Unlocked R3!\n");

  pthread_mutex_unlock(&mutex_arr[1]);  // unlock R2
  printf("[Thread 1]: Successfully Unlocked R2!\n");

  pthread_mutex_unlock(&mutex_arr[0]);  // unlock R1
  printf("[Thread 1]: Successfully Unlocked R1!\n");
  printf("\n[Thread 1]: COMPLETED EXECUTION!\n\n");

  return NULL;
}

void *thread2_func() {
  printf("[Thread 2]: Trying to lock R2\n");
  pthread_mutex_lock(&mutex_arr[1]);  // lock R2

  sleep(1);

  printf("[Thread 2]: Locked R2 Successfully!\n");

  printf("[Thread 2]: Trying to lock R3\n");
  pthread_mutex_lock(&mutex_arr[2]);  // lock R3
  printf("[Thread 2]: Locked R3 Successfully!\n");

  printf("[Thread 2]: Trying to lock R4\n");
  pthread_mutex_lock(&mutex_arr[3]);  // lock R4

  printf("[Thread 2]: Locked R4 Successfully!\n\n");

  int result = resource_arr[1] - resource_arr[2] - resource_arr[3];  // cs
  printf("Thread 2: (R2 - R3) - R4 = (%d - %d) - %d = %d\n\n", resource_arr[1],
         resource_arr[2], resource_arr[3], result);

  pthread_mutex_unlock(&mutex_arr[3]);  // unlock R4
  printf("[Thread 2]: Unlocked R4 Successfully!\n");

  pthread_mutex_unlock(&mutex_arr[2]);  // unlock R3
  printf("[Thread 2]: Unlocked R3 Successfully!\n");

  pthread_mutex_unlock(&mutex_arr[1]);  // unlock R2
  printf("[Thread 2]: Unlocked R2 Successfully!\n");
  printf("\n[Thread 2]: COMPLETED EXECUTION!\n\n");

  return NULL;
}

void *thread3_func() {
  printf("[Thread 3]: Trying to lock R3\n");
  pthread_mutex_lock(&mutex_arr[2]);  // lock R3

  sleep(1);

  printf("[Thread 3]: Locked R3 Successfully!\n");

  printf("[Thread 3]: Trying to lock R4\n");
  pthread_mutex_lock(&mutex_arr[3]);  // lock R4

  printf("[Thread 3]: Locked R4 Successfully!\n");

  printf("[Thread 3]: Trying to lock R5\n");
  pthread_mutex_lock(&mutex_arr[4]);  // lock R5
  printf("[Thread 3]: Locked R5 Successfully!\n\n");

  int result = resource_arr[2] * resource_arr[3] * resource_arr[4];  // cs
  printf("Thread 3: (R3 * R4) * R5 = (%d * %d) * %d = %d\n\n", resource_arr[2],
         resource_arr[3], resource_arr[4], result);

  pthread_mutex_unlock(&mutex_arr[4]);  // unlock R5
  printf("[Thread 3]: Unlocked R5 Successfully!\n");

  pthread_mutex_unlock(&mutex_arr[3]);  // unlock R4
  printf("[Thread 3]: Unlocked R4 Successfully!\n");

  pthread_mutex_unlock(&mutex_arr[2]);  // unlock R3
  printf("[Thread 3]: Unlocked R3 Successfully!\n");
  printf("\n[Thread 3]: COMPLETED EXECUTION!\n\n");

  return NULL;
}

void *thread4_func() {
  printf("[Thread 4]: Trying to lock R4\n");
  pthread_mutex_lock(&mutex_arr[3]);  // lock R4

  sleep(1);

  printf("[Thread 4]: Locked R4 Successfully!\n");

  printf("[Thread 4]: Trying to lock R5\n");
  pthread_mutex_lock(&mutex_arr[4]);  // lock R5
  printf("[Thread 4]: Locked R5 Successfully!\n");

  printf("[Thread 4]: Trying to lock R1\n");
  pthread_mutex_lock(&mutex_arr[0]);  // lock R1
  printf("[Thread 4]: Locked R1 Successfully!\n\n");

  int result = resource_arr[3] / resource_arr[4] / resource_arr[0];  // cs
  printf("Thread 4: (R4 / R5) / R1 = (%d / %d) / %d = %d\n\n", resource_arr[3],
         resource_arr[4], resource_arr[0], result);

  pthread_mutex_unlock(&mutex_arr[0]);  // unlock R1
  printf("[Thread 4]: Unlocked R1 Successfully!\n");

  pthread_mutex_unlock(&mutex_arr[4]);  // unlock R5
  printf("[Thread 4]: Unlocked R5 Successfully!\n");

  pthread_mutex_unlock(&mutex_arr[3]);  // unlock R4
  printf("[Thread 4]: Unlocked R4 Successfully!\n");
  printf("\n[Thread 4]: COMPLETED EXECUTION!\n\n");

  return NULL;
}

void *deadlock_func() {
  sleep(5);
  printf("\n[WARNING]: Deadlock Deteced!\nExiting...\n\n");
  exit(EXIT_FAILURE);
}

int main() {
  srand(time(0));
  pthread_t thread_arr[thread_count];

  for (int i = 0; i < resource_count; i++) {
    resource_arr[i] = (rand() % 20) + 1;
    pthread_mutex_init(&mutex_arr[i], NULL);
  }

  printf("\nShared Resources: \n");
  for (int i = 0; i < resource_count; i++) {
    printf("R%d = %d", i + 1, resource_arr[i]);

    if (i + 1 < resource_count) printf(", ");
  }
  printf("\n\n");

  pthread_create(&thread_arr[0], NULL, thread1_func, NULL);
  pthread_create(&thread_arr[1], NULL, thread2_func, NULL);
  pthread_create(&thread_arr[2], NULL, thread3_func, NULL);
  pthread_create(&thread_arr[3], NULL, thread4_func, NULL);

  pthread_t check_deadlock;
  pthread_create(&check_deadlock, NULL, deadlock_func, NULL);

  for (int i = 0; i < thread_count; i++) {
    pthread_join(thread_arr[i], NULL);
  }

  for (int i = 0; i < resource_count; i++) {
    pthread_mutex_destroy(&mutex_arr[i]);
  }

  return 0;
}
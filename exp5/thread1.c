#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h> // For sleep function

int* array;
int sum = 0;
int array_size;
pthread_mutex_t sum_mutex; // Mutex for synchronizing access to sum

// Function to be executed by the first thread
void* thread_function1(void* arg) {
    for (int i = 0; i < array_size / 2; i++) {
        pthread_mutex_lock(&sum_mutex);
        sum += array[i];
        printf("Thread 1: adding %d, partial sum = %d\n", array[i], sum);
        pthread_mutex_unlock(&sum_mutex);
        usleep(100000); // Adding delay to simulate longer processing (100ms)
    }
    pthread_exit(NULL);
}

// Function to be executed by the second thread
void* thread_function2(void* arg) {
    for (int i = array_size / 2; i < array_size; i++) {
        pthread_mutex_lock(&sum_mutex);
        sum += array[i];
        printf("Thread 2: adding %d, partial sum = %d\n", array[i], sum);
        pthread_mutex_unlock(&sum_mutex);
        usleep(100000); // Adding delay to simulate longer processing (100ms)
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t thread1, thread2;
    pthread_mutex_init(&sum_mutex, NULL); // Initialize the mutex

    // Reading the size of the array from the keyboard
    printf("Enter the size of the array: ");
    scanf("%d", &array_size);

    // Allocating memory for the array
    array = (int*)malloc(array_size * sizeof(int));
    if (array == NULL) {
        perror("Failed to allocate memory");
        return 1;
    }

    // Reading array elements from the keyboard
    printf("Enter %d integers: ", array_size);
    for (int i = 0; i < array_size; i++) {
        scanf("%d", &array[i]);
    }

    // Creating the first thread
    if (pthread_create(&thread1, NULL, thread_function1, NULL) != 0) {
        perror("Failed to create thread 1");
        free(array);
        return 1;
    }

    // Creating the second thread
    if (pthread_create(&thread2, NULL, thread_function2, NULL) != 0) {
        perror("Failed to create thread 2");
        free(array);
        return 1;
    }

    // Waiting for both threads to finish
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    printf("The total sum of the array elements is: %d\n", sum);

    // Freeing allocated memory and destroying the mutex
    free(array);
    pthread_mutex_destroy(&sum_mutex);

    return 0;
}

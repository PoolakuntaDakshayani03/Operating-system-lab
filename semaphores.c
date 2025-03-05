xx#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>  // For O_CREAT
#include <sys/stat.h>

#define MAX_PORTS 5  // Maximum number of ports allowed
sem_t *port_semaphore; // Semaphore to control port access

void* open_port(void* arg) {
    sem_wait(port_semaphore); // Wait if no port is available
    printf("Port opened by thread %ld\n", (long)arg);
    
    // Simulate port usage
    sleep(2);
    
    printf("Port closed by thread %ld\n", (long)arg);
    sem_post(port_semaphore); // Release the port
    return NULL;
}

int main() {
    pthread_t threads[10];

    // Create a named semaphore
    port_semaphore = sem_open("/my_semaphore", O_CREAT, 0644, MAX_PORTS);
    if (port_semaphore == SEM_FAILED) {
        perror("sem_open failed");
        return 1;
    }

    for (long i = 0; i < 10; i++) {
        pthread_create(&threads[i], NULL, open_port, (void*)i);
    }

    for (int i = 0; i < 10; i++) {
        pthread_join(threads[i], NULL);
    }

    // Clean up the semaphore
    sem_close(port_semaphore);
    sem_unlink("/my_semaphore"); // Unlink named semaphore

    return 0;
}


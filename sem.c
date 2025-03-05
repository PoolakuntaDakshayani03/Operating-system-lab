#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>  // For O_CREAT flag
#include <sys/stat.h>

#define MAX_PORTS 5  
#define SEM_NAME "/port_semaphore"

sem_t *port_semaphore;

void* open_port(void* arg) {
    sem_wait(port_semaphore); 
    printf("Port opened by thread %ld\n", (long)arg);
    
    sleep(2);  // Simulate port usage
    
    printf("Port closed by thread %ld\n", (long)arg);
    sem_post(port_semaphore);
    return NULL;
}

int main() {
    pthread_t threads[10];

    // Open a named semaphore
    port_semaphore = sem_open(SEM_NAME, O_CREAT, 0644, MAX_PORTS);
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

    // Close and unlink the named semaphore
    sem_close(port_semaphore);
    sem_unlink(SEM_NAME);

    return 0;
}



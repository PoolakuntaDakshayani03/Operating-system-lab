# Control the number of ports opened systems with a) Semephore b) Monitor
# Prgram description
# ✅ Semaphores, Monitors, and Ports in Operating Systems

These are key concepts in **process synchronization** and **resource management** in operating systems. Let's break them down with examples and comparisons. 

---

## 1️⃣ Semaphores

### 🔍 Definition:
A **semaphore** is a synchronization primitive used to control access to a shared resource in a concurrent system. It’s essentially an **integer variable** that supports two atomic operations:
- **`wait()` (also called `P()` or `down`)**: Decreases the semaphore value. If the value is **0**, the process waits until it becomes positive.
- **`signal()` (also called `V()` or `up`)**: Increases the semaphore value, potentially waking up waiting processes.

### 🚦 Types of Semaphores:
1. **Counting Semaphore:** Can take any non-negative integer value. Useful for managing multiple identical resources (like ports).
2. **Binary Semaphore (Mutex):** Takes only `0` or `1`—used for mutual exclusion.

### ⚙️ Key Takeaways:
- Controls access to shared resources.
- Ensures synchronization in concurrent systems.

---

## 2️⃣ Monitors

### 🔍 Definition:
A **monitor** is a high-level synchronization construct that manages access to shared resources. It combines:
- **Mutual Exclusion:** Only one process can execute inside the monitor at a time.
- **Condition Variables:** Allow threads to **wait** (using `wait()`) and **signal** others when conditions change (using `signal()` or `notify()`).

### 🧰 Structure of a Monitor:
- **Shared Variables:** Represent the resource (e.g., available ports).
- **Mutex (Lock):** To ensure exclusive access.
- **Condition Variables:** To manage waiting threads.

### ⚙️ Key Takeaways:
- Ensures mutual exclusion and condition synchronization.
- Simplifies complex process synchronization.

---

## 3️⃣ Ports

### 🔍 Definition:
In computing, a **port** is a communication endpoint used for data exchange between:
- Processes (Inter-Process Communication)
- Devices
- Networks (TCP/UDP ports in networking)

### 🚪 Types of Ports:
1. **Hardware Ports:** Physical connection interfaces (USB, HDMI).
2. **Software Ports:** Logical endpoints used for network communication (like port 80 for HTTP).

### 📊 Role of Ports in Resource Management:
- Ports are **finite resources** (limited number).
- OS must manage ports efficiently to avoid conflicts (e.g., two apps can’t bind to the same TCP port).

---

## 🚀 Semaphores vs. Monitors vs. Ports

| **Aspect**       | **Semaphore**                    | **Monitor**                        | **Port**                           |
|------------------|---------------------------------|------------------------------------|------------------------------------|
| **Purpose**       | Low-level synchronization       | High-level synchronization         | Endpoint for communication         |
| **Type**          | Integer with `wait/signal` ops  | Object with locks & condition vars | Resource (hardware/software)        |
| **Concurrency**   | Controls resource count         | Ensures mutual exclusion + waiting | Manages data flow between entities  |
| **Blocking**      | Manual handling with semaphores | Built-in with `wait()`/`signal()`   | Depends on system/network settings  |
| **OS Example**    | Process synchronization         | Thread-safe access to shared data  | Network socket or device interface  |

---

## 💡 Real-World Example:

Imagine **5 people** trying to use **3 charging ports** at an airport:

- **Semaphore:** A token system where each person grabs a token to charge. If no tokens are left, they wait.
- **Monitor:** A manager at the charging station allows one person at a time to check if a port is free. If not, the person waits in line, and the manager signals the next when a port frees up.
- **Port:** The actual charging slot used to plug in the charger.

---


# Source Code a) Semaphore
```c
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX_PORTS 5  // Maximum number of ports allowed
sem_t port_semaphore; // Semaphore to control port access

void* open_port(void* arg) {
    sem_wait(&port_semaphore); // Wait if no port is available
    printf("Port opened by thread %ld\n", (long)arg);
    
    // Simulate port usage
    sleep(2);
    
    printf("Port closed by thread %ld\n", (long)arg);
    sem_post(&port_semaphore); // Release the port
    return NULL;
}

int main() {
    pthread_t threads[10];
    sem_init(&port_semaphore, 0, MAX_PORTS); // Initialize semaphore with MAX_PORTS

    for (long i = 0; i < 10; i++) {
        pthread_create(&threads[i], NULL, open_port, (void*)i);
    }

    for (int i = 0; i < 10; i++) {
        pthread_join(threads[i], NULL);
    }

    sem_destroy(&port_semaphore);
    return 0;
}
```
# Output
![Semsphore output](ops1.png)

# b) Source Code for Monitors
```c
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>  // For sleep()

#define MAX_OPEN_PORTS 5  // Define the maximum number of allowed open ports

// Monitor structure to manage ports
typedef struct {
    int available_ports;
    pthread_mutex_t mutex;
    pthread_cond_t condition;
} PortMonitor;

// Function to initialize the monitor
void init_monitor(PortMonitor* monitor) {
    monitor->available_ports = MAX_OPEN_PORTS;
    pthread_mutex_init(&monitor->mutex, NULL);
    pthread_cond_init(&monitor->condition, NULL);
}

// Function to open a port (monitor-like behavior)
void open_port(PortMonitor* monitor, int thread_id) {
    pthread_mutex_lock(&monitor->mutex);  // Enter critical section

    while (monitor->available_ports == 0) {
        pthread_cond_wait(&monitor->condition, &monitor->mutex);  // Wait if no ports are available
    }
    monitor->available_ports--;  // Allocate a port
    printf("Port opened by thread %d\n", thread_id);

    pthread_mutex_unlock(&monitor->mutex);  // Exit critical section
}

// Function to close a port (monitor-like behavior)
void close_port(PortMonitor* monitor, int thread_id) {
    pthread_mutex_lock(&monitor->mutex);  // Enter critical section

    monitor->available_ports++;  // Release the port
    printf("Port closed by thread %d\n", thread_id);
    pthread_cond_signal(&monitor->condition);  // Notify waiting threads

    pthread_mutex_unlock(&monitor->mutex);  // Exit critical section
}

// Thread function
void* port_handler(void* arg) {
    int thread_id = *(int*)arg;
    static PortMonitor monitor = {0};  // Shared monitor instance

    // Initialize monitor only once (for the first thread)
    if (monitor.available_ports == 0) {
        init_monitor(&monitor);
    }

    open_port(&monitor, thread_id);
    sleep(2);  // Simulate port usage
    close_port(&monitor, thread_id);

    return NULL;
}

int main() {
    pthread_t threads[10];
    int thread_ids[10];

    for (int i = 0; i < 10; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, port_handler, &thread_ids[i]);
    }

    for (int i = 0; i < 10; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
```
![monitor output](opm1.png)

# Interpretion of the Program


# Program Interpretation: Semaphore-based Port Management

This C program uses semaphores to manage access to a fixed number of ports. It creates 10 threads, each representing a request to access a port. The semaphore ensures that no more than 5 threads can access the ports concurrently. Let's break down the program step by step:

## 1. Header Files and Macro Definition

```c
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX_PORTS 5  // Maximum number of ports allowed
```
- **`#include <stdio.h>`**: Includes the standard I/O library for printing to the console.
- **`#include <unistd.h>`**: Includes the Unix standard library for using functions like `sleep()`.
- **`#include <pthread.h>`**: Includes the POSIX thread library for thread management.
- **`#include <semaphore.h>`**: Includes the semaphore library for managing synchronization.
- **`#define MAX_PORTS 5`**: Defines the maximum number of ports that can be accessed at the same time (5 in this case).

## 2. Semaphore Declaration

```c
sem_t port_semaphore; // Semaphore to control port access
```

- **`sem_t port_semaphore;`**: Declares a semaphore variable to control access to the ports.

## 3. Thread Function: `open_port`

```c
void* open_port(void* arg) {
    sem_wait(&port_semaphore); // Wait if no port is available
    printf("Port opened by thread %ld
", (long)arg);
    
    // Simulate port usage
    sleep(2);
    
    printf("Port closed by thread %ld
", (long)arg);
    sem_post(&port_semaphore); // Release the port
    return NULL;
}
```

- **`sem_wait(&port_semaphore);`**: The thread waits if the semaphore value is 0 (i.e., no ports available). This will block the thread until a port is free.
- **`printf("Port opened by thread %ld
", (long)arg);`**: Prints the thread's ID (using `arg`) when it successfully opens the port.
- **`sleep(2);`**: Simulates the thread using the port for 2 seconds.
- **`printf("Port closed by thread %ld
", (long)arg);`**: Prints the thread's ID when it finishes using the port and closes it.
- **`sem_post(&port_semaphore);`**: The thread releases the port by incrementing the semaphore value, allowing another thread to access it.

## 4. Main Function

```c
int main() {
    pthread_t threads[10];
    sem_init(&port_semaphore, 0, MAX_PORTS); // Initialize semaphore with MAX_PORTS

    for (long i = 0; i < 10; i++) {
        pthread_create(&threads[i], NULL, open_port, (void*)i);
    }

    for (int i = 0; i < 10; i++) {
        pthread_join(threads[i], NULL);
    }

    sem_destroy(&port_semaphore);
    return 0;
}
```

- **`pthread_t threads[10];`**: Declares an array of 10 threads.
- **`sem_init(&port_semaphore, 0, MAX_PORTS);`**: Initializes the semaphore to allow up to 5 threads (ports) to access concurrently.
- **`for (long i = 0; i < 10; i++) {`**:
  - Loops 10 times to create 10 threads, each representing a request to open a port.
  - **`pthread_create(&threads[i], NULL, open_port, (void*)i);`**: Creates a new thread that runs the `open_port` function, passing the thread index `i` as an argument.
- **`for (int i = 0; i < 10; i++) {`**:
  - Loops to join each thread, ensuring the main thread waits for all threads to finish before terminating.
  - **`pthread_join(threads[i], NULL);`**: Waits for the thread `i` to complete its execution.
- **`sem_destroy(&port_semaphore);`**: Destroys the semaphore after all threads have completed to release resources.

## 5. Program Output

The program will print the following types of messages:
- **`Port opened by thread <thread_id>`**: When a thread successfully opens a port.
- **`Port closed by thread <thread_id>`**: When the thread finishes using the port and releases it.

Due to the semaphore control, no more than 5 threads can access the ports simultaneously. If there are more threads than available ports, the excess threads will block until a port is freed.

## 6. Execution Flow

1. Semaphore is initialized with a value of 5 (allowing up to 5 threads to access ports).
2. 10 threads are created, each trying to open a port.
3. As threads reach `sem_wait`, they either proceed or wait if no port is available.
4. After using the port for 2 seconds, the thread releases the port (`sem_post`), and another waiting thread can proceed.
5. The main thread waits for all threads to finish using `pthread_join`.
6. Semaphore is destroyed at the end.

## 7. Possible Output Example

```
Port opened by thread 0
Port opened by thread 1
Port opened by thread 2
Port opened by thread 3
Port opened by thread 4
Port closed by thread 0
Port opened by thread 5
Port opened by thread 6
Port opened by thread 7
Port opened by thread 8
Port closed by thread 1
Port closed by thread 2
Port closed by thread 3
Port closed by thread 4
Port closed by thread 5
Port opened by thread 9
Port closed by thread 6
Port closed by thread 7
Port closed by thread 8
Port closed by thread 9
```

In this example, only 5 threads can open ports simultaneously, and other threads have to wait for ports to become available.

## Conclusion

This program demonstrates how semaphores can be used to control concurrent access to shared resources (ports). It is a basic example of synchronization in multithreaded environments, ensuring that no more than a specified number of threads can access a resource at once.



# Program Interpretation: Port Management with Mutex and Condition Variables

This C program simulates managing a limited number of ports using multiple threads. It uses a monitor-like structure, mutexes, and condition variables for synchronization. Below is the detailed explanation of how the program works.

## 1. Header Files and Definitions

```c
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>  // For sleep()

#define MAX_OPEN_PORTS 5  // Define the maximum number of allowed open ports
```

- **`#include <stdio.h>`**: Includes the standard I/O library to handle printing.
- **`#include <pthread.h>`**: Includes the POSIX threads library to enable multithreading.
- **`#include <unistd.h>`**: Includes the standard library providing the `sleep()` function for simulating port usage time.
- **`#define MAX_OPEN_PORTS 5`**: Defines the maximum number of ports that can be concurrently open at any time (5 in this case).

## 2. PortMonitor Structure

```c
typedef struct {
    int available_ports;
    pthread_mutex_t mutex;
    pthread_cond_t condition;
} PortMonitor;
```

- **`PortMonitor` structure**: 
  - **`available_ports`**: Holds the count of available ports.
  - **`mutex`**: A mutex for locking critical sections and preventing race conditions.
  - **`condition`**: A condition variable used for synchronization when no ports are available.

## 3. Monitor Initialization Function

```c
void init_monitor(PortMonitor* monitor) {
    monitor->available_ports = MAX_OPEN_PORTS;
    pthread_mutex_init(&monitor->mutex, NULL);
    pthread_cond_init(&monitor->condition, NULL);
}
```

- **`init_monitor` function**: 
  - Initializes the monitor with the maximum available ports.
  - Initializes the mutex and the condition variable to manage synchronization between threads.

## 4. Open Port Function

```c
void open_port(PortMonitor* monitor, int thread_id) {
    pthread_mutex_lock(&monitor->mutex);  // Enter critical section

    while (monitor->available_ports == 0) {
        pthread_cond_wait(&monitor->condition, &monitor->mutex);  // Wait if no ports are available
    }
    monitor->available_ports--;  // Allocate a port
    printf("Port opened by thread %d
", thread_id);

    pthread_mutex_unlock(&monitor->mutex);  // Exit critical section
}
```

- **`open_port` function**: 
  - The thread first locks the mutex to ensure thread-safe access to shared resources.
  - If no ports are available (`available_ports == 0`), it waits using `pthread_cond_wait`.
  - Once a port becomes available, the thread decreases the `available_ports` count and prints that it has opened the port.
  - Finally, the mutex is unlocked to allow other threads to access the shared resource.

## 5. Close Port Function

```c
void close_port(PortMonitor* monitor, int thread_id) {
    pthread_mutex_lock(&monitor->mutex);  // Enter critical section

    monitor->available_ports++;  // Release the port
    printf("Port closed by thread %d
", thread_id);
    pthread_cond_signal(&monitor->condition);  // Notify waiting threads

    pthread_mutex_unlock(&monitor->mutex);  // Exit critical section
}
```

- **`close_port` function**: 
  - The thread locks the mutex to enter the critical section.
  - It releases a port by incrementing the `available_ports` count.
  - A message is printed indicating that the port was closed.
  - `pthread_cond_signal` is used to notify one of the waiting threads that a port is now available.
  - The mutex is unlocked, allowing other threads to proceed.

## 6. Thread Handler Function

```c
void* port_handler(void* arg) {
    int thread_id = *(int*)arg;
    static PortMonitor monitor = {0};  // Shared monitor instance

    // Initialize monitor only once (for the first thread)
    if (monitor.available_ports == 0) {
        init_monitor(&monitor);
    }

    open_port(&monitor, thread_id);
    sleep(2);  // Simulate port usage
    close_port(&monitor, thread_id);

    return NULL;
}
```

- **`port_handler` function**: 
  - Each thread receives a `thread_id` from the argument and uses it to identify the thread.
  - A shared `PortMonitor` instance is used across all threads. It is initialized by the first thread to manage the ports.
  - The thread attempts to open a port, simulates usage for 2 seconds, and then closes the port.
  - This demonstrates how the threads interact with the shared monitor to manage access to the ports.

## 7. Main Function

```c
int main() {
    pthread_t threads[10];
    int thread_ids[10];

    for (int i = 0; i < 10; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, port_handler, &thread_ids[i]);
    }

    for (int i = 0; i < 10; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
```

- **`main` function**: 
  - Declares arrays for 10 threads (`threads[10]`) and their respective IDs (`thread_ids[10]`).
  - A loop creates 10 threads using `pthread_create`, where each thread will execute the `port_handler` function.
  - After all threads are created, the main thread waits for each of them to complete using `pthread_join`.
  - Once all threads have finished, the program terminates.

## 8. Execution Flow

1. **Monitor Initialization**: The first thread initializes the monitor, which includes setting up the number of available ports and initializing the mutex and condition variable.
2. **Port Allocation**: Each thread attempts to open a port. If no ports are available, it waits until a port becomes available.
3. **Port Usage**: After opening a port, the thread simulates using it by sleeping for 2 seconds.
4. **Port Release**: The thread then closes the port, and one of the waiting threads can be notified and proceed to open the port.
5. **Synchronization**: The mutex and condition variable ensure that no more than 5 threads can open ports at the same time, and the condition variable coordinates which threads should wait or proceed.

## 9. Example Output

```
Port opened by thread 0
Port opened by thread 1
Port opened by thread 2
Port opened by thread 3
Port opened by thread 4
Port closed by thread 0
Port opened by thread 5
Port opened by thread 6
Port opened by thread 7
Port opened by thread 8
Port closed by thread 1
Port closed by thread 2
Port closed by thread 3
Port closed by thread 4
Port closed by thread 5
Port opened by thread 9
Port closed by thread 6
Port closed by thread 7
Port closed by thread 8
Port closed by thread 9
```

- In this example, only 5 threads can open ports at the same time. Any additional threads have to wait for a port to become available.

## 10. Conclusion

This program demonstrates how to use a monitor-like structure with mutexes and condition variables to synchronize access to a shared resource (ports). The program ensures that no more than a specified number of threads can use the ports concurrently, and it effectively manages the threads' access to the shared ports.

By using synchronization mechanisms, the program guarantees that the threads interact safely and efficiently without encountering race conditions.

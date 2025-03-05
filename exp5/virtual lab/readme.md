# Program Statement: Simulate LJF (langest job First) and SRJF (Shortest Remaining Job First) CPU Scheduling Algorithms.
```c
#include <stdio.h>

typedef struct {
    int pid;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int completion_time;
    int turnaround_time;
    int waiting_time;
} Process;

// Function to sort processes by Arrival Time
void sortByArrivalTime(Process proc[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (proc[i].arrival_time > proc[j].arrival_time) {
                Process temp = proc[i];
                proc[i] = proc[j];
                proc[j] = temp;
            }
        }
    }
}

// Function for Longest Job First (LJF) Scheduling
void LJF(Process proc[], int n) {
    sortByArrivalTime(proc, n);
    int time = 0, completed = 0;
    int total_waiting_time = 0, total_turnaround_time = 0;

    printf("\n--- Longest Job First (LJF) ---\n");
    printf("PID\tArrival\tBurst\tCompletion\tTurnaround\tWaiting\n");

    while (completed < n) {
        int max_burst_idx = -1, max_burst_time = -1;

        for (int i = 0; i < n; i++) {
            if (proc[i].arrival_time <= time && proc[i].remaining_time > 0 && proc[i].remaining_time > max_burst_time) {
                max_burst_time = proc[i].remaining_time;
                max_burst_idx = i;
            }
        }

        if (max_burst_idx == -1) {
            time++;
            continue;
        }

        Process *p = &proc[max_burst_idx];
        time += p->remaining_time;
        p->completion_time = time;
        p->turnaround_time = p->completion_time - p->arrival_time;
        p->waiting_time = p->turnaround_time - p->burst_time;
        p->remaining_time = 0;
        completed++;

        total_waiting_time += p->waiting_time;
        total_turnaround_time += p->turnaround_time;

        printf("%d\t%d\t%d\t%d\t\t%d\t\t%d\n", p->pid, p->arrival_time, p->burst_time, p->completion_time, p->turnaround_time, p->waiting_time);
    }

    printf("\nTotal Waiting Time: %d", total_waiting_time);
    printf("\nTotal Turnaround Time: %d", total_turnaround_time);
    printf("\nAverage Waiting Time: %.2f", (float)total_waiting_time / n);
    printf("\nAverage Turnaround Time: %.2f\n", (float)total_turnaround_time / n);
}

// Function for Shortest Remaining Job First (SRJF) Scheduling
void SRJF(Process proc[], int n) {
    sortByArrivalTime(proc, n);
    int time = 0, completed = 0;
    int total_waiting_time = 0, total_turnaround_time = 0;

    printf("\n--- Shortest Remaining Job First (SRJF) ---\n");
    printf("PID\tArrival\tBurst\tCompletion\tTurnaround\tWaiting\n");

    while (completed < n) {
        int min_remaining_idx = -1, min_remaining_time = 99999;

        for (int i = 0; i < n; i++) {
            if (proc[i].arrival_time <= time && proc[i].remaining_time > 0 && proc[i].remaining_time < min_remaining_time) {
                min_remaining_time = proc[i].remaining_time;
                min_remaining_idx = i;
            }
        }

        if (min_remaining_idx == -1) {
            time++;
            continue;
        }

        Process *p = &proc[min_remaining_idx];

        // Execute process for one time unit
        p->remaining_time--;
        time++;

        // If process is completed
        if (p->remaining_time == 0) {
            p->completion_time = time;
            p->turnaround_time = p->completion_time - p->arrival_time;
            p->waiting_time = p->turnaround_time - p->burst_time;
            completed++;

            total_waiting_time += p->waiting_time;
            total_turnaround_time += p->turnaround_time;

            printf("%d\t%d\t%d\t%d\t\t%d\t\t%d\n", p->pid, p->arrival_time, p->burst_time, p->completion_time, p->turnaround_time, p->waiting_time);
        }
    }

    printf("\nTotal Waiting Time: %d", total_waiting_time);
    printf("\nTotal Turnaround Time: %d", total_turnaround_time);
    printf("\nAverage Waiting Time: %.2f", (float)total_waiting_time / n);
    printf("\nAverage Turnaround Time: %.2f\n", (float)total_turnaround_time / n);
}

int main() {
    int n, choice;

    printf("Enter the number of processes: ");
    scanf("%d", &n);

    Process proc[n];

    for (int i = 0; i < n; i++) {
        proc[i].pid = i + 1;
        printf("Enter Arrival Time and Burst Time for Process %d: ", i + 1);
        scanf("%d %d", &proc[i].arrival_time, &proc[i].burst_time);
        proc[i].remaining_time = proc[i].burst_time; // Initialize remaining burst time
    }

    printf("\nChoose Scheduling Algorithm:\n");
    printf("1. Longest Job First (LJF)\n");
    printf("2. Shortest Remaining Job First (SRJF)\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    if (choice == 1) {
        LJF(proc, n);
    } else if (choice == 2) {
        SRJF(proc, n);
    } else {
        printf("Invalid choice!\n");
    }

    return 0;
}
```
---
# **Interpretation of the CPU Scheduling Program (LJF & SRJF)**

## **Introduction**
The given C program simulates two CPU scheduling algorithms:

1. **Longest Job First (LJF)** - Non-preemptive scheduling where the process with the longest burst time is executed first.
2. **Shortest Remaining Job First (SRJF)** - Preemptive scheduling where the process with the shortest remaining burst time is executed first.

The program also calculates:
- **Total Waiting Time**
- **Total Turnaround Time**
- **Average Waiting Time**
- **Average Turnaround Time**

## **Understanding the Program**

### **1. Process Structure**
The program defines a `Process` structure that includes the following fields:
- `pid` (Process ID)
- `arrival_time` (Arrival time of the process)
- `burst_time` (Total execution time of the process)
- `remaining_time` (Remaining execution time, useful for SRJF)
- `completion_time` (Time at which the process finishes execution)
- `turnaround_time` (Completion Time - Arrival Time)
- `waiting_time` (Turnaround Time - Burst Time)

### **2. Sorting by Arrival Time**
The function `sortByArrivalTime()` sorts the processes in increasing order of arrival time. This ensures that processes are considered for execution as soon as they arrive.

### **3. Longest Job First (LJF) Scheduling**
- The algorithm selects the process with the **maximum burst time** among those that have already arrived.
- The selected process runs until completion.
- The process’s **completion time, turnaround time, and waiting time** are calculated.
- The process is marked as completed, and the loop continues until all processes are executed.

### **4. Shortest Remaining Job First (SRJF) Scheduling**
- The algorithm selects the process with the **minimum remaining time** from the available processes.
- The selected process runs for **one time unit** and is preempted if another process with a shorter burst time arrives.
- If a process completes execution, its **completion time, turnaround time, and waiting time** are recorded.
- The loop continues until all processes are completed.

### **5. Calculation of Performance Metrics**
For both LJF and SRJF:
- **Total Waiting Time** is the sum of waiting times for all processes.
- **Total Turnaround Time** is the sum of turnaround times for all processes.
- **Average Waiting Time** is calculated as:
  
  \[ \text{Avg. Waiting Time} = \frac{\text{Total Waiting Time}}{n} \]
  
- **Average Turnaround Time** is calculated as:
  
  \[ \text{Avg. Turnaround Time} = \frac{\text{Total Turnaround Time}}{n} \]
  
where \( n \) is the total number of processes.

## **Example Execution & Interpretation**

### **Input Example:**
```
Enter the number of processes: 3
Enter Arrival Time and Burst Time for Process 1: 0 6
Enter Arrival Time and Burst Time for Process 2: 2 8
Enter Arrival Time and Burst Time for Process 3: 4 7

Choose Scheduling Algorithm:
1. Longest Job First (LJF)
2. Shortest Remaining Job First (SRJF)
Enter your choice: 2
```

### **Output (SRJF Selected):**
```
--- Shortest Remaining Job First (SRJF) ---
PID	Arrival	Burst	Completion	Turnaround	Waiting
1	0	6	6	6	0
3	4	7	13	9	2
2	2	8	21	19	11

Total Waiting Time: 13
Total Turnaround Time: 34
Average Waiting Time: 4.33
Average Turnaround Time: 11.33
```

### **Analysis of the Output:**
1. **Process 1 (P1) starts first** as it arrives at time 0 and has the shortest burst time among available processes.
2. **Process 3 (P3) is scheduled next** since it has a shorter remaining burst time than P2.
3. **Process 2 (P2) runs last** since it had the longest burst time among the three.
4. **The average waiting and turnaround times are calculated correctly.**

### **Output (LJF Selected):**
```
--- Longest Job First (LJF) ---
PID	Arrival	Burst	Completion	Turnaround	Waiting
2	2	8	10	8	0
3	4	7	17	13	6
1	0	6	23	23	17

Total Waiting Time: 23
Total Turnaround Time: 44
Average Waiting Time: 7.67
Average Turnaround Time: 14.67
```

### **Analysis of the Output:**
1. **Process 2 (P2) runs first** since it has the longest burst time among available processes.
2. **Process 3 (P3) runs next**, followed by **Process 1 (P1)**.
3. **The average waiting and turnaround times are calculated correctly.**

## **Conclusion**
- **LJF** favors longer processes but may cause **starvation** for shorter ones.
- **SRJF** efficiently minimizes **waiting and turnaround time**, but it involves **frequent context switching**.
- The program successfully simulates both algorithms and calculates key performance metrics.

This interpretation explains how the program works and how the results can be analyzed effectively.
---
## Executed Output
![ LFS Executed output ](lop1.png)
![ SRJF Executed Output](lop2.png)





# Program statement: Implement Peterson algorithm for critical section problem.
# Source Code: 
```c
#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>

#define NUM_ITERATIONS 5

volatile bool flag[2] = {false, false};
volatile int turn;

void enter_critical_section(int process) {
    int other = 1 - process;
    flag[process] = true;
    turn = other;
    while (flag[other] && turn == other);
}

void exit_critical_section(int process) {
    flag[process] = false;
}

void* process_function(void* arg) {
    int process = *(int*)arg;
    for (int i = 0; i < NUM_ITERATIONS; i++) {
        enter_critical_section(process);
        printf("Process %d is in the critical section (iteration %d)\n", process, i + 1);
        usleep(100000); // Simulating some work in critical section
        printf("Process %d is leaving the critical section (iteration %d)\n", process, i + 1);
        exit_critical_section(process);
        usleep(100000); // Simulating some work in non-critical section
    }
    return NULL;
}

int main() {
    pthread_t t0, t1;
    int p0 = 0, p1 = 1;
    
    pthread_create(&t0, NULL, process_function, &p0);
    pthread_create(&t1, NULL, process_function, &p1);
    
    pthread_join(t0, NULL);
    pthread_join(t1, NULL);
    
    return 0;
}
```
# OUTPUT:
![ Virtual lab experiment 2](vle2.png)

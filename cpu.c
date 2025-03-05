#include <stdio.h>

struct Process {
    int pid;
    int burst_time;
    int arrival_time;
    int priority;
};

void calculate_avg_times(int wait_time[], int turnaround_time[], int n) {
    float total_wt = 0, total_tat = 0;
    for (int i = 0; i < n; i++) {
        total_wt += wait_time[i];
        total_tat += turnaround_time[i];
    }
    printf("\nTotal Waiting Time: %.2f", total_wt);
    printf("\nTotal Turnaround Time: %.2f", total_tat);
    printf("\nAverage Waiting Time: %.2f", total_wt / n);
    printf("\nAverage Turnaround Time: %.2f\n", total_tat / n);
}

void fcfs(struct Process p[], int n) {
    int wait_time[n], turnaround_time[n], completion_time[n];
    
    completion_time[0] = p[0].arrival_time + p[0].burst_time;
    wait_time[0] = 0;
    turnaround_time[0] = completion_time[0] - p[0].arrival_time;
    
    for (int i = 1; i < n; i++) {
        completion_time[i] = completion_time[i - 1] + p[i].burst_time;
        wait_time[i] = completion_time[i - 1] - p[i].arrival_time;
        turnaround_time[i] = wait_time[i] + p[i].burst_time;
    }
    
    
    printf("PID\tBT\tAT\tWT\tTAT\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\n", p[i].pid, p[i].burst_time, p[i].arrival_time, wait_time[i], turnaround_time[i]);
    }
    calculate_avg_times(wait_time, turnaround_time, n);
}

void sjf(struct Process p[], int n) {
    struct Process temp;
    int wait_time[n], turnaround_time[n], completion_time[n];
    
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (p[i].burst_time > p[j].burst_time) {
                temp = p[i];
                p[i] = p[j];
                p[j] = temp;
            }
        }
    }
    
    completion_time[0] = p[0].arrival_time + p[0].burst_time;
    wait_time[0] = 0;
    turnaround_time[0] = completion_time[0] - p[0].arrival_time;
    
    for (int i = 1; i < n; i++) {
        completion_time[i] = completion_time[i - 1] + p[i].burst_time;
        wait_time[i] = completion_time[i - 1] - p[i].arrival_time;
        turnaround_time[i] = wait_time[i] + p[i].burst_time;
    }
    
    printf("\nSJF Scheduling\n");
    printf("PID\tBT\tAT\tWT\tTAT\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\n", p[i].pid, p[i].burst_time, p[i].arrival_time, wait_time[i], turnaround_time[i]);
    }
    calculate_avg_times(wait_time, turnaround_time, n);
}

void priority_scheduling(struct Process p[], int n) {
    struct Process temp;
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (p[i].priority > p[j].priority) {
                temp = p[i];
                p[i] = p[j];
                p[j] = temp;
            }
        }
    }
    fcfs(p, n);
}

void round_robin(struct Process p[], int n, int quantum) {
    int remaining_bt[n], wait_time[n], turnaround_time[n], completion_time[n];
    for (int i = 0; i < n; i++) remaining_bt[i] = p[i].burst_time;
    int time = 0, done;
    
    printf("\nRound Robin Scheduling\n");
    printf("PID\tWT\tTAT\n");
    
    do {
        done = 1;
        for (int i = 0; i < n; i++) {
            if (remaining_bt[i] > 0) {
                done = 0;
                if (remaining_bt[i] > quantum) {
                    time += quantum;
                    remaining_bt[i] -= quantum;
                } else {
                    time += remaining_bt[i];
                    wait_time[i] = time - p[i].arrival_time - p[i].burst_time;
                    turnaround_time[i] = time - p[i].arrival_time;
                    printf("%d\t%d\t%d\n", p[i].pid, wait_time[i], turnaround_time[i]);
                    remaining_bt[i] = 0;
                }
            }
        }
    } while (!done);
    calculate_avg_times(wait_time, turnaround_time, n);
}

int main() {
    int n;
    printf("Enter number of processes: ");
    scanf("%d", &n);
    struct Process p[n];
    
    for (int i = 0; i < n; i++) {
        printf("Enter PID, Burst Time, Arrival Time, and Priority for Process %d: ", i + 1);
        scanf("%d %d %d %d", &p[i].pid, &p[i].burst_time, &p[i].arrival_time, &p[i].priority);
    }
    
    int choice, quantum;
    do {
        printf("Choose Scheduling Algorithm:\n1. FCFS\n2. SJF\n3. Priority\n4. Round Robin\n5. Exit\n");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
		printf("\nFCFS Scheduling\n");
                fcfs(p, n);
                break;
            case 2:
                sjf(p, n);
                break;
            case 3:
		printf("\nPriority Sheduling \n");
                priority_scheduling(p, n);
                break;
            case 4:
                printf("Enter time quantum: ");
                scanf("%d", &quantum);
                round_robin(p, n, quantum);
                break;
            case 5:
                printf("Exiting program...\n");
                break;
            default:
                printf("Invalid choice\n");
        }
    } while (choice != 5);
    
    return 0;
}

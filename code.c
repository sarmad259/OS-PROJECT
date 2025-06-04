#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char name[50];
    int priority;
    int cpuBurst;
} Task;

// Function prototypes
void fcfs(Task tasks[], int n);
void sjf(Task tasks[], int n);
void prioritySched(Task tasks[], int n);
void RR(Task tasks[], int n, int timeQuantum);
void PRR(Task tasks[], int n, int timeQuantum);

int BURSTCOMPARE(const void *a, const void *b) {
    return ((Task *)a)->cpuBurst - ((Task *)b)->cpuBurst;
}

int PRIORITYCOMPARE(const void *a, const void *b) {
    return ((Task *)b)->priority - ((Task *)a)->priority;
}

int main() {
    FILE *file = fopen("schedule.txt", "r");
    if (!file) {
        perror("Failed to open file. Check if it's really there");
        return EXIT_FAILURE;
    }

    // Read tasks from file
    Task tasks[100];
    int count = 0;
    while (fscanf(file, "%[^,], %d, %d\n", tasks[count].name, &tasks[count].priority, &tasks[count].cpuBurst) == 3) {
        count++;
    }
    fclose(file);

    int timeQuantum;
    printf("Enter time quantum for Round Robin: ");
    scanf("%d", &timeQuantum);

    printf("\n========== FCFS ==========\n");
    fcfs(tasks, count);

    printf("\n========== SJF ==========\n");
    sjf(tasks, count);

    printf("\n========== Priority ==========\n");
    prioritySched(tasks, count);

    printf("\n========== RR ==========\n");
    RR(tasks, count, timeQuantum);

    printf("\n========== Priority with RR ==========\n");
    PRR(tasks, count, timeQuantum);

    return 0;
}

// First-Come-First-Served (FCFS)
void fcfs(Task tasks[], int n) {
    int waitingTime = 0, totalTurnaround = 0, currentTime = 0;

    printf("Task Order:\n");
    for (int i = 0; i < n; i++) {
        printf("%s ", tasks[i].name);
        currentTime += tasks[i].cpuBurst;
        totalTurnaround += currentTime;
        waitingTime += currentTime - tasks[i].cpuBurst;
    }

    printf("\nAverage Waiting Time: %.2f\n", (float)waitingTime / n);
    printf("Average Turnaround Time: %.2f\n", (float)totalTurnaround / n);
}

// Shortest Job First (SJF)
void sjf(Task tasks[], int n) {
    Task sortedTasks[n];
    memcpy(sortedTasks, tasks, sizeof(Task) * n);

    qsort(sortedTasks, n, sizeof(Task), BURSTCOMPARE);

    fcfs(sortedTasks, n); // Use FCFS logic since jobs are already sorted by burst time
}

// Priority Scheduling
void prioritySched(Task tasks[], int n) {
    Task sortedTasks[n];
    memcpy(sortedTasks, tasks, sizeof(Task) * n);

    qsort(sortedTasks, n, sizeof(Task), PRIORITYCOMPARE);

    fcfs(sortedTasks, n); // Use FCFS logic since jobs are already sorted by priority
}

// Round Robin (RR)
void RR(Task tasks[], int n, int timeQuantum) {
    int remainingBurst[n], waitingTime = 0, totalTurnaround = 0;
    int completed = 0, currentTime = 0;

    for (int i = 0; i < n; i++) remainingBurst[i] = tasks[i].cpuBurst;

    while (completed < n) {
        for (int i = 0; i < n; i++) {
            if (remainingBurst[i] > 0) {
                printf("%s ", tasks[i].name);
                if (remainingBurst[i] > timeQuantum) {
                    currentTime += timeQuantum;
                    remainingBurst[i] -= timeQuantum;
                } else {
                    currentTime += remainingBurst[i];
                    waitingTime += currentTime - tasks[i].cpuBurst;
                    totalTurnaround += currentTime;
                    remainingBurst[i] = 0;
                    completed++;
                }
            }
        }
    }

    printf("\nAverage Waiting Time: %.2f\n", (float)waitingTime / n);
    printf("Average Turnaround Time: %.2f\n", (float)totalTurnaround / n);
}

// Priority with Round Robin
void PRR(Task tasks[], int n, int timeQuantum) {
    Task sortedTasks[n];
    memcpy(sortedTasks, tasks, sizeof(Task) * n);

    qsort(sortedTasks, n, sizeof(Task), PRIORITYCOMPARE);

    RR(sortedTasks, n, timeQuantum);
}


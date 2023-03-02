#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int job_id;
    int deadline;
    int profit;
} Job;

int compare_jobs(const void *a, const void *b) {
    return ((Job*)b)->profit - ((Job*)a)->profit;
}

void job_sequence(Job jobs[], int n) {
    int max_deadline = 0;
    for (int i = 0; i < n; i++) {
        if (jobs[i].deadline > max_deadline) {
            max_deadline = jobs[i].deadline;
        }
    }

    int slot[max_deadline];
    for (int i = 0; i < max_deadline; i++) {
        slot[i] = -1;
    }

    int total_profit = 0, deadline, j;

    for (int i = 0; i < n; i++) {
        deadline = jobs[i].deadline;
        j = deadline - 1;
        while (j >= 0 && slot[j] != -1) {
            j--;
        }
        if (j >= 0) {
            slot[j] = jobs[i].job_id;
            total_profit += jobs[i].profit;
        }
    }

    printf("Job sequence: ");
    for (int i = 0; i < max_deadline; i++) {
        if (slot[i] != -1) {
            printf("%d ", slot[i]);
        }
    }

    printf("\nTotal profit: %d\n", total_profit);
}

int main() {
    Job jobs[] = {
        {1, 4, 20},
        {2, 1, 10},
        {3, 1, 40},
        {4, 2, 30}
    };
    int n = sizeof(jobs) / sizeof(jobs[0]);

    qsort(jobs, n, sizeof(Job), compare_jobs);

    job_sequence(jobs, n);

    return 0;
}


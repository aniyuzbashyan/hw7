#include <stdio.h>
#define MAX 100

typedef struct {
    int pid;
    int arrival_time;
    int burst_time;
    int start_time;
    int completion_time;
    int turnaround_time;
    int waiting_time;
    int response_time;
    int done;
} Process;

void calculate(Process p[], int i, int *time) {
    if (*time < p[i].arrival_time)
        *time = p[i].arrival_time;

    p[i].start_time = *time;
    *time += p[i].burst_time;

    p[i].completion_time = *time;
    p[i].turnaround_time = *time - p[i].arrival_time;
    p[i].waiting_time = p[i].turnaround_time - p[i].burst_time;
    p[i].response_time = p[i].start_time - p[i].arrival_time;
    p[i].done = 1;
}

void print_results(Process p[], int n, int order[], int starts[], int ends[]) {
    double wait = 0, turn = 0, response = 0;

    printf("Gantt Chart: ");

    int time = 0;
    for (int i = 0; i < n; i++) {
        if (starts[i] > time)
            printf("| Idle ");

        printf("| P%d ", order[i]);
        time = ends[i];
    }
    printf("|\n");

    printf("PID  AT  BT  WT  TAT  RT\n");

    for (int i = 0; i < n; i++) {
        printf("%-4d %-3d %-3d %-3d %-4d %d\n",
               p[i].pid, p[i].arrival_time, p[i].burst_time,
               p[i].waiting_time, p[i].turnaround_time,
               p[i].response_time);

        wait += p[i].waiting_time;
        turn += p[i].turnaround_time;
        response += p[i].response_time;
    }

    printf("Average Waiting Time: %.2f\n", wait / n);
    printf("Average Turnaround Time: %.2f\n", turn / n);
    printf("Average Response Time: %.2f\n", response / n);
}

void fcfs(Process original[], int n) {
    Process p[MAX];
    int order[MAX], starts[MAX], ends[MAX];
    int time = 0;

    for (int i = 0; i < n; i++)
        p[i] = original[i];

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (p[i].arrival_time > p[j].arrival_time) {
                Process temp = p[i];
                p[i] = p[j];
                p[j] = temp;
            }
        }
    }

    for (int i = 0; i < n; i++) {
        calculate(p, i, &time);
        order[i] = p[i].pid;
        starts[i] = p[i].start_time;
        ends[i] = time;
    }

    printf("\n=== First Come First Served (FCFS) ===\n");
    print_results(p, n, order, starts, ends);
}

void sjf(Process original[], int n) {
    Process p[MAX];
    int order[MAX], starts[MAX], ends[MAX];
    int time = 0;

    for (int i = 0; i < n; i++)
        p[i] = original[i];

    for (int k = 0; k < n; k++) {
        int selected = -1;

        for (int i = 0; i < n; i++) {
            if (p[i].done == 0 && p[i].arrival_time <= time) {
                if (selected == -1 ||
                    p[i].burst_time < p[selected].burst_time ||
                    (p[i].burst_time == p[selected].burst_time &&
                     p[i].arrival_time < p[selected].arrival_time))
                    selected = i;
            }
        }

        if (selected == -1) {
            for (int i = 0; i < n; i++) {
                if (p[i].done == 0 &&
                    (selected == -1 ||
                     p[i].arrival_time < p[selected].arrival_time))
                    selected = i;
            }
        }

        calculate(p, selected, &time);

        order[k] = p[selected].pid;
        starts[k] = p[selected].start_time;
        ends[k] = time;
    }

    printf("\n=== Shortest Job First (SJF) ===\n");
    print_results(p, n, order, starts, ends);
}

int main() {
    Process p[MAX] = {0};
    int n;

    printf("Enter the number of processes: ");
    if (scanf("%d", &n) != 1 || n < 1 || n > MAX) {
        printf("Invalid number of processes.\n");
        return 1;
    }

    for (int i = 0; i < n; i++) {
        p[i].pid = i + 1;

        printf("Enter arrival time and burst time for P%d: ", i + 1);
        if (scanf("%d %d", &p[i].arrival_time, &p[i].burst_time) != 2 ||
            p[i].arrival_time < 0 || p[i].burst_time <= 0) {
            printf("Invalid input.\n");
            return 1;
        }
    }

    fcfs(p, n);
    sjf(p, n);

    return 0;
}

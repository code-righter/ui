#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <iomanip>
using namespace std;

struct Process {
    int pid;
    int arrival;
    int burst;
    int remaining;
    int finish;
    int tat;
    int wait;
    int response;
    bool finished;
    bool started;
};

// --------------------------
// NON-PREEMPTIVE SJF
// --------------------------
void sjf_non_preemptive(vector<Process> p) {
    int n = p.size();
    int current_time = 0, completed = 0;
    double total_tat = 0, total_wait = 0, total_rt = 0;

    // sort by arrival time
    sort(p.begin(), p.end(), [](const Process &a, const Process &b) {
        return a.arrival < b.arrival;
    });

    while (completed < n) {
        int idx = -1, min_bt = INT_MAX;

        // find process with minimum burst time
        for (int i = 0; i < n; i++) {
            if (!p[i].finished && p[i].arrival <= current_time) {
                if (p[i].burst < min_bt) {
                    min_bt = p[i].burst;
                    idx = i;
                }
            }
        }

        if (idx == -1) {
            current_time++;
            continue;
        }

        // set response time (only first time)
        p[idx].response = current_time - p[idx].arrival;

        // run fully
        current_time += p[idx].burst;
        p[idx].finish = current_time;
        p[idx].tat = p[idx].finish - p[idx].arrival;
        p[idx].wait = p[idx].tat - p[idx].burst;
        p[idx].finished = true;

        total_tat += p[idx].tat;
        total_wait += p[idx].wait;
        total_rt += p[idx].response;
        completed++;
    }

    cout << "\nPID\tAT\tBT\tFT\tTAT\tWT\tRT\n";
    for (int i = 0; i < n; i++) {
        cout << p[i].pid << "\t" << p[i].arrival << "\t" << p[i].burst << "\t"
             << p[i].finish << "\t" << p[i].tat << "\t" << p[i].wait << "\t"
             << p[i].response << "\n";
    }

    cout << fixed << setprecision(2);
    cout << "\nAverage Turnaround Time = " << total_tat / n;
    cout << "\nAverage Waiting Time    = " << total_wait / n;
    cout << "\nAverage Response Time   = " << total_rt / n << endl;
}

// --------------------------
// PREEMPTIVE SJF (SRTF)
// --------------------------
void sjf_preemptive(vector<Process> p) {
    int n = p.size();
    int current_time = 0, completed = 0;
    double total_tat = 0, total_wait = 0, total_rt = 0;

    for (int i = 0; i < n; i++) {
        p[i].remaining = p[i].burst;
        p[i].finished = false;
        p[i].started = false;
    }

    // find earliest arrival
    int earliest = INT_MAX;
    for (int i = 0; i < n; i++)
        earliest = min(earliest, p[i].arrival);
    current_time = earliest;

    while (completed < n) {
        int idx = -1, min_rem = INT_MAX;

        // find process with minimum remaining time
        for (int i = 0; i < n; i++) {
            if (!p[i].finished && p[i].arrival <= current_time) {
                if (p[i].remaining < min_rem) {
                    min_rem = p[i].remaining;
                    idx = i;
                }
            }
        }

        if (idx == -1) {
            current_time++;
            continue;
        }

        // record response time only first time CPU starts the process
        if (!p[idx].started) {
            p[idx].response = current_time - p[idx].arrival;
            p[idx].started = true;
        }

        // execute for 1 unit
        p[idx].remaining--;
        current_time++;

        // if finished
        if (p[idx].remaining == 0) {
            p[idx].finish = current_time;
            p[idx].tat = p[idx].finish - p[idx].arrival;
            p[idx].wait = p[idx].tat - p[idx].burst;
            p[idx].finished = true;

            total_tat += p[idx].tat;
            total_wait += p[idx].wait;
            total_rt += p[idx].response;
            completed++;
        }
    }

    cout << "\nPID\tAT\tBT\tFT\tTAT\tWT\tRT\n";
    for (int i = 0; i < n; i++) {
        cout << p[i].pid << "\t" << p[i].arrival << "\t" << p[i].burst << "\t"
             << p[i].finish << "\t" << p[i].tat << "\t" << p[i].wait << "\t"
             << p[i].response << "\n";
    }

    cout << fixed << setprecision(2);
    cout << "\nAverage Turnaround Time = " << total_tat / n;
    cout << "\nAverage Waiting Time    = " << total_wait / n;
    cout << "\nAverage Response Time   = " << total_rt / n << endl;
}

// --------------------------
// MAIN
// --------------------------
int main() {
    int n, choice;

    cout << "Enter number of processes: ";
    cin >> n;

    vector<Process> p(n);

    for (int i = 0; i < n; i++) {
        p[i].pid = i + 1;
        cout << "Arrival time of P" << p[i].pid << ": ";
        cin >> p[i].arrival;
        cout << "Burst time of P" << p[i].pid << ": ";
        cin >> p[i].burst;
    }

    cout << "\nChoose Scheduling Type:\n";
    cout << "1. SJF Non-Preemptive\n";
    cout << "2. SJF Preemptive (SRTF)\n";
    cout << "Enter choice: ";
    cin >> choice;

    if (choice == 1)
        sjf_non_preemptive(p);
    else if (choice == 2)
        sjf_preemptive(p);
    else
        cout << "Invalid choice.";

    return 0;
}
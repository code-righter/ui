#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>

using namespace std;

struct Process {
    int pid;
    int at;    // arrival time
    int bt;    // burst time
    int pr;    // priority (lower = higher priority)
    int rem;   // remaining burst time
    int ft;    // finish time
    int tat;   // turnaround time
    int wt;    // waiting time
    bool started;
};

int main() {
    int n;
    cout << "Enter number of processes: ";
    cin >> n;
    vector<Process> p(n);

    for (int i = 0; i < n; ++i) {
        p[i].pid = i + 1;
        cout << "Process " << p[i].pid << " - Arrival Time, Burst Time, Priority: ";
        cin >> p[i].at >> p[i].bt >> p[i].pr;
        p[i].rem = p[i].bt;
        p[i].ft = p[i].tat = p[i].wt = 0;
        p[i].started = false;
    }

    // We'll simulate time unit by unit.
    // Start from earliest arrival
    int current_time = numeric_limits<int>::max();
    for (int i = 0; i < n; ++i) if (p[i].at < current_time) current_time = p[i].at;

    int completed = 0;
    double total_tat = 0.0;
    double total_wt = 0.0;

    while (completed < n) {
        // find highest priority process among arrived with rem > 0
        int idx = -1;
        for (int i = 0; i < n; ++i) {
            if (p[i].rem > 0 && p[i].at <= current_time) {
                if (idx == -1) idx = i;
                else {
                    if (p[i].pr < p[idx].pr) idx = i; // higher priority
                    else if (p[i].pr == p[idx].pr) {
                        if (p[i].at < p[idx].at) idx = i;
                        else if (p[i].at == p[idx].at && p[i].pid < p[idx].pid) idx = i;
                    }
                }
            }
        }

        if (idx == -1) {
            // no process has arrived yet or all arrived are finished -> jump to next arrival
            int next_at = numeric_limits<int>::max();
            for (int i = 0; i < n; ++i)
                if (p[i].rem > 0 && p[i].at < next_at) next_at = p[i].at;
            // advance time to next arrival
            current_time = next_at;
            continue;
        }

        // execute selected process for 1 time unit (preemptive)
        p[idx].rem -= 1;
        // mark started (not used for response time here, but kept for possible extension)
        if (!p[idx].started) p[idx].started = true;

        // if finished now, set completion data
        if (p[idx].rem == 0) {
            p[idx].ft = current_time + 1;               // finish at end of this time unit
            p[idx].tat = p[idx].ft - p[idx].at;
            p[idx].wt = p[idx].tat - p[idx].bt;

            total_tat += p[idx].tat;
            total_wt += p[idx].wt;
            completed++;
        }

        // advance time by 1
        current_time += 1;
    }

    // Print results (in PID order)
    // Sort by pid for nicer output (optional)
    sort(p.begin(), p.end(), [](const Process &a, const Process &b){
        return a.pid < b.pid;
    });

    cout << "\nPID\tAT\tBT\tPR\tFT\tTAT\tWT\n";
    for (int i = 0; i < n; ++i) {
        cout << p[i].pid << '\t'
             << p[i].at << '\t'
             << p[i].bt << '\t'
             << p[i].pr << '\t'
             << p[i].ft << '\t'
             << p[i].tat << '\t'
             << p[i].wt << '\n';
    }

    cout << "\nAverage Turnaround Time = " << (total_tat / n) << '\n';
    cout << "Average Waiting Time    = " << (total_wt / n) << '\n';

    return 0;
}
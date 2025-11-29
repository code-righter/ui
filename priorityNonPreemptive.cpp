#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Process {
    int pid;
    int at;   // arrival time
    int bt;   // burst time
    int pr;   // priority (lower value = higher priority)
    int ft;   // finish time
    int tat;  // turnaround time
    int wt;   // waiting time
    bool done;
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
        p[i].done = false;
        p[i].ft = p[i].tat = p[i].wt = 0;
    }

    // Sort by arrival time primarily so we can easily find next arrivals
    sort(p.begin(), p.end(), [](const Process &a, const Process &b) {
        if (a.at != b.at) return a.at < b.at;
        return a.pid < b.pid;
    });

    int completed = 0;
    int current_time = 0;
    double total_tat = 0;
    double total_wt = 0;

    // start time: set to earliest arrival
    if (n > 0) current_time = p[0].at;

    while (completed < n) {
        // find process with highest priority (lowest pr) among arrived and not done
        int idx = -1;
        for (int i = 0; i < n; ++i) {
            if (!p[i].done && p[i].at <= current_time) {
                if (idx == -1) idx = i;
                else {
                    if (p[i].pr < p[idx].pr) idx = i;            // priority
                    else if (p[i].pr == p[idx].pr) {             // tie-breaker: arrival time
                        if (p[i].at < p[idx].at) idx = i;
                        else if (p[i].at == p[idx].at && p[i].pid < p[idx].pid) idx = i;
                    }
                }
            }
        }

        if (idx == -1) {
            // no process has arrived yet; advance time to next arrival
            int next_at = INT_MAX;
            for (int i = 0; i < n; ++i)
                if (!p[i].done && p[i].at < next_at) next_at = p[i].at;
            current_time = next_at;
            continue;
        }

        // run the selected process non-preemptively
        int start_time = current_time;
        p[idx].ft = start_time + p[idx].bt;
        p[idx].tat = p[idx].ft - p[idx].at;
        p[idx].wt = p[idx].tat - p[idx].bt;
        p[idx].done = true;

        // update totals and time
        total_tat += p[idx].tat;
        total_wt += p[idx].wt;
        current_time = p[idx].ft;
        completed++;
    }

    // Print results
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
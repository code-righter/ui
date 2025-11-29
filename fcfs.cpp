#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct Process {
    int pid;            // process id
    int arrival;        // arrival time
    int burst;          // burst time
    int finish;         // finish time
    int tat;            // turnaround time
    int waiting;        // waiting time
};

int main() {
    int n;
    cout << "Enter number of processes: ";
    cin >> n;

    vector<Process> p(n);

    // Input arrival and burst times
    for (int i = 0; i < n; i++) {
        p[i].pid = i + 1;
        cout << "\nProcess " << p[i].pid << " Arrival Time: ";
        cin >> p[i].arrival;
        cout << "Process " << p[i].pid << " Burst Time: ";
        cin >> p[i].burst;
    }

    // Sort by arrival time for FCFS
    sort(p.begin(), p.end(), [](Process a, Process b){
        return a.arrival < b.arrival;
    });

    int current_time = 0;
    float total_tat = 0, total_wait = 0;

    // Calculate times
    for (int i = 0; i < n; i++) {

        // CPU waits if process arrives later
        if (current_time < p[i].arrival)
            current_time = p[i].arrival;

        p[i].finish = current_time + p[i].burst;
        p[i].tat = p[i].finish - p[i].arrival;
        p[i].waiting = p[i].tat - p[i].burst;

        total_tat += p[i].tat;
        total_wait += p[i].waiting;

        current_time = p[i].finish;
    }

    // Output table
    cout << "\n\nFCFS Scheduling Result\n";
    cout << "PID\tAT\tBT\tFT\tTAT\tWT\n";
    for (int i = 0; i < n; i++) {
        cout << p[i].pid << "\t"
             << p[i].arrival << "\t"
             << p[i].burst << "\t"
             << p[i].finish << "\t"
             << p[i].tat << "\t"
             << p[i].waiting << "\n";
    }

    cout << "\nAverage Turnaround Time = " << total_tat / n;
    cout << "\nAverage Waiting Time    = " << total_wait / n << endl;

    return 0;
}
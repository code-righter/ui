#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

struct Process {
    int id;
    int AT;
    int BT;
    int FT;
    int TAT;
    int WT;
};

int main() {
    int n;
    cout << "Enter number of processes: ";
    cin >> n;

    vector<Process> p(n);
    vector<int> rem(n);

    for (int i = 0; i < n; i++) {
        p[i].id = i + 1;
        cout << "Arrival Time of P" << i+1 << ": ";
        cin >> p[i].AT;
        cout << "Burst Time of P" << i+1 << ": ";
        cin >> p[i].BT;
        rem[i] = p[i].BT;
    }

    int tq;
    cout << "Enter Time Quantum: ";
    cin >> tq;

    // sort by arrival time
    sort(p.begin(), p.end(), [](Process &a, Process &b){
        return a.AT < b.AT;
    });

    queue<int> q;
    int t = 0, completed = 0, ptr = 0;

    if (ptr < n && t < p[ptr].AT)
        t = p[ptr].AT;

    while (ptr < n && p[ptr].AT <= t) {
        q.push(ptr);
        ptr++;
    }

    while (completed < n) {

        if (q.empty()) {
            if (ptr < n) {
                t = p[ptr].AT;
                while (ptr < n && p[ptr].AT <= t) {
                    q.push(ptr);
                    ptr++;
                }
            }
            continue;
        }

        int i = q.front();
        q.pop();

        int use = (rem[i] < tq) ? rem[i] : tq;
        rem[i] -= use;
        t += use;

        while (ptr < n && p[ptr].AT <= t) {
            q.push(ptr);
            ptr++;
        }

        if (rem[i] == 0) {
            p[i].FT = t;
            completed++;
        } else {
            q.push(i);
        }
    }

    // compute TAT and WT + averages
    double totalTAT = 0, totalWT = 0;

    for (int i = 0; i < n; i++) {
        p[i].TAT = p[i].FT - p[i].AT;
        p[i].WT  = p[i].TAT - p[i].BT;

        totalTAT += p[i].TAT;
        totalWT  += p[i].WT;
    }

    cout << "\nProcess\tAT\tBT\tFT\tTAT\tWT\n";
    for (int i = 0; i < n; i++) {
        cout << "P" << p[i].id << "\t"
             << p[i].AT << "\t"
             << p[i].BT << "\t"
             << p[i].FT << "\t"
             << p[i].TAT << "\t"
             << p[i].WT << "\n";
    }

    cout << "\nAverage TAT = " << totalTAT / n;
    cout << "\nAverage WT  = " << totalWT / n << "\n";

    return 0;
}
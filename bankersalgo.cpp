#include <iostream>
#include <vector>
using namespace std;

int main() {
    int n, m; // n = number of processes, m = number of resource types
    cout << "Enter number of processes: ";
    if (!(cin >> n) || n <= 0) return 0;
    cout << "Enter number of resource types: ";
    cin >> m;
    if (m <= 0) return 0;

    vector<int> total(m);
    cout << "Enter total number of each resource type (space separated " << m << " values):\n";
    for (int j = 0; j < m; ++j) cin >> total[j];

    // allocate matrices
    vector<vector<int>> alloc(n, vector<int>(m));
    vector<vector<int>> maxm(n, vector<int>(m));
    vector<vector<int>> need(n, vector<int>(m));
    vector<int> avail(m, 0);

    cout << "Enter Allocation matrix (" << n << " rows, each " << m << " values):\n";
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
            cin >> alloc[i][j];

    cout << "Enter Max (maximum demand) matrix (" << n << " rows, each " << m << " values):\n";
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
            cin >> maxm[i][j];

    // Calculate Available = Total - sum of allocations for each resource type
    for (int j = 0; j < m; ++j) {
        int sumAlloc = 0;
        for (int i = 0; i < n; ++i) sumAlloc += alloc[i][j];
        avail[j] = total[j] - sumAlloc;
    }

    // Calculate Need matrix = Max - Allocation
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            need[i][j] = maxm[i][j] - alloc[i][j];
            if (need[i][j] < 0) {
                cerr << "Error: For process P" << i << " resource " << j
                     << ", Allocation exceeds Max.\n";
                return 1;
            }
        }
    }

    // Print Available
    cout << "\nCalculated Available vector:\n";
    for (int j = 0; j < m; ++j) cout << avail[j] << (j+1<m ? " " : "\n");

    // Print Need matrix
    cout << "\nCalculated Need matrix:\n";
    for (int i = 0; i < n; ++i) {
        cout << "P" << i << ": ";
        for (int j = 0; j < m; ++j) cout << need[i][j] << (j+1<m ? " " : "");
        cout << "\n";
    }

    // Banker's safety algorithm (find safe sequence)
    vector<int> work = avail;
    vector<bool> finish(n, false);
    vector<int> safeSeq;
    bool progress = true;

    while (safeSeq.size() < (size_t)n && progress) {
        progress = false;
        for (int i = 0; i < n; ++i) {
            if (!finish[i]) {
                bool canRun = true;
                for (int j = 0; j < m; ++j) {
                    if (need[i][j] > work[j]) { canRun = false; break; }
                }
                if (canRun) {
                    // pretend to run and release its allocated resources
                    for (int j = 0; j < m; ++j) work[j] += alloc[i][j];
                    finish[i] = true;
                    safeSeq.push_back(i);
                    progress = true;
                }
            }
        }
    }

    // Check if all processes finished
    bool safe = (safeSeq.size() == (size_t)n);
    if (safe) {
        cout << "\nThe system is in a SAFE state.\nSafe sequence: ";
        for (size_t k = 0; k < safeSeq.size(); ++k) {
            cout << "P" << safeSeq[k];
            if (k + 1 < safeSeq.size()) cout << " -> ";
        }
        cout << "\n";
    } else {
        cout << "\nThe system is NOT in a safe state (UNSAFE).\n";
        cout << "No safe sequence exists with the current Available and allocations.\n";
    }

    return 0;
}
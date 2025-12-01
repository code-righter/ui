#include <iostream>
#include <vector>
#include <climits>
using namespace std;

int main() {
    int framesCount, refCount;
    cout << "Number of frames: ";
    cin >> framesCount;
    cout << "Number of references: ";
    cin >> refCount;

    vector<int> refs(refCount);
    cout << "Enter reference string (space separated):\n";
    for (int i = 0; i < refCount; ++i) cin >> refs[i];

    vector<int> frames(framesCount, -1); // -1 means empty slot
    int pageFaults = 0;

    for (int i = 0; i < refCount; ++i) {
        int page = refs[i];

        // Check if page already in frames (hit)
        bool hit = false;
        for (int f = 0; f < framesCount; ++f) {
            if (frames[f] == page) {
                hit = true;
                break;
            }
        }
        if (hit) continue;

        // Miss -> page fault
        ++pageFaults;

        // If there is empty slot, use it
        bool placed = false;
        for (int f = 0; f < framesCount; ++f) {
            if (frames[f] == -1) {
                frames[f] = page;
                placed = true;
                break;
            }
        }
        if (placed) continue;

        // No empty slot -> choose a page to replace using Optimal policy:
        // For each page in frame, find next use index; if never used again, its next use = INT_MAX.
        int replaceIndex = 0;
        int farthestNextUse = -1;
        for (int f = 0; f < framesCount; ++f) {
            int nextUse = INT_MAX;
            for (int k = i + 1; k < refCount; ++k) {
                if (refs[k] == frames[f]) {
                    nextUse = k;
                    break;
                }
            }
            // We want the page whose next use is farthest (or never used again)
            if (nextUse > farthestNextUse) {
                farthestNextUse = nextUse;
                replaceIndex = f;
            }
        }

        // Replace
        frames[replaceIndex] = page;
    }

    cout << "Total page faults = " << pageFaults << endl;
    return 0;
}
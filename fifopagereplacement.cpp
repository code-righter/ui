#include <iostream>
#include <vector>

using namespace std;

int main() {
    int framesCount, refLen;
    cout << "Number of frames: ";
    cin >> framesCount;
    cout << "Length of reference string: ";
    cin >> refLen;

    vector<int> ref(refLen);
    cout << "Enter reference string (space separated integers):\n";
    for (int i = 0; i < refLen; ++i) cin >> ref[i];

    vector<int> frames(framesCount, -1); // -1 means empty frame
    int nextToReplace = 0; // FIFO pointer (index of oldest page)
    int pageFaults = 0;

    for (int i = 0; i < refLen; ++i) {
        int page = ref[i];
        bool hit = false;
        // check if page already present (hit)
        for (int f = 0; f < framesCount; ++f) {
            if (frames[f] == page) { hit = true; break; }
        }

        if (!hit) {
            // page fault: place page at nextToReplace index
            frames[nextToReplace] = page;
            nextToReplace = (nextToReplace + 1) % framesCount;
            ++pageFaults;
        }
    }

    cout << "Total page faults = " << pageFaults << "\n";
    return 0;
}
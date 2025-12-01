// Simple Readers–Writers Problem (Reader Priority)
// Compile: g++ -std=c++11 rw.cpp -pthread -o rw
// Run: ./rw

#include <iostream>
#include <thread>
#include <mutex>
#include <vector>

int shared_data = 0;

// Mutex to protect reader_count
std::mutex rc_mutex;

// Mutex to protect the shared_data (writers need exclusive access)
std::mutex db_mutex;

int reader_count = 0;

// ------------------------ Reader Function ------------------------
void reader(int id, int times) {
    for (int i = 0; i < times; i++) {

        // Entry section
        rc_mutex.lock();
        reader_count++;
        if (reader_count == 1)
            db_mutex.lock();  // first reader blocks writers
        rc_mutex.unlock();

        // Critical section (reading)
        std::cout << "[Reader " << id << "] reads shared_data = " << shared_data << "\n";

        // Exit section
        rc_mutex.lock();
        reader_count--;
        if (reader_count == 0)
            db_mutex.unlock();  // last reader unlocks
        rc_mutex.unlock();
    }
}

// ------------------------ Writer Function ------------------------
void writer(int id, int times) {
    for (int i = 0; i < times; i++) {

        db_mutex.lock();  // writers get exclusive access

        // Critical section (writing)
        shared_data++;
        std::cout << ">>> [Writer " << id << "] writes shared_data = " << shared_data << "\n";

        db_mutex.unlock();
    }
}

// ---------------------------- MAIN -------------------------------
int main() {
    int R, W, times;

    std::cout << "Enter number of Readers: ";
    std::cin >> R;

    std::cout << "Enter number of Writers: ";
    std::cin >> W;

    std::cout << "How many times each thread should operate? ";
    std::cin >> times;

    std::vector<std::thread> threads;

    // Create Reader threads
    for (int i = 1; i <= R; i++)
        threads.emplace_back(reader, i, times);

    // Create Writer threads
    for (int i = 1; i <= W; i++)
        threads.emplace_back(writer, i, times);

    // Join threads
    for (auto &t : threads)
        t.join();

    std::cout << "\nFinal value of shared_data = " << shared_data << "\n";
    return 0;
}
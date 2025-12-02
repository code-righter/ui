#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
using namespace std;

// Simple semaphore using mutex + condition_variable
class Semaphore {
public:
    Semaphore(int count = 0) : count(count) {}

    void wait() {
        unique_lock<mutex> lock(mtx);
        cv.wait(lock, [&](){ return count > 0; });
        count--;
    }

    void signal() {
        unique_lock<mutex> lock(mtx);
        count++;
        cv.notify_one();
    }

private:
    mutex mtx;
    condition_variable cv;
    int count;
};

queue<int> bufferQ;
mutex bufferMutex;

Semaphore *emptySlots;
Semaphore *filledSlots;

// Producer Function
void producer(int id, int items) {
    for (int i = 0; i < items; i++) {
        int item = id * 100 + i;

        emptySlots->wait();  // wait for empty slot

        {
            lock_guard<mutex> lock(bufferMutex);
            bufferQ.push(item);
            cout << "[Producer " << id << "] produced: " << item
                 << " | buffer size = " << bufferQ.size() << "\n";
        }

        filledSlots->signal(); // signal data available
    }
}

// Consumer Function
void consumer(int id, int items) {
    for (int i = 0; i < items; i++) {
        filledSlots->wait(); // wait for data

        int item;
        {
            lock_guard<mutex> lock(bufferMutex);
            item = bufferQ.front();
            bufferQ.pop();
            cout << "    [Consumer " << id << "] consumed: " << item
                 << " | buffer size = " << bufferQ.size() << "\n";
        }

        emptySlots->signal(); // signal empty slot
    }
}

int main() {
    int bufferSize, prodCount, consCount, itemsPerProducer;

    // -------- USER INPUT ----------
    cout << "Enter buffer capacity: ";
    cin >> bufferSize;

    cout << "Enter number of producers: ";
    cin >> prodCount;

    cout << "Enter number of consumers: ";
    cin >> consCount;

    cout << "Enter items produced by each producer: ";
    cin >> itemsPerProducer;
    // --------------------------------

    // Initialize semaphores dynamically
    emptySlots = new Semaphore(bufferSize);
    filledSlots = new Semaphore(0);

    int totalItems = prodCount * itemsPerProducer;
    int itemsPerConsumer = totalItems / consCount;

    // Create producer threads
    thread producers[prodCount];
    for (int i = 0; i < prodCount; i++)
        producers[i] = thread(producer, i + 1, itemsPerProducer);

    // Create consumer threads
    thread consumers[consCount];
    for (int i = 0; i < consCount; i++)
        consumers[i] = thread(consumer, i + 1, itemsPerConsumer);

    // Join all threads
    for (int i = 0; i < prodCount; i++) producers[i].join();
    for (int i = 0; i < consCount; i++) consumers[i].join();

    cout << "All producers and consumers finished.\n";
    return 0;
}
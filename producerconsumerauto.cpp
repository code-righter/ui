#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <chrono>

// Very small semaphore implementation (works on C++11+)
class Semaphore {
public:
    Semaphore(int count = 0) : count(count) {}

    void wait() {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [&](){ return count > 0; });
        --count;
    }

    void signal() {
        std::unique_lock<std::mutex> lock(mtx);
        ++count;
        cv.notify_one();
    }

private:
    std::mutex mtx;
    std::condition_variable cv;
    int count;
};

const int BUFFER_CAPACITY = 5;
std::queue<int> buffer;
std::mutex buffer_mutex;

Semaphore emptySlots(BUFFER_CAPACITY); // initially all slots are empty
Semaphore filledSlots(0);               // initially no items

// simple producer that produces 'n_items' integers (unique id per producer)
void producer(int id, int n_items) {
    for (int i = 0; i < n_items; ++i) {
        int item = id * 100 + i; // simple item value to show origin
        emptySlots.wait();       // wait for an empty slot

        {
            std::lock_guard<std::mutex> lock(buffer_mutex);
            buffer.push(item);
            std::cout << "[Producer " << id << "] produced: " << item
                      << " | buffer_size = " << buffer.size() << '\n';
        }

        filledSlots.signal();    // signal that there's a filled slot

        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // slow down for demo
    }
}

// simple consumer that consumes 'n_items' integers
void consumer(int id, int n_items) {
    for (int i = 0; i < n_items; ++i) {
        filledSlots.wait();      // wait for an item to be available

        int item;
        {
            std::lock_guard<std::mutex> lock(buffer_mutex);
            item = buffer.front();
            buffer.pop();
            std::cout << "    [Consumer " << id << "] consumed: " << item
                      << " | buffer_size = " << buffer.size() << '\n';
        }

        emptySlots.signal();     // signal that there's a free slot

        std::this_thread::sleep_for(std::chrono::milliseconds(150)); // slow down for demo
    }
}

int main() {
    int producers_count ;
    int consumers_count ;
    int items_per_producer;
    std::cout << "Enter producer count: ";
    std::cin >> producers_count;

    std::cout << "Enter consumer count: ";
    std::cin >> consumers_count;

    std::cout << "Enter items produced by each producer: ";
    std::cin >> items_per_producer;
    // const int items_per_consumer = (producers_count * items_per_producer) / consumers_count;
    int items_per_consumer = (producers_count * items_per_producer) / consumers_count;

    // start producers
    std::thread producers[producers_count];
    for (int i = 0; i < producers_count; ++i)
        producers[i] = std::thread(producer, i+1, items_per_producer);

    // start consumers
    std::thread consumers[consumers_count];
    for (int i = 0; i < consumers_count; ++i)
        consumers[i] = std::thread(consumer, i+1, items_per_consumer);

    // join all threads
    for (int i = 0; i < producers_count; ++i) producers[i].join();
    for (int i = 0; i < consumers_count; ++i) consumers[i].join();

    std::cout << "All producers and consumers finished.\n";
    return 0;
}
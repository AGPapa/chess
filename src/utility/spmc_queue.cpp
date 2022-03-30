#include <thread>
#include <queue>

#include "ringbuffer_queue.cpp"

template<typename Item>
class SPMCQueue {
    // Sincle-producer multi-consumer
    public:
        SPMCQueue() {
            queue = RingbufferQueue<Item, 1024>();
        }

        void enqueue(Item i) {
            bool success = queue.enqueue(i);
            while (success == false) {
                std::mutex mutex;
                std::unique_lock<std::mutex> lock = std::unique_lock<std::mutex>(mutex);
                _full_variable.wait_for(lock, std::chrono::milliseconds(5));
                success = queue.enqueue(i);
            }
        }

        bool dequeue(Item* i) {
            _mutex.lock();
            bool success = queue.dequeue(i);
            _full_variable.notify_one();
            _mutex.unlock();
            return success;
        }

        bool empty() {
            return queue.empty();
        }

        int size() {
            return queue.size();
        }

     private:
        std::mutex _mutex;
        RingbufferQueue<Item, 1024> queue;
        std::condition_variable _full_variable;
};
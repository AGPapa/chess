#include <thread>
#include <queue>

#include "ringbuffer_queue.cpp"

template<typename Item>
class MPSCQueue {
    // Multi-producer single-consumer
    public:
        MPSCQueue() {
            queue = RingbufferQueue<Item, 1024>();
        }

        void enqueue(Item i) {
            _mutex.lock();
            bool success = queue.enqueue(i);
            while (success == false) {
                std::mutex mutex;
                std::unique_lock<std::mutex> lock = std::unique_lock<std::mutex>(mutex);
                _full_variable.wait(lock);
                success = queue.enqueue(i);
            }
            _mutex.unlock();
        }

        bool dequeue(Item* i) {
            bool success = queue.dequeue(i);
            _full_variable.notify_one();
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
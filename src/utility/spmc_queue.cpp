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

        void enqueue(std::unique_ptr<Item> i) {
            i = queue.enqueue(std::move(i));
            while (i != nullptr) {
                std::mutex mutex;
                std::unique_lock<std::mutex> lock = std::unique_lock<std::mutex>(mutex);
                _full_variable.wait_for(lock, std::chrono::milliseconds(5));
                i = queue.enqueue(std::move(i));
            }
        }

        std::unique_ptr<Item> dequeue() {
            _mutex.lock();
            std::unique_ptr<Item> i = std::move(queue.dequeue());
            _full_variable.notify_one();
            _mutex.unlock();
            return i;
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
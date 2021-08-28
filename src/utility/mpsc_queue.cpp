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

        void enqueue(std::unique_ptr<Item> i) {
            _mutex.lock();
            i = queue.enqueue(std::move(i));
            while (i != nullptr) {
                std::mutex mutex;
                std::unique_lock<std::mutex> lock = std::unique_lock<std::mutex>(mutex);
                _full_variable.wait(lock);
                i = queue.enqueue(std::move(i));
            }
            _mutex.unlock();
        }

        std::unique_ptr<Item> dequeue() {
            std::unique_ptr<Item> i = std::move(queue.dequeue());
            _full_variable.notify_one();
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
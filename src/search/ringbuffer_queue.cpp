#include <thread>
#include <atomic>

#include <iostream>

template<typename Item, int MaxSize>
class RingbufferQueue {
    // Single-producer single-consumer
    public:
        RingbufferQueue() {
            _read_index = 0;
            _write_index = 0;
        }

        std::unique_ptr<Item> enqueue(std::unique_ptr<Item> i) {
            while (full()) {
               return std::move(i);
            }
            _ringbuffer[_write_index] = std::move(i);
            atomic_signal_fence(std::memory_order_relaxed); //without this the multithreading sometimes messes up
            _write_index = _next(_write_index);
            return nullptr;
        }

        std::unique_ptr<Item> dequeue() {
            if (empty()) {
                return nullptr;
            }
            std::unique_ptr<Item> i = std::move(_ringbuffer[_read_index]);
            _read_index = _next(_read_index);
            return i;
        }

        bool empty() const {
            return _read_index == _write_index;
        }

        bool full() const {
            return _read_index == _next(_write_index);
        }

        int size() const {
            if (_write_index >= _read_index) {
                return _write_index - _read_index;
            } else {
                return MaxSize - _read_index + _write_index;
            }
        }

     private:
        std::unique_ptr<Item> _ringbuffer[MaxSize];
        volatile int _read_index;
        volatile int _write_index;

        int _next(const int i) const {
            return (i + 1) % MaxSize;
        }
};
#pragma once

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

        bool enqueue(Item i) {
            if (full()) {
               return false;
            }
            _ringbuffer[_write_index] = i;
            atomic_signal_fence(std::memory_order_acq_rel); //without this the multithreading sometimes messes up
            _write_index = _next(_write_index);
            return true;
        }

        bool dequeue(Item* i) {
            if (empty()) {
                return false;
            }
            *i = _ringbuffer[_read_index];
            _read_index = _next(_read_index);
            return true;
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
        Item _ringbuffer[MaxSize];
        volatile int _read_index;
        volatile int _write_index;

        int _next(const int i) const {
            return (i + 1) % MaxSize;
        }
};
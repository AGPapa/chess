#include "../../utility/ringbuffer_queue.cpp"
#include <thread>

#include <gtest/gtest.h>

void enqueue_items(RingbufferQueue<int, 1024> *q) {
    for (int i = 0; i < 100000; i++) {
        bool success = false;
        while (success == false) {
            success = q->enqueue(i);
        }
    }
}

void dequeue_items(RingbufferQueue<int, 1024> *q) {
    std::unique_ptr<int> previous_item = std::unique_ptr<int>(new int(100));
    std::unique_ptr<int> item = std::unique_ptr<int>(new int(100));
    int i = 0;
    while (i < 100000) {
        while (!q->empty()) {
            q->dequeue(item.get());
            if (i > 1) {
               ASSERT_EQ(*previous_item.get() + 1, *item.get());
            }
            *previous_item = *item;
            i++;
        }
    }
}

TEST(RingBufferTest, single_item) {
    RingbufferQueue<int, 5> q = RingbufferQueue<int, 5>();
    ASSERT_EQ(q.empty(), true);
    q.enqueue(1);
    ASSERT_EQ(q.empty(), false);
    std::unique_ptr<int> item = std::unique_ptr<int>(new int(100));
    q.dequeue(item.get());
    ASSERT_EQ(*item.get(), 1);
    ASSERT_EQ(q.empty(), true);
    
    q.enqueue(2);
    q.dequeue(item.get());
    ASSERT_EQ(*item.get(), 2);
}

TEST(RingBufferTest, multi_item) {
    RingbufferQueue<int, 5> q = RingbufferQueue<int, 5>();
    std::unique_ptr<int> item = std::unique_ptr<int>(new int(100));
    q.enqueue(1);
    q.enqueue(2);
    q.dequeue(item.get());
    ASSERT_EQ(*item.get(), 1);
    q.dequeue(item.get());
    ASSERT_EQ(*item.get(), 2);
}

TEST(RingBufferTest, overflow) {
    RingbufferQueue<int, 3> q = RingbufferQueue<int, 3>();
    std::unique_ptr<int> item = std::unique_ptr<int>(new int(100));
    q.enqueue(1);
    q.dequeue(item.get());
    ASSERT_EQ(*item.get(), 1);
    ASSERT_EQ(q.empty(), true);

    q.enqueue(2);
    ASSERT_EQ(q.size(), 1);
    ASSERT_EQ(q.empty(), false);
    ASSERT_EQ(q.full(), false);

    q.enqueue(3);
    ASSERT_EQ(q.size(), 2);
    ASSERT_EQ(q.full(), true);

    q.dequeue(item.get());
    ASSERT_EQ(*item.get(), 2);
    ASSERT_EQ(q.size(), 1);

    q.dequeue(item.get());
    ASSERT_EQ(*item.get(), 3);
    ASSERT_EQ(q.size(), 0);
    ASSERT_EQ(q.empty(), true);
}

TEST(RingBufferTest, full) {
    RingbufferQueue<int, 3> q = RingbufferQueue<int, 3>();
    std::unique_ptr<int> item = std::unique_ptr<int>(new int(100));
    ASSERT_EQ(q.enqueue(1), true);
    q.dequeue(item.get());
    ASSERT_EQ(*item.get(), 1);
    ASSERT_EQ(q.empty(), true);

    ASSERT_EQ(q.enqueue(2), true);
    ASSERT_EQ(q.size(), 1);
    ASSERT_EQ(q.empty(), false);
    ASSERT_EQ(q.full(), false);

    ASSERT_EQ(q.enqueue(3), true);
    ASSERT_EQ(q.size(), 2);
    ASSERT_EQ(q.full(), true);

    ASSERT_EQ(q.enqueue(4), false);
    ASSERT_EQ(q.size(), 2);
    ASSERT_EQ(q.full(), true);
}

TEST(RingBufferTest, thread_test) {
    RingbufferQueue<int, 1024> q = RingbufferQueue<int, 1024>();
    std::thread enqueue_thread = std::thread(enqueue_items, &q);
    std::thread dequeue_thread = std::thread(dequeue_items, &q);
    enqueue_thread.join();
    dequeue_thread.join();
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
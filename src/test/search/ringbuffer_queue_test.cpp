#include "../../search/ringbuffer_queue.cpp"
#include <thread>

#include <gtest/gtest.h>

void enqueue_items(RingbufferQueue<int, 1024> *q) {
    for (int i = 0; i < 10000; i++) {
        std::unique_ptr<int> item = std::unique_ptr<int>(new int(i));
        bool success = false;
        while (item != nullptr) {
            item = std::move(q->enqueue(std::move(item)));
        }
    }
}

void dequeue_items(RingbufferQueue<int, 1024> *q) {
    std::unique_ptr<int> previous_item;
    std::unique_ptr<int> item;
    int i = 0;
    while (i < 10000) {
        while (!q->empty()) {
            item = std::move(q->dequeue());
            if (i > 1) {
               ASSERT_EQ(*previous_item.get() + 1, *item.get());
            }
            previous_item = std::move(item);
            i++;
        }
    }
}

TEST(RingBufferTest, single_item) {
    RingbufferQueue<int, 5> q = RingbufferQueue<int, 5>();
    std::unique_ptr<int> item = std::unique_ptr<int>(new int(1));
    ASSERT_EQ(q.empty(), true);
    q.enqueue(std::move(item));
    ASSERT_EQ(q.empty(), false);
    item = std::move(q.dequeue());
    ASSERT_EQ(*item.get(), 1);
    ASSERT_EQ(q.empty(), true);
    

    item.reset();
    item = std::unique_ptr<int>(new int(2));
    q.enqueue(std::move(item));
    item = std::move(q.dequeue());
    ASSERT_EQ(*item.get(), 2);
}

TEST(RingBufferTest, multi_item) {
    RingbufferQueue<int, 5> q = RingbufferQueue<int, 5>();
    std::unique_ptr<int> item = std::unique_ptr<int>(new int(1));
    q.enqueue(std::move(item));
    item = std::unique_ptr<int>(new int(2));
    q.enqueue(std::move(item));
    item = std::move(q.dequeue());
    ASSERT_EQ(*item.get(), 1);
    item = std::move(q.dequeue());
    ASSERT_EQ(*item.get(), 2);
}

TEST(RingBufferTest, overflow) {
    RingbufferQueue<int, 3> q = RingbufferQueue<int, 3>();
    std::unique_ptr<int> item = std::unique_ptr<int>(new int(1));
    q.enqueue(std::move(item));
    item = std::move(q.dequeue());
    ASSERT_EQ(*item.get(), 1);
    ASSERT_EQ(q.empty(), true);

    item = std::unique_ptr<int>(new int(2));
    q.enqueue(std::move(item));
    ASSERT_EQ(q.size(), 1);
    ASSERT_EQ(q.empty(), false);
    ASSERT_EQ(q.full(), false);

    item = std::unique_ptr<int>(new int(3));
    q.enqueue(std::move(item));
    ASSERT_EQ(q.size(), 2);
    ASSERT_EQ(q.full(), true);

    item = std::move(q.dequeue());
    ASSERT_EQ(*item.get(), 2);
    ASSERT_EQ(q.size(), 1);

    item = std::move(q.dequeue());
    ASSERT_EQ(*item.get(), 3);
    ASSERT_EQ(q.size(), 0);
    ASSERT_EQ(q.empty(), true);
}

TEST(RingBufferTest, full) {
     RingbufferQueue<int, 3> q = RingbufferQueue<int, 3>();
    std::unique_ptr<int> item = std::unique_ptr<int>(new int(1));
    ASSERT_EQ(q.enqueue(std::move(item)), nullptr);
    item = std::move(q.dequeue());
    ASSERT_EQ(*item.get(), 1);
    ASSERT_EQ(q.empty(), true);

    item = std::unique_ptr<int>(new int(2));
    ASSERT_EQ(q.enqueue(std::move(item)), nullptr);
    ASSERT_EQ(q.size(), 1);
    ASSERT_EQ(q.empty(), false);
    ASSERT_EQ(q.full(), false);

    item = std::unique_ptr<int>(new int(3));
    ASSERT_EQ(q.enqueue(std::move(item)), nullptr);
    ASSERT_EQ(q.size(), 2);
    ASSERT_EQ(q.full(), true);

    item = std::unique_ptr<int>(new int(3));
    ASSERT_NE(q.enqueue(std::move(item)), nullptr);
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
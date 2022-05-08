#pragma once
#include <chrono>
#include <condition_variable>
#include <mutex>

class CountDownLatch {
public:
    explicit CountDownLatch(const unsigned int count): m_count(count) { }
    virtual ~CountDownLatch() = default;

    void await();

    void countDown();

    unsigned int getCount();

protected:
    std::mutex m_mutex;
    std::condition_variable m_cv;
    unsigned int m_count = 0;
};
#include "countdown_latch.h"

void CountDownLatch::await() {
    std::unique_lock<std::mutex> lock(m_mutex);
    if (m_count > 0) {
        m_cv.wait(lock);
    }
}

void CountDownLatch::countDown() {
    std::unique_lock<std::mutex> lock(m_mutex);
    m_count--;
    if (m_count == 0)
        m_cv.notify_all();
}

unsigned int CountDownLatch::getCount() {
    std::unique_lock<std::mutex> lock(m_mutex);
    return m_count;
}

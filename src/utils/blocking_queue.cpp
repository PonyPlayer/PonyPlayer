#include "blocking_queue.h"

template<typename T>
bool BlockingQueue<T>::isEmpty() {
    return size == 0;
}

template<typename T>
bool BlockingQueue<T>::isFull() {
    return size == capacity;
}

template<typename T>
void BlockingQueue<T>::enqueue(const T &item) {
    if (isFull()) throw std::runtime_error("blocking queue internal error! trying to enqueue on a full queue!");
    if (frontIdx == -1) ++frontIdx;
    ++rearIdx;
    rearIdx = rearIdx % capacity;
    queue[rearIdx] = item;
    size++;
}

template<typename T>
BlockingQueue<T>::BlockingQueue(const long long int &_capacity):capacity(_capacity), frontIdx(-1), rearIdx(-1), size(0) {
    if (capacity < 1) throw std::runtime_error("blocking queue should have a capacity larger than 1!");
    queue = new T[capacity];
}

template<typename T>
void BlockingQueue<T>::push(const T &item) {
    std::unique_lock<std::shared_mutex> exLock(mutex);
    if (isFull()) throw std::range_error("queue is full while doing non-blocking push");
    enqueue(item);
    exLock.unlock();
    cond.notify_all();
}

template<typename T>
void BlockingQueue<T>::bpush(const T &item) {
    std::unique_lock<std::shared_mutex> exLock(mutex);
    if (isFull()) cond.wait(exLock, [&]() { return capacity - size > 0; });
    enqueue(item);
    exLock.unlock();
    cond.notify_all();
}

template<typename T>
void BlockingQueue<T>::pop() {
    std::unique_lock<std::shared_mutex> exLock(mutex);
    if (isEmpty()) throw std::range_error("queue is empty while doing non-blocking pop");
    dequeue();
    exLock.unlock();
    cond.notify_all();
}

template<typename T>
void BlockingQueue<T>::dequeue() {
    if (isEmpty()) throw std::runtime_error("blocking queue internal error! trying to dequeue on a empty queue!");
    if (frontIdx == rearIdx) frontIdx = rearIdx = -1;
    else frontIdx = (frontIdx + 1) % capacity;
    size--;
}

template<typename T>
void BlockingQueue<T>::bpop() {
    std::unique_lock<std::shared_mutex> exLock(mutex);
    if (isEmpty()) cond.wait(exLock, [&]() { return size > 0; });
    dequeue();
    exLock.unlock();
    cond.notify_all();
}

template<typename T>
T BlockingQueue<T>::front() {
    std::unique_lock<std::shared_mutex> exLock(mutex);
    if (isEmpty()) throw std::range_error("queue is empty while doing non-blocking front");
    T ret = queue[frontIdx];
    exLock.unlock();
    return ret;
}

template<typename T>
T BlockingQueue<T>::bfront() {
    std::unique_lock<std::shared_mutex> exLock(mutex);
    if (isEmpty()) cond.wait(exLock, [&]() { return size > 0; });
    T ret = queue[frontIdx];
    exLock.unlock();
    return ret;
}

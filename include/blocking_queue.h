#pragma once

#include <shared_mutex>
#include <condition_variable>
#include <sstream>
#include <vector>
#include "demuxer.h"
#include <functional>

template<typename T>
class BlockingQueue {
private:
    long long int capacity, frontIdx, rearIdx, size;
    T *queue;
    std::shared_mutex mutex;
    std::condition_variable_any cond;

    bool isEmpty();

    bool isFull();

    void enqueue(const T &);

    void dequeue();

    void applyToAll(std::function<void(T)> func);



public:

    explicit BlockingQueue(const long long int &_capacity);

    /*
     * 向队列中插入元素(非阻塞)
     */

    void push(const T &item);

    /*
     * 向队列中插入元素(阻塞直到可以插入)
     */

    bool bpush(const T &item);

    /*
     * 从队列中弹出元素(非阻塞)
     */

    void pop();

    /*
     * 从队列中弹出元素(阻塞直到可以弹出)
     */

    bool bpop();

    /*
     * 查看队列头(非阻塞)
     */

    T front();

    /*
     * 查看队列头(阻塞直到队列中有元素)
     */

    T bfront();

    /**
     * 清空队列元素
     * @return 清理元素数量
     */
    long long int clear();

    long long int clearWith(std::function<void(T)> func);

    void notify();

};

template
class BlockingQueue<int>;

template
class BlockingQueue<AVFrame *>;


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
BlockingQueue<T>::BlockingQueue(const long long int &_capacity):capacity(_capacity), frontIdx(-1), rearIdx(-1),
                                                                size(0) {
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
bool BlockingQueue<T>::bpush(const T &item) {
    std::unique_lock<std::shared_mutex> exLock(mutex);
    if (isFull()) cond.wait(exLock);
    if (capacity - size <= 0) {
        exLock.unlock();
        return false;
    }
    enqueue(item);
    exLock.unlock();
    cond.notify_all();
    return true;
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
bool BlockingQueue<T>::bpop() {
    std::unique_lock<std::shared_mutex> exLock(mutex);
    if (isEmpty()) cond.wait(exLock);
    if (size <= 0) {
        exLock.unlock();
        return false;
    }
    dequeue();
    exLock.unlock();
    cond.notify_all();
    return true;
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
    if (isEmpty()) cond.wait(exLock);
    if (size <= 0) {
        exLock.unlock();
        return T();
    }
    T ret = queue[frontIdx];
    exLock.unlock();
    return ret;
}

template<typename T>
long long int BlockingQueue<T>::clear() {
    std::unique_lock<std::shared_mutex> exLock(mutex);
    long long int ret = size;
    frontIdx = rearIdx = -1;
    size = 0;
    exLock.unlock();
    cond.notify_all();
    return ret;
}

template<typename T>
void BlockingQueue<T>::applyToAll(std::function<void(T)> func) {
    std::unique_lock<std::shared_mutex> exLock(mutex);
    long long int ret = size;
    if (frontIdx == -1 || rearIdx == -1) return;
    for (long long int i = frontIdx; i != rearIdx; i = (i + 1) % capacity) {
        func(queue[i]);
    }
    func(queue[rearIdx]);
    exLock.unlock();
}

template<typename T>
long long int BlockingQueue<T>::clearWith(std::function<void(T)> func) {
    std::unique_lock<std::shared_mutex> exLock(mutex);
    long long int ret = size;
    if (frontIdx == -1 || rearIdx == -1) return 0;
    for (long long int i = frontIdx; i != rearIdx; i = (i + 1) % capacity) {
        func(queue[i]);
    }
    func(queue[rearIdx]);
    frontIdx = rearIdx = -1;
    size = 0;
    exLock.unlock();
    cond.notify_all();
    return ret;
}

template<typename T>
void BlockingQueue<T>::notify() {
    cond.notify_all();
}



#pragma once

#include <shared_mutex>
#include <condition_variable>
#include <sstream>
#include <vector>

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

public:

    explicit BlockingQueue(const long long int &_capacity);

    /*
     * 向队列中插入元素(非阻塞)
     */

    void push(const T &item);

    /*
     * 向队列中插入元素(阻塞直到可以插入)
     */

    void bpush(const T &item);

    /*
     * 从队列中弹出元素(非阻塞)
     */

    void pop();

    /*
     * 从队列中弹出元素(阻塞直到可以弹出)
     */

    void bpop();

    /*
     * 查看队列头(非阻塞)
     */

    T front();

    /*
     * 查看队列头(阻塞直到队列中有元素)
     */

    T bfront();


};

template
class BlockingQueue<int>;

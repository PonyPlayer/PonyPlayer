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

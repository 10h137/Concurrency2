#include <cstddef>
#include <climits>
#include <queue>
#include <stack>
#include <mutex>
#include <condition_variable>
#include <thread>
#include "PTask.h"

template <typename T>
class Node
{
public:
    T data;
    std::atomic<Node<T> *> next;
    Node<T>(T data)
    {
        this->data = data;
    }
    Node<T>()
    {
        this->data = NULL;
    }
};

template <typename T>
class LockingQueue
{
// ACTUALLY STACK
private:
    std::stack<T> queue_;
    std::condition_variable condition_var;
    std::mutex mutex_;

public:
    LockingQueue()
    {
    }

    void enqueue(T payload)
    {
        std::unique_lock<std::mutex> mlock(mutex_);
        queue_.push(payload);
        mlock.unlock();
    }

    T dequeue()
    {
        std::unique_lock<std::mutex> mlock(mutex_);
        if (queue_.empty())
            return (T)NULL;
        T item = queue_.top();
        queue_.pop();
        return item;
    }

   
};

template class LockingQueue<int>;
template class LockingQueue<PTask<int, int> *>;
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "queues.h"
#include "PTask.h"


template <typename T>
LockingQueue<T>::LockingQueue(){
}

template <typename T>
void LockingQueue<T>::enqueue(T payload){
    std::unique_lock<std::mutex> mlock(mutex_);
    queue_.push(payload);
    mlock.unlock();
    condition_var.notify_one();
}

template <typename T>
T LockingQueue<T>::dequeue(){
    std::unique_lock<std::mutex> mlock(mutex_);
    if (queue_.empty()) return NULL;
    T item = queue_.front();
    queue_.pop();
    mlock.unlock();
    return item;
}


template class LockingQueue<int>;
template class LockingQueue<PTask<int, int>*>;

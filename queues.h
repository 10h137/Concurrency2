#include <atomic>
#include <cstddef>
#include <climits>
#include <queue>
#include <mutex>
#include <condition_variable>


template <typename T>
class CQueue{
    public:
    virtual ~CQueue() {}
    virtual void enqueue(T data) = 0;
    virtual T dequeue() = 0;
};



template <typename T>
class Node {
public:
T data = 0;
std::atomic<Node<T>*> next;
Node<T>(T data) {
this->data = data;
}
Node<T>() {
this->data = data;
}
};

template <typename T>
class NonLockingQueue : public CQueue<T> {
private:
std::atomic<Node<T>*> head;
std::atomic<Node<T>*> tail;

public:
NonLockingQueue<T>() {
    head = new Node<T> ();
    tail = new Node<T> ();
    head.load()->next.store(tail.load());
};

void printQueue();

void enqueue(T data);

T dequeue();

};

template <typename T>
class LockingQueue : public CQueue<T>{

private:
std::queue<T> queue_;
std::condition_variable condition_var;
std::mutex mutex_;

public:
LockingQueue();


void enqueue(T data);
T dequeue();
bool isEmpty();

};
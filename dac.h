#include <vector>
#include "PTask.h"
#include "ConcurrentQueue.h"
#include "CFib.h"
#include <iostream>

using namespace std;

template <typename I, typename O>

class DAC
{

public:
    typedef void *(*THREADFUNCPTR)(void *);
    typedef void (*Divide)(const PTask<I, O> &, std::vector<PTask<I, O> *> *);
    typedef void (*Combine)(std::vector<PTask<I, O> *> &, PTask<I, O> *);
    typedef void (*Base)(const I &, PTask<I, O> *);
    typedef bool (*Threshold)(const I &);

    Divide divide;
    Combine combine;
    Base base;
    Threshold threshold;
    int max_threads;
    std::atomic<bool> complete;

    struct s
    {
        int index;
        std::atomic<bool> *complete;
        Combine combine;
        std::vector<LockingQueue<PTask<I, O> *> *> *worker_queues;
    };

    DAC(const Divide d, Combine c, Base b, Threshold t, int max_t)
    {
        std::cout.setf(std::ios::unitbuf);
        divide = d;
        combine = c;
        base = b;
        threshold = t;
        max_threads = max_t;
        complete = new std::atomic<bool>(false);
    };

    void getTasks(std::vector<PTask<I, O> *> *tasks, PTask<I, O> *parent)
    {
        I in = *(parent->input);
        if (threshold(in))
        {
            base(in, parent);
        }
        else
        {
            std::vector<PTask<I, O> *> division;
            divide(in, &division);
            for (size_t i = 0; i < division.size(); i++)
            {
                parent->dependencies.push_back(division[i]);
                // may need memcpy
                tasks->push_back(division[i]);
                getTasks(tasks, division[i]);
            }
        }
        return;
    };

    static void *worker_thread(void *args)
    {
        s *a = (s *)args;
        std::vector<LockingQueue<PTask<I, O> *> *> *b = a->worker_queues;
        int index = a->index;
        Combine combine = a->combine;
        std::atomic<bool> *complete = a->complete;
        LockingQueue<PTask<I, O> *> *queue = b->at(index);
    
        while (!complete->load())
        {
            PTask<I, O> *task = queue->dequeue();
            // if unable to steal all tasks complete
            if (!task)
            {
                // break;
                // cout << "jkjkbjkb" << index;
                if (!steal(&task, a))
                {
                    cout << "noy";
                    break;
                }else{
                    cout << (task==NULL);
                    cout<<"Stolen";
                }
            }

            bool ready = false;

            // waits for all dependncies to be ready before combining
            while (!ready)
            {

                int count = 0;
                for (int i = 0; i < task->dependencies.size(); i++)
                {
                    if (task->dependencies[i]->output.load())
                        count++;
                }
                if (count == task->dependencies.size())
                    ready = true;
            }
            //combines dependencies
            if (task->dependencies.size() == 0)
                continue;

            combine(task->dependencies, task);
            // cout << "I"<<*task->input;
            // cout << "   OUT" << *task->output;
        }
        delete (a);
        return 0;
    };

    void processTasks(std::vector<PTask<I, O> *> tasks)
    {
        complete.store(false);

        std::vector<LockingQueue<PTask<I, O> *> *> worker_queues = *(new std::vector<LockingQueue<PTask<I, O> *> *>);
        for (int i = 0; i < max_threads; i++)
        {
            worker_queues.push_back(new LockingQueue<PTask<I, O> *>);
        };

        // distribute tasks evenly amoung queues
        for (int i = 0; i < tasks.size(); i++)
        {
            worker_queues[i % (worker_queues.size())]->enqueue(tasks[i]);
        };

        // thread array for workers
        pthread_t threads[max_threads];
        for (int i = 0; i < max_threads; i++)
        {

            s *a = (s *)malloc(sizeof(struct s));
            a->worker_queues = &worker_queues;
            a->combine = this->combine;
            a->complete = &complete;
            a->index = int(i);
            pthread_create(&threads[i], NULL, &DAC::worker_thread, a);
        }
        for (int i = 0; i < max_threads; i++)
        {
            pthread_join(threads[i], NULL);
        }

        complete = true;
        cout << "All Threads Joined";
    };

    static bool steal(PTask<I, O> **task, s *a)
    {
        std::vector<LockingQueue<PTask<I, O> *> *> *b = a->worker_queues;
        int index = a->index;
        bool found = false;
        for (int i = 0; i < b->size(); i++)
        {
            if (i == index)
                continue;
            *task = b->at(i)->dequeue();
            if (*task)
            {
                return true;
            }
        }
        return false;
    };

    void run(I &in, O &result)
    {
        std::vector<PTask<I, O> *> tasks;
        PTask<I, O> *first_task = new PTask<I, O>(in);
        tasks.push_back(first_task);
        getTasks(&tasks, first_task);
        processTasks(tasks);
        result = *(first_task->output.load());
        // cout << "\nRESULT: " << result << endl;
        for(int i = 0; i < result.size(); i++){
            cout << result[i];
        }
    }
};

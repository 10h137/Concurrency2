#include "header.h"

using namespace std;

template <typename I, typename O>

class DAC
{
  typedef void * (*THREADFUNCPTR)(void *);
    typedef void (*Divide)(const PTask<I,O> &, std::vector<PTask<I, O> *> *);
    typedef void (*Combine)(std::vector<PTask<I, O> *> &, O *);
    typedef void (*Base)(const I &, PTask<I, O> *);
    typedef bool (*Threshold)(const I &);


public:

    static Divide divide;
    static Combine combine;
    static Base base;
    static Threshold threshold;
    static I input;
    static O result;
    static int max_threads;
    static std::atomic<bool> complete;


    void getTasks(const I &in, std::vector<PTask<I, O> *> *tasks, PTask<I, O> *parent)
    {
        if (threshold(in))
        {
            PTask<I, O> *res = new PTask<I, O>();
            base(in, res);
            parent->dependencies.push_back(res);
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
                getTasks(division[i]->input, tasks, division[i]);
            }
        }
        return;
    };


    static void* worker_thread(void* args){
    CQueue<PTask<I, O> *> *queue = (LockingQueue<PTask<I, O> *> *) args;
    while(!complete){
        PTask<I, O>* task = queue->dequeue();
        // if unable to steal all tasks complete 
        if(!steal(task)) {
            complete = true;
            break;
        }
        bool ready = false;
        // waits for all dependncies to be ready before combining
        while(!ready){
            int count = 0;
            for(int i = 0; i < task->dependencies.size(); i++){
                // fix to int pointer 
                if(task->dependencies[i]->output != NULL) count++;
            }
            if(count == task->dependencies.size()) ready = true;

        }
        //combines dependencies
        combine(task->dependencies,&task->output);
         // create threads 

     }
    };



    static void processTasks(std::vector<PTask<I, O> *> tasks){
        complete = false;

        LockingQueue<PTask<I, O> *> *worker_queues[max_threads];
        for(int i = 0; i < max_threads; i++){
            worker_queues[i] = new LockingQueue<PTask<I, O>* >;
        };

        // distribute tasks evenly amoung queues
        for(int i = 0; i < tasks.size(); i++){
            worker_queues[i%tasks.size()]->enqueue(tasks[i]);
        };

        // thread array for workers
        pthread_t threads[max_threads];
        for(int i = 0;i < max_threads; i++){
            pthread_create(&threads[i],NULL,&DAC::worker_thread,&worker_queues[i]);
            cout << "Thread " << i << " created";

        }
   
        while(!tasks[0]->output);
        complete = true;  
        cout << "All Threads Joined";

    };

    static bool steal(PTask<I, O> *task){

    };

    DAC(const Divide d, Combine c, Base b, Threshold t, const I &in, O &out, int max_t)
    {
        divide = d;
        combine = c;
        base = b;
        threshold = t;
        input = in;
        result = out;
        max_threads = max_t;
    };

     void run(I &in, O &result){
        std::vector<PTask<I, O> *> tasks;
        PTask<I, O>* first_task = new PTask<I, O>(in);
        tasks.push_back(first_task);
        getTasks(in, &tasks, first_task);
        processTasks(tasks);
        result = first_task->output;

    }

};
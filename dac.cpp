#include "dac.h"

template class DAC<int,int>;


template <typename I, typename O>
void DAC<I,O>::getTasks(const I &in, std::vector<PTask<I, O> *> *tasks, PTask<I, O> *parent)
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


template <typename I, typename O>
void* DAC<I,O>::worker_thread(void* args){
s* a= (s*) args;
std::vector<LockingQueue<PTask<int, int> *> *> *b = a->worker_queues;
int index = a->index;
CQueue<PTask<I, O> *> *queue = b->at(index);
while(!complete.load()){
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


    template <typename I, typename O>
    void DAC<I,O>::processTasks(std::vector<PTask<I, O> *> tasks){
        complete = false;

        std::vector< LockingQueue<PTask<I, O> *> *> worker_queues = *(new std::vector< LockingQueue<PTask<I, O> *> *>);
        for(int i = 0; i < max_threads; i++){
            worker_queues.push_back(new LockingQueue<PTask<I, O>* >);
        };

        // distribute tasks evenly amoung queues
        for(int i = 0; i < tasks.size(); i++){
            worker_queues[i%tasks.size()]->enqueue(tasks[i]);
        };

        // thread array for workers
        pthread_t threads[max_threads];
        for(int i = 0;i < max_threads; i++){
            s* a = new s();
            a->worker_queues = &worker_queues;
            a->index = i;
            pthread_create(&threads[i],NULL,&DAC::worker_thread,&a);
            cout << "Thread " << i << " created";

        }
   
        while(!tasks[0]->output);
        complete = true;  
        cout << "All Threads Joined";

    };


    template <typename I, typename O>
    static bool steal(PTask<I, O> *task){

    };

    template <typename I, typename O>
     void DAC<I,O>::run(I &in, O &result){
        std::vector<PTask<I, O> *> tasks;
        PTask<I, O>* first_task = new PTask<I, O>(in);
        tasks.push_back(first_task);
        getTasks(in, &tasks, first_task);
        processTasks(tasks);
        result = first_task->output;

    }

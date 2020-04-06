#include <vector>
#include "PTask.h"
#include "queues.h"


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
    static int max_threads;
    static std::atomic<bool> complete;

    struct s{
        int index;
        std::vector<LockingQueue<PTask<I, O> *> *> *worker_queues;

    };

    DAC(const Divide d, Combine c, Base b, Threshold t, int max_t)
    {
        divide = d;
        combine = c;
        base = b;
        threshold = t;
        max_threads = max_t;
        complete = new std::atomic<bool>(false);
    };

    void getTasks(const I &in, std::vector<PTask<I, O> *> *tasks, PTask<I, O> *parent);
    static void* worker_thread(void* args);
    static void processTasks(std::vector<PTask<I, O> *> tasks);
    static bool steal(PTask<I, O> *task);
    void run(I &in, O &result);

};



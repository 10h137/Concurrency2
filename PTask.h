#ifndef Included_NameModel_H
#define Included_NameModel_H
#include <vector>

template <typename I, typename O>
struct PTask
{
    public:
    std::vector<PTask<I, O> *> dependencies;
    I* input;
    std::atomic<O*> output;

    PTask(){}

    PTask( I *in){
        input = in; 
    }
    
};

template class PTask<int,int>;

#endif 
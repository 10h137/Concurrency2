#ifndef Included_NameModel_H
#define Included_NameModel_H
#include <vector>




// Existing code goes here




template <typename I, typename O>
struct PTask
{
    public:
    std::vector<PTask<I, O> *> dependencies;
    I input;
    O output;


    PTask(const I in){
        input = in; 
    }

    // ~PTask(){
    //     delete(output);
    // }
    
};
#endif 
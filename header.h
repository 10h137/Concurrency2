#include <stdlib.h>
#include <iostream>
#include "queues.h"
#include <pthread.h>

template <typename I, typename O>
class PTask
{
    public:
    std::vector<PTask<I, O> *> dependencies;
    I input;
    O output;

 PTask(){
    }

    PTask(const I in){
        input = in; 
    }


    // ~PTask(){
    //     delete(output);
    // }
    
};

class FIB{
public:

 
   static void divide(const PTask<int, int> &in, std::vector<PTask<int, int> *> *out){
      (*out).push_back(new PTask<int, int>(in.input-1));
      (*out).push_back(new PTask<int, int>(in.input-2));
   };

   static void combine(std::vector<PTask<int, int> *> &in, int *out){
      *out = in[0]->output + in[1]->output;
   };

   static void base(const int &in, PTask<int, int> *out){
      (*out).output = 1;
   };

   static bool threshold(const int &in){
    return (in<2);
   };

};


template <typename I, typename O>
class Alg{   
    public:
    virtual void divide(PTask<I, O> &in, std::vector<PTask<I, O> > &out) = 0;
    virtual void combine(std::vector<PTask<I, O> > &in, O &out) = 0;
    virtual void base(const I &in, PTask<I, O> &out) = 0;
    virtual bool threshold(const I &in) = 0;
    virtual ~Alg() {}


};


#ifndef Included_NameModel_Y
#define Included_NameModel_Y

#include <vector>
#include "PTask.h"
#include <iostream>

class CFib {
public:


static void divide(const PTask<int, int> &in, std::vector<PTask<int, int> *> *out){
    out->push_back(new PTask<int, int>(new int((*in.input)-1)));
    out->push_back(new PTask<int, int>(new int((*in.input)-2)));
};

static void combine(std::vector<PTask<int, int> *> &in, PTask<int, int> *out){
    out->output.store(new int(*(in[0]->output) + *(in[1]->output)));

};

static void base(const int &in, PTask<int, int> *out){
    out->output.store(new int(1) );
};

static bool threshold(const int &in){
    return (in<2);
};

};
#endif 
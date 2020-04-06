 #include "CFib.h";

void CFib::divide(const PTask<int, int> &in, std::vector<PTask<int, int> *> *out){
    (*out).push_back(new PTask<int, int>(in.input-1));
    (*out).push_back(new PTask<int, int>(in.input-2));
};

void CFib::combine(std::vector<PTask<int, int> *> &in, int *out){
    *out = in[0]->output + in[1]->output;
};

void CFib::base(const int &in, PTask<int, int> *out){
    (*out).output = 1;
};

bool CFib::threshold(const int &in){
    return (in<2);
};
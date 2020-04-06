#include "dac.h"

static void run(){
    int in = 10;
    int out;
    DAC<int,int> *d = new DAC<int,int>(FIB::divide,FIB::combine, FIB::base, FIB::threshold, in, out, 4);
    (*d).run(in, out);
};

int main(){

run();
    
};



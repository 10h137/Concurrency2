#include "dac.h"
#include "CFib.h"

static void run(){
    int in = 10;
    int out;
    DAC<int,int> *d = new DAC<int,int>(&CFib::divide,&CFib::combine, &CFib::base, &CFib::threshold, 4);
    (*d).run(in, out);
};

int main( int argc, const char* argv[] ){

run();
    
};





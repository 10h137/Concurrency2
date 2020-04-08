#include "DAC.h"
#include "CFib.h"
#include "CMerge.h"

#include <cstdlib>


int main(int argc, const char *argv[])
{
    int in = atoi(argv[1]);
    int out;
    std::vector<int> output;
static const int arr[] = {16,2,4,7,2,4,9,0,7,5,3,77,29};
vector<int> input (arr, arr + sizeof(arr) / sizeof(arr[0]) );    DAC<std::vector<int>, std::vector<int> > *d = new DAC<std::vector<int>, std::vector<int> >(&CMerge::divide, &CMerge::combine, &CMerge::base, &CMerge::threshold, atoi(argv[2]));
    (*d).run(input, output);
};

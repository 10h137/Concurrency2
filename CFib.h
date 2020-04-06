#include <vector>
#include "PTask.h"

class CFib {
public:

   static void divide(const PTask<int, int> &in, std::vector<PTask<int, int> *> *out);
   static void combine(std::vector<PTask<int, int> *> &in, int *out);
   static void base(const int &in, PTask<int, int> *out);
   static bool threshold(const int &in);

};
#include <vector>

template <typename I, typename O>
class Alg{   
    public:
    virtual void divide(PTask<I, O> &in, std::vector<PTask<I, O> > &out) = 0;
    virtual void combine(std::vector<PTask<I, O> > &in, O &out) = 0;
    virtual void base(const I &in, PTask<I, O> &out) = 0;
    virtual bool threshold(const I &in) = 0;
    virtual ~Alg() {}

};




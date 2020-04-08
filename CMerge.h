#ifndef Included_NameModel_Z
#define Included_NameModel_Z

#include <vector>
#include "PTask.h"
// #include <algorithm>    // std::sort

#include <iostream>

class CMerge
{
public:
    static void divide(const PTask<std::vector<int>, std::vector<int> > &in, std::vector<PTask<std::vector<int>, std::vector<int> > *> *out)
    {
        int mid = (in.input->size() - 1) / 2;
        PTask<std::vector<int>, std::vector<int> > *left = new PTask<std::vector<int>, std::vector<int> >;
        PTask<std::vector<int>, std::vector<int> > *right = new PTask<std::vector<int>, std::vector<int> >;

        std::vector<int> subListRight(in.input->begin() + mid, in.input->end());
        std::vector<int> subListLeft(in.input->begin(), in.input->begin() + mid);

        *left->input = subListLeft;
        *right->input = subListRight;

        out->push_back(left);
        out->push_back(right);
    };

    static void combine(std::vector<PTask<std::vector<int>, std::vector<int> > *> &in, PTask<std::vector<int>, std::vector<int> > *out)
    {
        std::vector<int> *arr;
        std::vector<int> *L = in[0]->output;
        std::vector<int> *R = in[1]->output;

        int l_index = 0;
        int r_index = 0;

        /* Merge the temp arrays back into arr[l..r]*/
        int i = 0; // Initial index of first subarray
        int j = 0; // Initial index of second subarray
        int k = 0; // Initial index of merged subarray
        while (i < L->size() && j < R->size())
        {
            if (L[i] <= R[j])
            {
                (*arr)[k] = (*L)[i];
                i++;
            }
            else
            {
                (*arr)[k] = (*R)[j];
                j++;
            }
            k++;
        }

        /* Copy the remaining elements of L[], if there 
	are any */
        while (i < L->size())
        {
            (*arr)[k] = (*L)[i];
            i++;
            k++;
        }

        /* Copy the remaining elements of R[], if there 
	are any */
        while (j < R->size())
        {
            (*arr)[k] = (*R)[j];
            j++;
            k++;
        }
        out->output.store(arr);
    };

    static void base(const std::vector<int> &in, PTask<std::vector<int>, std::vector<int> > *out)
    {
        std::vector<int> h; 
        std::copy(in.begin(), in.end(), h.begin());
        std::sort(h.begin(), h.end() );
        out->output.store(&h);

    };

    static bool threshold(const std::vector<int> &in)
    {
        return in.size() <= 2;
    };
};
#endif

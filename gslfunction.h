#ifndef GSLFUNCTIONCXX_H
#define GSLFUNCTIONCXX_H

#include <gsl/gsl_math.h>

class GslFunction
{
public:
    virtual double function(double d,void *v) = 0;

    gsl_function functor(void *v);

private:
    struct CCallbackHolder
    {
        GslFunction *cls;
        void *data;
    } _h;
    static double callback(double d, void *v);
};

#endif // GSLFUNCTIONCXX_H

#include "gslfunction.h"

gsl_function GslFunction::functor(void *v)
{
    _h.cls = this;
    _h.data = v;

    gsl_function f;
    f.function = &GslFunction::callback;
    f.params = &_h;

    return f;
}

double GslFunction::callback(double d, void *v)
{
    CCallbackHolder *h = static_cast<CCallbackHolder *>(v);
    return h->cls->function(d, h->data);
}


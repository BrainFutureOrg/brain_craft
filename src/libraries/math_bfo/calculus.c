//
// Created by kosenko on 06.02.24.
//

#include "calculus.h"
#define bfo_e 0.00001

double bfo_der(double (*f)(double), double  x0)
{
    return (f(x0 + bfo_e) - f(x0 - bfo_e)) / (2 * bfo_e);
}

double bfo_dern(double (*f)(double), double  x0, unsigned char n)
{
    if(n == 0)
    {
        return f(x0);
    }
    else
    {
        return (bfo_dern(f, x0 + bfo_e, n - 1) - bfo_dern(f, x0 - bfo_e, n - 1)) / (2 * bfo_e);
    }
}
//
// Created by the_E on 20/07/24.
//

#ifndef BRANCHES_NUMERICINTEGRATION_H
#define BRANCHES_NUMERICINTEGRATION_H

#include "../branches_core/branches_core.h"

T numericIntegration(std::string &function, T from, T to, T dx, std::map<std::string, T> &symbols)
{
    algebraParser f(function);
    bool neg = from > to;
    if (neg)std::swap(from, to);
    int slices = int((to - from) / dx);
    T sum = 0;
    for (int i = 0; i <= slices; i++)
    {
        auto M = symbols;
        M.merge(std::map<std::string, T>({{"x", from + i * dx}}));
        sum += f.evaluate(M) * dx;
    }
    return sum;
}


#endif //BRANCHES_NUMERICINTEGRATION_H

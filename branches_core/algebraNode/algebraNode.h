//
// Created by the_E on 20/05/28.
//

#ifndef BRANCHES_ALGEBRANODE_H
#define BRANCHES_ALGEBRANODE_H

#include "../branches_core.h"

class algebraNode
{
private:
protected:
    algebraNode *left;
    algebraNode *right;
public:
    std::string original;

    algebraNode();

    double compile();

    virtual double compile(std::map<std::string, double> &symMap) = 0;

    virtual std::string derivative(const std::string &direction) = 0;

    virtual std::string simplify() = 0;

    virtual std::string nodetype() = 0;

    virtual ~algebraNode();
};

#endif //BRANCHES_ALGEBRANODE_H

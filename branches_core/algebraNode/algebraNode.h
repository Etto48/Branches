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
public:
    std::string original;

    algebraNode() = default;

    T compile();

    virtual T compile(std::map<std::string, T> &symMap) = 0;

    virtual std::string derivative(const std::string &direction) = 0;

    virtual std::string simplify() = 0;

    virtual std::string nodetype() = 0;

    virtual std::vector<std::string> getVars() = 0;

    virtual ~algebraNode() = default;
};

#endif //BRANCHES_ALGEBRANODE_H

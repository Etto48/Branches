//
// Created by the_E on 20/07/29.
//

#ifndef BRANCHES_PREFIXNODE_H
#define BRANCHES_PREFIXNODE_H

#include "../branches_core.h"

class prefixNode:public algebraNode
{
private:
protected:
    char prefix;
    algebraNode *arg;
public:
    explicit prefixNode(std::string expr);

    T compile(std::map<std::string, T> &symMap) override;

    std::string derivative(const std::string &direction) override;

    std::string simplify() override;

    std::string nodetype() override;

    std::vector<std::string> getVars() override;

    ~prefixNode() override;
};


#endif //BRANCHES_PREFIXNODE_H

//
// Created by the_E on 20/05/28.
//

#ifndef BRANCHES_EXPRNODE_H
#define BRANCHES_EXPRNODE_H

#include "../branches_core.h"

class exprNode : public algebraNode
{
private:
protected:
    std::string op;
public:
    explicit exprNode(std::string expr);

    double compile(std::map<std::string, double> &symMap) override;

    std::string derivative(const std::string &direction) override;

    std::string simplify() override;

    std::string nodetype() override;
};

#endif //BRANCHES_EXPRNODE_H

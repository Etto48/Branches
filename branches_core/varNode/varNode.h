//
// Created by the_E on 20/05/28.
//

#ifndef BRANCHES_VARNODE_H
#define BRANCHES_VARNODE_H

#include "../branches_core.h"

class varNode : public algebraNode
{
private:
protected:
    bool variable;
    std::string value;
public:
    explicit varNode(const std::string &val);

    double compile(std::map<std::string, double> &symMap) override;

    std::string derivative(const std::string &direction) override;

    std::string simplify() override;

    std::string nodetype() override;

    std::vector<std::string> getVars() override;
};

#endif //BRANCHES_VARNODE_H

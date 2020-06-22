//
// Created by the_E on 20/05/28.
//

#ifndef BRANCHES_FUNCNODE_H
#define BRANCHES_FUNCNODE_H

#include "../branches_core.h"

class funcNode : public algebraNode
{
private:
protected:
    std::string func;
    algebraNode *arg;
public:
    explicit funcNode(std::string content);

    double compile(std::map<std::string, double> &symMap) override;

    std::string derivative(const std::string &direction) override;

    std::string simplify() override;

    std::string nodetype() override;

    std::vector<std::string> getVars() override;

    ~funcNode();
};

#endif //BRANCHES_FUNCNODE_H

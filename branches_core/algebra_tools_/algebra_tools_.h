//
// Created by the_E on 20/05/28.
//

#ifndef BRANCHES_ALGEBRA_TOOLS__H
#define BRANCHES_ALGEBRA_TOOLS__H

#include "../branches_core.h"
#include "../algebraNode/algebraNode.h"
#include "./exception.h"

namespace algebra_tools_
{
    void removeWrappingBraces(std::string &expr);

    bool isFunction(const std::string &expr);

    bool hasOperator(const std::string &expr);

    bool goodBraces(const std::string &content);

    bool isPrefixed(const std::string &expr);

    bool noOtherSign(const std::string &expr);

    algebraNode *newAdequateNode(std::string content);

    bool strEqD(const std::string &expr, const double &d);

    std::string dtos(const double &d);
}

#endif //BRANCHES_ALGEBRA_TOOLS__H

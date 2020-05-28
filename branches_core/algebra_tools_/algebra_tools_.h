//
// Created by the_E on 20/05/28.
//

#ifndef BRANCHES_ALGEBRA_TOOLS__H
#define BRANCHES_ALGEBRA_TOOLS__H

#include "../branches_core.h"

namespace algebra_tools_
{
    class except : public std::exception
    {
    protected:
        std::string s;
    public:
        explicit except(std::string ss) : s(std::move(ss))
        {}

        ~except() noexcept override = default;

        [[nodiscard]] const char *what() const noexcept override
        { return s.c_str(); }
    };

    void removeWrappingBraces(std::string &expr);

    bool isFunction(const std::string &expr);

    bool hasOperator(const std::string &expr);

    bool goodBraces(const std::string &content);

    algebraNode *newAdequateNode(std::string content);

    bool strEqD(const std::string &expr, const double &d);

    std::string dtos(const double &d);
}

#endif //BRANCHES_ALGEBRA_TOOLS__H

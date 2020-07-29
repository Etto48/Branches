//
// Created by the_E on 20/05/08.
//

#ifndef BRANCHES_ALGEBRAPARSER_H
#define BRANCHES_ALGEBRAPARSER_H

#include "../branches_core.h"

class algebraParser
{
private:
protected:
    algebraNode *root;
public:
    explicit algebraParser(std::string expr);

    algebraParser(const algebraParser &old);

    [[maybe_unused]] T evaluate();

    T evaluate(std::map<std::string, T> symMap);

    std::string simplify();

    std::vector<std::string> getVars();

    ~algebraParser();

    static std::vector<std::string> ops()
    { return {"+", "-", "*", "/", "^"}; }//inverse order by priority
    static std::vector<std::string> functions()
    {
        return {"ceil", "floor", "ln", "log2", "log", "exp", "cosh", "sinh",
                "tanh", "acosh", "asinh", "atanh", "cos", "sin",
                "tan", "acos", "asin", "atan", "sqrt", "cbrt", "abs", "neg"};
    }//functions
    static std::vector<char> prefixes()
    {
        return {'+','-'};
    }
    static std::map<std::string, T> defaultSymMap()
    {
        return {{"e",  exp(1)},
                {"pi", 2 * acos(0)}};
    }

    std::string derivative(const std::string &direction);

};

#endif //BRANCHES_ALGEBRAPARSER_H

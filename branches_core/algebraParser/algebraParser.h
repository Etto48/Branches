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

    [[maybe_unused]] double evaluate();

    double evaluate(std::map<std::string, double> symMap);

    std::string simplify();

    ~algebraParser();

    static std::vector<std::string> ops()
    { return {"+", "-", "*", "/", "^"}; }//inverse order by priority
    static std::vector<std::string> functions()
    {
        return {"ceil", "floor", "ln", "log2", "log", "exp", "cosh", "sinh",
                "tanh", "acosh", "asinh", "atanh", "cos", "sin",
                "tan", "acos", "asin", "atan", "sqrt", "cbrt", "abs", "neg"};
    }//functions
    static std::map<std::string, double> defaultSymMap()
    {
        return {{"e",  exp(1)},
                {"pi", 2 * acos(0)}};
    }

    std::string derivative(const std::string &direction);

};

#endif //BRANCHES_ALGEBRAPARSER_H

//
// Created by the_E on 20/05/08.
//

#ifndef BRANCHES_BRANCHES_CORE_H
#define BRANCHES_BRANCHES_CORE_H
#define _USE_MATH_DEFINES

#include <string>
#include <utility>
#include <vector>
#include <iostream>
#include <cmath>
#include <map>

class algebraParser;

class algebraNode;

class varNode;

class exprNode;

class funcNode;

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
}

class algebraNode
{
private:
protected:
    algebraNode *left;
    algebraNode *right;
public:
    algebraNode();

    virtual double compile(std::map<std::string, double> &symMap) = 0;

    virtual ~algebraNode();
};

class varNode : public algebraNode
{
private:
protected:
    bool variable;
    std::string value;
public:
    explicit varNode(const std::string &val);

    double compile(std::map<std::string, double> &symMap) override;
};

class funcNode : public algebraNode
{
private:
protected:
    std::string func;
public:
    explicit funcNode(std::string content);

    double compile(std::map<std::string, double> &symMap) override;

};

class exprNode : public algebraNode
{
private:
protected:
    std::string op;
public:
    explicit exprNode(std::string expr);

    double compile(std::map<std::string, double> &symMap) override;
};

class algebraParser
{
private:
protected:
    algebraNode *root;
public:
    explicit algebraParser(std::string expr);

    [[maybe_unused]] double evaluate();

    double evaluate(std::map<std::string, double> symMap);

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

};


#endif //BRANCHES_BRANCHES_CORE_H

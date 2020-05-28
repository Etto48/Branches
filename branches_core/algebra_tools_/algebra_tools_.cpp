//
// Created by the_E on 20/05/28.
//

#include "../algebraParser/algebraParser.h"

using namespace std;

///algebra_tools_
void algebra_tools_::removeWrappingBraces(string &expr)
{
    goodBraces(expr);
    while (expr[0] == '(' && expr[expr.length() - 1] == ')')
    {
        if (expr.length() == 1)return;
        if (expr[0] == '(' && expr[expr.length() - 1] == ')')
        {
//cout<<"Removing braces from "<<expr<<" at indexes "<<0<<" and "<<expr.length()-1<<endl;
            if (expr.length() == 2)throw except("Empty Braces");
            unsigned depth = 0, conj = 0, i = 0;
            for (auto &c : expr)///find conj of first brace
            {
                if (c == '(')
                {
                    depth += 1;
                } else if (c == ')')
                {
                    depth -= 1;
                    if (depth == 0)
                    {
                        conj = i;
                        break;
                    }
                }
                i += 1;
            }

            if (conj == expr.length() - 1)
            {
                expr.erase(0, 1);
                expr.erase(expr.length() - 1, 1);
            } else
            {
                return;
            }
        } else
        {
            return;
        }
    }
}

bool algebra_tools_::isFunction(const string &expr)
{
    bool startsFun = false;
    string func;
    for (auto &fun: algebraParser::functions())
    {
        if (expr.substr(0, fun.length()) == fun)
        {
            //cout<<"Function "<<fun<<" found  by parser"<<endl;
            startsFun = true;
            func = fun;
            break;
        }
    }
    bool isFunction = false;
    if (startsFun)
    {
        string ck = expr.substr(func.length());
        //cout<<"Checking function on string "<<ck<<endl;
        if (ck[0] == '(')
        {
            unsigned cdepth = 0, conj = 0, ci = 0;
            for (auto &c : ck)///find conj of first brace
            {
                if (c == '(')
                {
                    cdepth += 1;
                } else if (c == ')')
                {
                    cdepth -= 1;
                    if (cdepth == 0)
                    {
                        conj = ci;
                        break;
                    }
                }
                ci += 1;
            }
            if (conj == ck.length() - 1)
            {
                isFunction = true;
            }
        }
    }
    return isFunction;
}

bool algebra_tools_::hasOperator(const string &expr)
{
    bool hasIt = false;
    string allOps;
    for (auto &s : algebraParser::ops())allOps.append(s);
    for (auto &o : allOps)
    {
        if (expr.find(o) != string::npos)
        {
            hasIt = true;
            break;
        }
    }
    return hasIt;
}

bool algebra_tools_::goodBraces(const std::string &content)
{
    unsigned openBr = 0, closedBr = 0, depth = 0;
    for (auto &c : content)//find depth
    {
        if (c == '(')
        {
            openBr += 1;
            depth += 1;
        } else if (c == ')')
        {
            closedBr += 1;
            depth -= 1;
        }
        if (depth < 0)
            throw algebra_tools_::except("Unbalanced Braces");

    }
    if (openBr != closedBr)
        throw algebra_tools_::except("Unbalanced Braces");
    return true;
}

std::string algebra_tools_::dtos(const double &d)
{
    if (d == int(d))
    {
        return to_string(int(d));
    } else
        return to_string(d);
}

algebraNode *algebra_tools_::newAdequateNode(std::string content)
{
    //check for balanced braces
    algebra_tools_::goodBraces(content);
    algebra_tools_::removeWrappingBraces(content);
    bool contentIsFunction = algebra_tools_::isFunction(content);
    bool contentHasOperator = algebra_tools_::hasOperator(content);


    if (contentIsFunction)
        return new funcNode(content);
    else if (!contentHasOperator)
        return new varNode(content);
    else
        return new exprNode(content);

}

bool algebra_tools_::strEqD(const std::string &expr, const double &d)
{
    if (expr.find_first_not_of(".0123456789") != std::string::npos)return false;
    else if (stod(expr) == d)return true;
    return false;
}
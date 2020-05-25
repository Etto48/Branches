//
// Created by the_E on 20/05/24.
//

#include "branches_core.h"

using namespace std;
///algebra_tools_
namespace algebra_tools_
{

}

//operators
//functions
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

///algebraNode
algebraNode::algebraNode()
{
    left = right = nullptr;
}

algebraNode::~algebraNode()
{
    delete left;
    delete right;
}

///varNode
varNode::varNode(const string &val)
{
    //cout<<"converting: "<<val<<endl;
    value = val;
    variable = value.find_first_not_of("0123456789") != std::string::npos;
    //cout<<"val:"<<value<<" var:"<<variable<<endl;
}

double varNode::compile(map<string, double> &symMap)
{
    if (variable) return symMap[value];
    else return stod(value);//I hope it works (it didn't)(sike, super reliable)
}

///funcNode
funcNode::funcNode(string content)
{
    //cout<<"Found function:"<<content<<endl;
    right = nullptr;
    for (auto &f : algebraParser::functions())
    {
        if (content.substr(0, f.length()) == f)
        {
            func = f;
            break;
        }
    }
    content = content.substr(func.length());
    //cout<<"Inner: "<<content<<endl;
    left = algebra_tools_::newAdequateNode(content);

}

double funcNode::compile(map<string, double> &symMap)
{
    if (func == "ln")
        return log(left->compile(symMap));
    if (func == "log2")
        return log2(left->compile(symMap));
    if (func == "log")
        return log10(left->compile(symMap));
    if (func == "cos")
        return cos(left->compile(symMap));
    if (func == "sin")
        return sin(left->compile(symMap));
    if (func == "abs")
        return abs(left->compile(symMap));
    if (func == "neg")
        return -(left->compile(symMap));
    else
        throw algebra_tools_::except("funcNode: Invalid Function Name");
}

///exprNode
exprNode::exprNode(string expr)
{
    //cout<<"Found expr:"<<expr<<endl;
    algebra_tools_::removeWrappingBraces(expr);

    int depth = 0;
    unsigned fmeo = -1;
    unsigned i;
    bool found = false;
    for (auto &pop: algebraParser::ops())
    {
        i = 0;
        for (auto &c : expr)///find first most extern operator
        {
            if (c == '(')depth += 1;
            else if (c == ')')depth -= 1;
            else if (depth == 0 && pop.find(c) != string::npos)
            {
                //cout<<"found operator "<<c<<" in "<<expr<<endl;
                fmeo = i;
                found = true;
                break;
            }
            i += 1;
        }
        if (found)
            break;
    }
    if (fmeo <= 0 || fmeo >= expr.length() - 1)
        throw algebra_tools_::except(
                "exprNode: Invalid Operator Location");
    string left, right;


    ///super wrong

    left = expr.substr(0, fmeo);
    right = expr.substr(fmeo + 1, expr.length() - fmeo - 1);

    op = expr.substr(fmeo, 1);

    algebraNode::left = algebra_tools_::newAdequateNode(left);
    algebraNode::right = algebra_tools_::newAdequateNode(right);
}

double exprNode::compile(map<string, double> &symMap)
{
    if (op == "+")
        return left->compile(symMap) + right->compile(symMap);
    if (op == "-")
        return left->compile(symMap) - right->compile(symMap);
    if (op == "*")
        return left->compile(symMap) * right->compile(symMap);
    if (op == "/")
        return left->compile(symMap) / right->compile(symMap);
    if (op == "^")
        return pow(left->compile(symMap), right->compile(symMap));
    else
        throw algebra_tools_::except("exprNode: Invalid Operator Symbol");
}

///algebraParser
algebraParser::algebraParser(string expr)
{

    root = nullptr;
    algebra_tools_::removeWrappingBraces(expr);
    //cout<<expr<<endl;
    root = algebra_tools_::newAdequateNode(expr);
}

[[maybe_unused]] double algebraParser::evaluate()
{
    map<string, double> dummyMap;
    return evaluate(dummyMap);
}

double algebraParser::evaluate(map<string, double> symMap)
{
    symMap.merge(algebraParser::defaultSymMap());
    return root->compile(symMap);
}

algebraParser::~algebraParser()
{
    delete root;
}



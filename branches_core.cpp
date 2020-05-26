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
    original = value = val;
    variable = value.find_first_not_of("0123456789") != std::string::npos;
}

double varNode::compile(map<string, double> &symMap)
{
    if (variable) return symMap[value];
    else return stod(value);//I hope it works (it didn't)(sike, super reliable)
}

std::string varNode::derivative(const string &direction)
{
    return variable ? (direction == value ? "1" : "0") : "0";
}

///funcNode
funcNode::funcNode(string content)
{
    //cout<<"Found function:"<<content<<endl;
    original = content;
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
    if (func == "ceil")
        return ceil(left->compile(symMap));
    if (func == "floor")
        return floor(left->compile(symMap));
    if (func == "ln")
        return log(left->compile(symMap));
    if (func == "log2")
        return log2(left->compile(symMap));
    if (func == "log")
        return log10(left->compile(symMap));
    if (func == "exp")
        return exp(left->compile(symMap));
    if (func == "cosh")
        return cosh(left->compile(symMap));
    if (func == "sinh")
        return sinh(left->compile(symMap));
    if (func == "tanh")
        return tanh(left->compile(symMap));
    if (func == "acosh")
        return acosh(left->compile(symMap));
    if (func == "asinh")
        return asinh(left->compile(symMap));
    if (func == "atanh")
        return atanh(left->compile(symMap));
    if (func == "cos")
        return cos(left->compile(symMap));
    if (func == "sin")
        return sin(left->compile(symMap));
    if (func == "tan")
        return tan(left->compile(symMap));
    if (func == "acos")
        return acos(left->compile(symMap));
    if (func == "asin")
        return asin(left->compile(symMap));
    if (func == "atan")
        return atan(left->compile(symMap));
    if (func == "sqrt")
        return sqrt(left->compile(symMap));
    if (func == "cbrt")
        return cbrt(left->compile(symMap));
    if (func == "abs")
        return abs(left->compile(symMap));
    if (func == "neg")
        return -(left->compile(symMap));
    else
        throw algebra_tools_::except("funcNode: Invalid Function Name");
}

std::string funcNode::derivative(const string &direction)
{
    if (func == "ceil")
        return "0";
    if (func == "floor")
        return "0";
    if (func == "ln")
    {
        auto d = left->derivative(direction);
        auto a = left->original;

        if (d == "0")
            return "0";
        else
            return "((" + d + ")/(" + a + "))";
    }
    if (func == "log2")
    {
        auto d = left->derivative(direction);
        auto a = left->original;
        if (d == "0")
            return "0";
        else
            return "((" + d + ")/((" + a + ")*ln(2)))";
    }
    if (func == "log")
    {
        auto d = left->derivative(direction);
        auto a = left->original;
        if (d == "0")
            return "0";
        else
            return "((" + d + ")/((" + a + ")*ln(10)))";
    }
    if (func == "exp")
    {
        auto d = left->derivative(direction);
        auto a = left->original;
        if (d == "0")
            return "0";
        else
            return "(exp(" + a + ")*(" + d + "))";
    }
    if (func == "cosh")
    {
        auto d = left->derivative(direction);
        auto a = left->original;
        if (d == "0")
            return "0";
        else
            return "(sinh(" + a + ")*(" + d + "))";
    }
    if (func == "sinh")
    {
        auto d = left->derivative(direction);
        auto a = left->original;
        if (d == "0")
            return "0";
        else
            return "(cosh(" + a + ")*(" + d + "))";
    }
    if (func == "tanh")
    {
        auto d = left->derivative(direction);
        auto a = left->original;
        if (d == "0")
            return "0";
        else
            return "((1-tanh(" + a + ")^2)*(" + d + "))";
    }
    if (func == "acosh")
    {
        auto d = left->derivative(direction);
        auto a = left->original;
        if (d == "0")
            return "0";
        else
            return "((" + d + ")/sqrt((" + a + ")^2-1))";
    }
    if (func == "asinh")
    {
        auto d = left->derivative(direction);
        auto a = left->original;
        if (d == "0")
            return "0";
        else
            return "((" + d + ")/sqrt((" + a + ")^2+1))";
    }
    if (func == "atanh")
    {
        auto d = left->derivative(direction);
        auto a = left->original;
        if (d == "0")
            return "0";
        else
            return "((" + d + ")/(1-(" + a + ")^2))";
    }
    if (func == "cos")
    {
        auto d = left->derivative(direction);
        auto a = left->original;
        if (d == "0")
            return "0";
        else
            return "(neg(sin(" + a + "))*(" + d + "))";
    }
    if (func == "sin")
    {
        auto d = left->derivative(direction);
        auto a = left->original;
        if (d == "0")
            return "0";
        else
            return "(cos(" + a + ")*(" + d + "))";
    }
    if (func == "tan")
    {
        auto d = left->derivative(direction);
        auto a = left->original;
        if (d == "0")
            return "0";
        else
            return "((" + d + ")/(tan(" + a + ")^2))";
    }
    if (func == "acos")
    {
        auto d = left->derivative(direction);
        auto a = left->original;
        if (d == "0")
            return "0";
        else
            return "(neg((" + d + ")/sqrt(1-(" + a + ")^2)))";
    }
    if (func == "asin")
    {
        auto d = left->derivative(direction);
        auto a = left->original;
        if (d == "0")
            return "0";
        else
            return "((" + d + ")/sqrt(1-(" + a + ")^2))";
    }
    if (func == "atan")
    {
        auto d = left->derivative(direction);
        auto a = left->original;
        if (d == "0")
            return "0";
        else
            return "((" + d + ")/(1+(" + a + ")^2))";
    }
    if (func == "sqrt")
    {
        auto d = left->derivative(direction);
        auto a = left->original;
        if (d == "0")
            return "0";
        else
            return "((" + d + ")/(2*(" + a + ")^(1/2)))";
    }
    if (func == "cbrt")
    {
        auto d = left->derivative(direction);
        auto a = left->original;
        if (d == "0")
            return "0";
        else
            return "((" + d + ")/(3*(" + a + ")^(2/3)))";
    }
    if (func == "abs")
    {
        auto d = left->derivative(direction);
        auto a = left->original;
        if (d == "0")
            return "0";
        else
            return "((abs(" + a + ")/(" + a + "))*(" + d + "))";
    }
    if (func == "neg")
    {
        auto d = left->derivative(direction);
        auto a = left->original;
        if (d == "0")
            return "0";
        else
            return "(neg(" + d + "))";
    } else
        throw algebra_tools_::except("funcNode: Invalid Function Name");
}

///exprNode
exprNode::exprNode(string expr)
{
    //cout<<"Found expr:"<<expr<<endl;

    original = expr;

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

    if (op == "/" && right == "0")
        throw algebra_tools_::except("Divide by 0");

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

std::string exprNode::derivative(const string &direction)
{
    if (op == "+")
    {
        auto ld = left->derivative(direction);
        auto rd = right->derivative(direction);
        if (ld == "0" && rd == "0")
            return "0";
        else if (ld == "0")
            return "(" + rd + ")";
        else if (rd == "0")
            return "(" + ld + ")";
        else
            return "(" + ld + ")+(" + rd + ")";
    } else if (op == "-")
    {
        auto ld = left->derivative(direction);
        auto rd = right->derivative(direction);
        if (ld == "0" && rd == "0")
            return "0";
        else if (ld == "0")
            return "neg(" + rd + ")";
        else if (rd == "0")
            return "(" + ld + ")";
        else
            return "(" + ld + ")-(" + rd + ")";
    } else if (op == "*")
    {
        auto ld = left->derivative(direction);
        auto rd = right->derivative(direction);
        auto l = left->original;
        auto r = right->original;

        if (ld != "0" && rd != "0" && r != "0" && l != "0")
            return "(" + ld + ")*(" + r + ")+(" + l + ")*(" + rd + ")";
        else if ((ld == "0" || r == "0") && l != "0" && rd != "0")
            return "(" + l + ")*(" + rd + ")";
        else if ((l == "0" || rd == "0") && ld != "0" && r != "0")
            return "(" + ld + ")*(" + r + ")";
        else
            return "0";
    } else if (op == "/")
    {
        auto ld = left->derivative(direction);
        auto rd = right->derivative(direction);
        auto l = left->original;
        auto r = right->original;

        if ((ld == "0" && rd == "0") || (l == "0" && r != "0"))
            return "0";
        else if (ld != "0" && rd != "0" && r != "0" && l != "0")
            return "((" + ld + ")*(" + r + ")-(" + l + ")*(" + rd + "))/(" + r + ")^2";
        else if ((ld == "0") && l != "0" && rd != "0")
            return "((" + l + ")*(" + rd + "))/(" + r + ")^2";
        else if ((l == "0" || rd == "0") && ld != "0" && r != "0")
            return "((" + ld + ")*(" + r + "))/(" + r + ")^2";
        else
            throw algebra_tools_::except("Divide by 0");
    } else if (op == "^")
    {
        auto ld = left->derivative(direction);
        auto rd = right->derivative(direction);
        auto l = left->original;
        auto r = right->original;

        if (rd == "0" && ld == "0")
            return "0";
        else if (rd == "0")
            return "((" + r + ")*(" + l + ")^(" + r + "-1))*(" + ld + ")";
        else if (ld == "0")
            return "((" + l + ")^(" + r + ")*((" + rd + "*ln(" + l + "))";
        else
            return "((" + l + ")^(" + r + ")*((" + rd + "*ln(" + l + ")+(" + r + "*" + ld + ")/(" + l + "))";
    } else
        throw algebra_tools_::except("Invalid operator");
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

std::string algebraParser::derivative(const string &direction)
{
    return root->derivative(direction);
}



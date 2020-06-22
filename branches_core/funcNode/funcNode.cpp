//
// Created by the_E on 20/05/28.
//

#include "../algebraParser/algebraParser.h"
#include "funcNode.h"


using namespace std;

///funcNode
funcNode::funcNode(string content)
{
    //cout<<"Found function:"<<content<<endl;
    original = content;
    arg = nullptr;

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
    arg = algebra_tools_::newAdequateNode(content);

}

double funcNode::compile(map<string, double> &symMap)
{
    if (func == "ceil")
        return ceil(arg->compile(symMap));
    if (func == "floor")
        return floor(arg->compile(symMap));
    if (func == "ln")
        return log(arg->compile(symMap));
    if (func == "log2")
        return log2(arg->compile(symMap));
    if (func == "log")
        return log10(arg->compile(symMap));
    if (func == "exp")
        return exp(arg->compile(symMap));
    if (func == "cosh")
        return cosh(arg->compile(symMap));
    if (func == "sinh")
        return sinh(arg->compile(symMap));
    if (func == "tanh")
        return tanh(arg->compile(symMap));
    if (func == "acosh")
        return acosh(arg->compile(symMap));
    if (func == "asinh")
        return asinh(arg->compile(symMap));
    if (func == "atanh")
        return atanh(arg->compile(symMap));
    if (func == "cos")
        return cos(arg->compile(symMap));
    if (func == "sin")
        return sin(arg->compile(symMap));
    if (func == "tan")
        return tan(arg->compile(symMap));
    if (func == "acos")
        return acos(arg->compile(symMap));
    if (func == "asin")
        return asin(arg->compile(symMap));
    if (func == "atan")
        return atan(arg->compile(symMap));
    if (func == "sqrt")
        return sqrt(arg->compile(symMap));
    if (func == "cbrt")
        return cbrt(arg->compile(symMap));
    if (func == "abs")
        return abs(arg->compile(symMap));
    if (func == "neg")
        return -(arg->compile(symMap));
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
        auto d = arg->derivative(direction);
        auto a = arg->original;

        if (d == "0")
            return "0";
        else
            return "((" + d + ")/(" + a + "))";
    }
    if (func == "log2")
    {
        auto d = arg->derivative(direction);
        auto a = arg->original;
        if (d == "0")
            return "0";
        else
            return "((" + d + ")/((" + a + ")*ln(2)))";
    }
    if (func == "log")
    {
        auto d = arg->derivative(direction);
        auto a = arg->original;
        if (d == "0")
            return "0";
        else
            return "((" + d + ")/((" + a + ")*ln(10)))";
    }
    if (func == "exp")
    {
        auto d = arg->derivative(direction);
        auto a = arg->original;
        if (d == "0")
            return "0";
        else
            return "(exp(" + a + ")*(" + d + "))";
    }
    if (func == "cosh")
    {
        auto d = arg->derivative(direction);
        auto a = arg->original;
        if (d == "0")
            return "0";
        else
            return "(sinh(" + a + ")*(" + d + "))";
    }
    if (func == "sinh")
    {
        auto d = arg->derivative(direction);
        auto a = arg->original;
        if (d == "0")
            return "0";
        else
            return "(cosh(" + a + ")*(" + d + "))";
    }
    if (func == "tanh")
    {
        auto d = arg->derivative(direction);
        auto a = arg->original;
        if (d == "0")
            return "0";
        else
            return "((1-tanh(" + a + ")^2)*(" + d + "))";
    }
    if (func == "acosh")
    {
        auto d = arg->derivative(direction);
        auto a = arg->original;
        if (d == "0")
            return "0";
        else
            return "((" + d + ")/sqrt((" + a + ")^2-1))";
    }
    if (func == "asinh")
    {
        auto d = arg->derivative(direction);
        auto a = arg->original;
        if (d == "0")
            return "0";
        else
            return "((" + d + ")/sqrt((" + a + ")^2+1))";
    }
    if (func == "atanh")
    {
        auto d = arg->derivative(direction);
        auto a = arg->original;
        if (d == "0")
            return "0";
        else
            return "((" + d + ")/(1-(" + a + ")^2))";
    }
    if (func == "cos")
    {
        auto d = arg->derivative(direction);
        auto a = arg->original;
        if (d == "0")
            return "0";
        else
            return "(neg(sin(" + a + "))*(" + d + "))";
    }
    if (func == "sin")
    {
        auto d = arg->derivative(direction);
        auto a = arg->original;
        if (d == "0")
            return "0";
        else
            return "(cos(" + a + ")*(" + d + "))";
    }
    if (func == "tan")
    {
        auto d = arg->derivative(direction);
        auto a = arg->original;
        if (d == "0")
            return "0";
        else
            return "((" + d + ")/(tan(" + a + ")^2))";
    }
    if (func == "acos")
    {
        auto d = arg->derivative(direction);
        auto a = arg->original;
        if (d == "0")
            return "0";
        else
            return "(neg((" + d + ")/sqrt(1-(" + a + ")^2)))";
    }
    if (func == "asin")
    {
        auto d = arg->derivative(direction);
        auto a = arg->original;
        if (d == "0")
            return "0";
        else
            return "((" + d + ")/sqrt(1-(" + a + ")^2))";
    }
    if (func == "atan")
    {
        auto d = arg->derivative(direction);
        auto a = arg->original;
        if (d == "0")
            return "0";
        else
            return "((" + d + ")/(1+(" + a + ")^2))";
    }
    if (func == "sqrt")
    {
        auto d = arg->derivative(direction);
        auto a = arg->original;
        if (d == "0")
            return "0";
        else
            return "((" + d + ")/(2*(" + a + ")^(1/2)))";
    }
    if (func == "cbrt")
    {
        auto d = arg->derivative(direction);
        auto a = arg->original;
        if (d == "0")
            return "0";
        else
            return "((" + d + ")/(3*(" + a + ")^(2/3)))";
    }
    if (func == "abs")
    {
        auto d = arg->derivative(direction);
        auto a = arg->original;
        if (d == "0")
            return "0";
        else
            return "((abs(" + a + ")/(" + a + "))*(" + d + "))";
    }
    if (func == "neg")
    {
        auto d = arg->derivative(direction);
        auto a = arg->original;
        if (d == "0")
            return "0";
        else
            return "(neg(" + d + "))";
    } else
        throw algebra_tools_::except("funcNode: Invalid Function Name");
}

std::string funcNode::simplify()
{
    string ret;
    try
    {
        double v = arg->compile();
        if (v < 0)
            ret = "neg(" + algebra_tools_::dtos(-v) + ")";
        else
            ret = algebra_tools_::dtos(v);
    } catch (algebra_tools_::except &)
    {
        ret = arg->simplify();
        algebra_tools_::removeWrappingBraces(ret);
    }
    return func + "(" + ret + ")";
}

std::string funcNode::nodetype()
{
    return "func";
}

std::vector<std::string> funcNode::getVars()
{
    return arg->getVars();
}

funcNode::~funcNode()
{
    delete arg;
}

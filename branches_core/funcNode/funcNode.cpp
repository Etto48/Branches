//
// Created by the_E on 20/05/28.
//

#include "../algebraParser/algebraParser.h"

using namespace std;

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

std::string funcNode::simplify()
{
    string ret;
    try
    {
        double v = left->compile();
        if (v < 0)
            ret = "neg(" + algebra_tools_::dtos(-v) + ")";
        else
            ret = algebra_tools_::dtos(v);
    } catch (algebra_tools_::except &)
    {
        ret = left->simplify();
        algebra_tools_::removeWrappingBraces(ret);
    }
    return func + "(" + ret + ")";
}

std::string funcNode::nodetype()
{
    return "func";
}
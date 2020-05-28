//
// Created by the_E on 20/05/28.
//

#include "../algebraParser/algebraParser.h"

using namespace std;

///varNode
varNode::varNode(const string &val)
{
    original = value = val;
    variable = value.find_first_not_of(".0123456789") != std::string::npos;
}

double varNode::compile(map<string, double> &symMap)
{
    if (variable)
    {
        if (symMap.contains(value))
            return symMap[value];
        else
            throw algebra_tools_::except("Not recognized symbol \"" + value + "\"");
    } else return stod(value);//I hope it works (it didn't)(sike, super reliable)
}

std::string varNode::derivative(const string &direction)
{
    return variable ? (direction == value ? "1" : "0") : "0";
}

std::string varNode::simplify()
{
    return value;
}

std::string varNode::nodetype()
{
    return "var";
}
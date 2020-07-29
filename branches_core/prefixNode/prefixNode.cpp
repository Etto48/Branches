//
// Created by the_E on 20/07/29.
//

#include "prefixNode.h"

prefixNode::prefixNode(std::string expr)
{
    original = expr;
    for(const auto& p:algebraParser::prefixes())
    {
        if(expr[0]==p)
        {
            prefix = p;
            expr.erase(0,1);
            arg=algebra_tools_::newAdequateNode(expr);
            break;
        }
    }
}

T prefixNode::compile(std::map<std::string, T> &symMap)
{
    if(prefix=='+')
        return arg->compile(symMap);
    else if(prefix=='-')
        return -arg->compile(symMap);
    else
        throw algebra_tools_::except("Not supported prefix");
}

std::string prefixNode::derivative(const std::string &direction)
{
    if(prefix=='+')
        return arg->derivative(direction);
    else if(prefix=='-')
        return "-("+arg->derivative(direction)+")";
    else
        throw algebra_tools_::except("Not supported prefix");
}

std::string prefixNode::simplify()
{
    if(prefix=='+')
        return arg->simplify();
    else if(prefix=='-')
        return "-("+arg->simplify()+")";
    else
        throw algebra_tools_::except("Not supported prefix");
}

std::string prefixNode::nodetype()
{
    return "prefix";
}

std::vector<std::string> prefixNode::getVars()
{
    return arg->getVars();
}

prefixNode::~prefixNode()
{
    delete arg;
}

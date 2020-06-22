//
// Created by the_E on 20/05/24.
//

#include "algebraParser.h"

using namespace std;

///algebraParser
algebraParser::algebraParser(string expr)
{

    root = nullptr;
    algebra_tools_::removeWrappingBraces(expr);
    //cout<<expr<<endl;
    root = algebra_tools_::newAdequateNode(expr);
}

[[maybe_unused]] T algebraParser::evaluate()
{
    map<string, T> dummyMap;
    return evaluate(dummyMap);
}

T algebraParser::evaluate(map<string, T> symMap)
{
    symMap.merge(algebraParser::defaultSymMap());
    return root->compile(symMap);
}

std::string algebraParser::derivative(const string &direction)
{
    return algebraParser(root->derivative(algebraParser(direction).simplify())).simplify();
    //return root->derivative(direction);
}

std::string algebraParser::simplify()
{
    return root->simplify();
}

std::vector<std::string> algebraParser::getVars()
{
    return root->getVars();

}

algebraParser::~algebraParser()
{
    delete root;
}

algebraParser::algebraParser(const algebraParser &old)
{
    root = algebra_tools_::newAdequateNode(old.root->original);
}

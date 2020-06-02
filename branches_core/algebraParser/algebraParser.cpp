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
    return algebraParser(root->derivative(algebraParser(direction).simplify())).simplify();
    //return root->derivative(direction);
}

std::string algebraParser::simplify()
{
    return root->simplify();
}



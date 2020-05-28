//
// Created by the_E on 20/05/28.
//

#include "../algebraParser/algebraParser.h"

using namespace std;

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

double algebraNode::compile()
{
    auto dummyMap = algebraParser::defaultSymMap();
    return compile(dummyMap);
}
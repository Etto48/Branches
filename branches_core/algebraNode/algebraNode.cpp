//
// Created by the_E on 20/05/28.
//

#include "../algebraParser/algebraParser.h"

using namespace std;

///algebraNode

T algebraNode::compile()
{
    auto dummyMap = algebraParser::defaultSymMap();
    return compile(dummyMap);
}
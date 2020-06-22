//
// Created by the_E on 20/06/02.
//

#include "vectorParser.h"
#include <sstream>

using namespace std;

vectorParser::vectorParser(std::string str)
{

    //expected input {<f(x,y,z)>,<g(x,y,z)>,...}
    if (str[0] != '{' || str[str.length() - 1] != '}')
        throw algebra_tools_::except("Wrong vector format");

    //remove braces
    str = str.substr(1, str.length() - 2);

    stringstream ss(str);
    string item;
    vector<string> sStrings;

    while (getline(ss, item, ','))
    {
        sStrings.push_back(item);
    }

    for (auto &s:sStrings)
    {
        comps.emplace_back(s);
    }

}

vector<string> vectorParser::getVars()
{
    vector<string> ret;
    for (auto &c:comps)
    {
        auto ret2 = c.getVars();
        ret.insert(ret.end(), ret2.begin(), ret2.end());
    }
    return ret;
}

unsigned vectorParser::getDimensions()
{
    return comps.size();
}

DT vectorParser::evaluate(const map<string, T> &args)
{
    DT ret(getDimensions());
    unsigned i = 0;
    for (auto &p:comps)
    {
        ret[i] = p.evaluate(args);
        i++;
    }
    return ret;
}

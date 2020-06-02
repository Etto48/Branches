//
// Created by the_E on 20/06/02.
//

#ifndef BRANCHES_VECTORPARSER_H
#define BRANCHES_VECTORPARSER_H

#include <vector>
#include <string>
#include <map>
#include "../branches_core.h"

class vectorParser
{
private:
    std::vector<algebraParser> comps;
public:
    vectorParser(std::string);

    std::vector<std::string> getVars();

    unsigned getDimensions();

    std::vector<double> evaluate(std::map<std::string, std::string> args);
};


#endif //BRANCHES_VECTORPARSER_H

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
    explicit vectorParser(std::string str);

    std::vector<std::string> getVars();

    unsigned getDimensions();

    DT evaluate(const std::map<std::string, T> &args);
};


#endif //BRANCHES_VECTORPARSER_H

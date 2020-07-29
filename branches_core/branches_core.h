//
// Created by the_E on 20/05/28.
//

#ifndef BRANCHES_BRANCHES_CORE_H
#define BRANCHES_BRANCHES_CORE_H

#include <string>
#include <utility>
#include <vector>
#include <iostream>
#include <cmath>
#include <map>
#include "DataTypes/DataTypes.h"

typedef double T;
typedef vec<T> DT;

class algebraParser;

class vectorParser;

class algebraNode;

class varNode;

class exprNode;

class prefixNode;

#include "algebra_tools_/algebra_tools_.h"
#include "algebraParser/algebraParser.h"
#include "vectorParser/vectorParser.h"

#include "algebraNode/algebraNode.h"
#include "varNode/varNode.h"
#include "exprNode/exprNode.h"
#include "funcNode/funcNode.h"
#include "prefixNode/prefixNode.h"

#endif //BRANCHES_BRANCHES_CORE_H

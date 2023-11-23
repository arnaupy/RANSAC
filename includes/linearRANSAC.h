#ifndef LINEARRANSAC_H
#define LINEARRANSAC_H
#include <iostream>
#include "utils.h"

int linearRANSAC(Data2D data,
                 std::string resultsFilepath,
                 int nSamples,
                 double threshold,
                 double lr,
                 CostFunctions costFunction);

#endif
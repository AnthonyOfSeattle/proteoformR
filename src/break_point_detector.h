#ifndef BREAK_POINT_DETECTOR_H
#define BREAK_POINT_DETECTOR_H

#include <Rcpp.h>
#include <vector>
#include <string>
#include "dynamic_container.h"
using namespace Rcpp;

class BreakpointDetector{
  std::vector<DynamicContainer*> lookup_table_;
  NumericMatrix values_;
  double penalty_;
public:
  BreakpointDetector(NumericMatrix , double, std::string);
  void FindMinimum(int);
  IntegerVector GetBreakpointVector();
  IntegerVector Fit();
};

#endif
#ifndef BREAK_POINT_DETECTOR_H
#define BREAK_POINT_DETECTOR_H

#include <Rcpp.h>
#include <vector>
#include "objective_calculator.h"
#include "dynamic_container.h"
using namespace Rcpp;

class BreakpointDetector{
  std::vector<DynamicContainer*> lookup_table_;
  NumericMatrix values_;
public:
  BreakpointDetector(NumericMatrix , double);
  ~BreakpointDetector();
  void FindMinimum(int);
  IntegerVector Fit();
};

#endif


// class BreakpointDetector{
//   std::vector<ObjectiveCalculator*> lookup_table_;
//   std::vector<IntegerVector> breakpoint_positions_;
//   NumericMatrix values_;
// public:
//   BreakpointDetector(NumericMatrix , double);
//   ~BreakpointDetector();
//   void FindMinimum(int);
//   IntegerVector Fit();
// };
//   
// #endif
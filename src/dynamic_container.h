#ifndef DYNAMIC_CONTAINER_H
#define DYNAMIC_CONTAINER_H

#include <Rcpp.h>
#include "objective_calculator.h"
using namespace Rcpp;

class DynamicContainer{
  double objective_save;
  ObjectiveCalculator* objective_main_;
  ObjectiveCalculator* objective_hold_;
  IntegerVector breakpoints_;
  IntegerVector held_positions_;
  bool isBuffered;
public:
  DynamicContainer(int, double);
  DynamicContainer(const DynamicContainer&);
  ~DynamicContainer();
  void Update(NumericMatrix::Row);
  void HoldOut(NumericMatrix::Row);
  void SaveObjective(double);
  double GetObjective();
  IntegerVector GetBreakpoints();
  IntegerVector GetHold();
};


#endif
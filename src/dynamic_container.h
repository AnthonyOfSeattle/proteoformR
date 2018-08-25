#ifndef DYNAMIC_CONTAINER_H
#define DYNAMIC_CONTAINER_H

#include <Rcpp.h>
#include <string>
#include "objective_calculator.h"
using namespace Rcpp;

class DynamicContainer{
  int true_position_;
  ObjectiveCalculator* objective_main_;
  DynamicContainer* last_break_;
  double penalty_;
public:
  DynamicContainer(int, double, double, std::string);
  DynamicContainer(DynamicContainer&);

  void SetPos(int);
  int GetPos();
  
  void Update(NumericMatrix::Row);
  double GetObjective();
  void Reset();
  
  DynamicContainer* GetLastBreak();
};


#endif
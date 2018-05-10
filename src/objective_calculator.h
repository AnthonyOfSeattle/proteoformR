#ifndef OBJECTIVE_CALCULATOR_H
#define OBJECTIVE_CALCULATOR_H

#include <Rcpp.h>
#include "loss_calculator.h"
using namespace Rcpp;

class ObjectiveCalculator{
  double objective_;
  int buffer_counter_;
  LossCalculator* loss_;
  //PenaltyCalculator* penalty_;
public:
  ObjectiveCalculator(int, int);
  void SetObjective(double);
  void Update(NumericMatrix::Row);
  double GetObjective();
};

#endif

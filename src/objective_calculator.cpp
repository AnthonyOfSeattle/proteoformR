#include <Rcpp.h>
#include "objective_calculator.h"
using namespace Rcpp;


ObjectiveCalculator::ObjectiveCalculator(int nsamples,
                                         double penalty){
  objective_ = 0.;
  loss_ = new LossCalculator(nsamples);
  penalty_ = penalty;
}

ObjectiveCalculator::~ObjectiveCalculator(){
  delete loss_;
}

void ObjectiveCalculator::SetObjective(double value){
  objective_ = value;
}

void ObjectiveCalculator::Update(NumericMatrix::Row value){
  loss_ -> Update(value);
}

double ObjectiveCalculator::GetObjective(){
  double cur_loss = loss_ -> GetLoss();
  return objective_ + cur_loss + penalty_;
}
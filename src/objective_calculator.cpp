#include <Rcpp.h>
#include "objective_calculator.h"
using namespace Rcpp;


ObjectiveCalculator::ObjectiveCalculator(int nsamples,
                                         int buffer_size){
  objective_ = 0.;
  loss_ = new LossCalculator(nsamples);
  buffer_counter_ = buffer_size;
}

void ObjectiveCalculator::SetObjective(double value){
  objective_ = value;
}

void ObjectiveCalculator::Update(NumericMatrix::Row value){
  loss_ -> Update(value);
}

double ObjectiveCalculator::GetObjective(){
  double cur_loss = loss_ -> GetLoss();
  return objective_ + cur_loss;
}
#include <Rcpp.h>
#include "dynamic_container.h"
using namespace Rcpp;

DynamicContainer::DynamicContainer(int nsamples,
                                   double penalty,
                                   double objective = 0.){
  objective_main_ = new ObjectiveCalculator(nsamples, penalty);
  objective_main_ -> SetObjective(objective);
  last_break_ = this;
  penalty_ = penalty;
  is_held_ = false;
}

DynamicContainer::DynamicContainer(DynamicContainer& prev){
  objective_main_ = new ObjectiveCalculator(*prev.objective_main_);
  last_break_ = &prev;
  penalty_ = prev.penalty_;
  is_held_ = false;
}

void DynamicContainer::SetPos(int pos){
  true_position_ = pos;
}

int DynamicContainer::GetPos(){
  return true_position_;
}

void DynamicContainer::Update(NumericMatrix::Row values){
  objective_main_ -> Update(values);
}

double DynamicContainer::GetObjective(){
  double objective = objective_main_ -> GetObjective();
  if(is_held_){
    objective += penalty_;
    }
  return objective;
}

void DynamicContainer::Reset(){
  objective_main_ -> SetObjective( GetObjective() );
  objective_main_ -> ZeroAccumulators();
}

DynamicContainer* DynamicContainer::GetLastBreak(){
  return last_break_;
}

void DynamicContainer::Hold(){
  is_held_ = true;
}

bool DynamicContainer::isHeld(){
  return is_held_;
}
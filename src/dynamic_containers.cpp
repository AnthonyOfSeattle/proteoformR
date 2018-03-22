#include <Rcpp.h>
#include "utils.h"
#include "dynamic_containers.h"
using namespace Rcpp;

DynamicContainer::DynamicContainer(int nsamp){
  total_n_ = 0;
  path_loss_ = 0.;
  per_sample_n_ = rep(0., nsamp);
  saved_error_ = rep(0., nsamp);
  moment_one_ =  rep(0., nsamp);
  moment_two_ =  rep(0., nsamp);
  break_point_positions_ = IntegerVector(0);
  isTerminal = 0;
  last_global_index_ = 0;
  nsamp_ = nsamp;
}

void DynamicContainer::reset(){
  total_n_ = 0;
  path_loss_ = 0.;
  per_sample_n_ = rep(0., nsamp_);
  saved_error_ = rep(0., nsamp_);
  moment_one_ =  rep(0., nsamp_);
  moment_two_ =  rep(0., nsamp_);
  break_point_positions_ = IntegerVector(0);
  isTerminal = 0;
  last_global_index_ = 0;
}

void DynamicContainer::UpdateStats(NumericMatrix::Row values){
  // Add values to sufficient statistics
  // In many cases, we may encounter NA values,
  // these are ignored in the following sugar expressions
  per_sample_n_ = ifelse(!(is_na(values)), per_sample_n_ + 1., per_sample_n_);
  moment_one_ = ifelse(!(is_na(values)), moment_one_ + values, moment_one_);
  moment_two_ = ifelse(!(is_na(values)), moment_two_ +  pow(values , 2), moment_two_); 
  total_n_ += 1;
}

double DynamicContainer::CalculateError(){
  // Calculate error, add it to the current path loss, and output
  // Only calculate error if you have seen values for that sample
  NumericVector current_error = ifelse(!(per_sample_n_ == 0),
                                       moment_two_ - pow(moment_one_, 2)/per_sample_n_,
                                       0.);
  return path_loss_ + sum(saved_error_ + current_error);
}

bool DynamicContainer::InducesSinglet(int index){
  if (last_global_index_ > 0){
    return last_global_index_ == index - 1;}
  else{
    return 0;
  }
}

void DynamicContainer::SaveAndReset(){
  // Calculate error and save it
  // Only calculate error if you have seen values for that sample 
  NumericVector current_error = ifelse(!(per_sample_n_ == 0),
                                       moment_two_ - pow(moment_one_, 2)/per_sample_n_,
                                       0.);
  saved_error_ = saved_error_ + current_error;
  // Reset
  per_sample_n_ = rep(0., per_sample_n_.length());
  moment_one_ = rep(0., moment_one_.length());
  moment_two_ = rep(0., moment_two_.length());
}

void DynamicContainer::SetLoss(double new_loss){
  path_loss_ = new_loss;
}

void DynamicContainer::SetBreakPoints(IntegerVector positions){
  break_point_positions_ = positions;
};

void DynamicContainer::UpdateBreakPoints(int global_index){
  break_point_positions_.push_back(global_index);
  last_global_index_ = global_index;
};

IntegerVector DynamicContainer::GetBreakPointPositions(){
  return break_point_positions_;
};

NumericVector DynamicContainer::GetModel(){
  double total_n = sum(per_sample_n_);
  double absolute_mean = sum(moment_one_/total_n);
  return rep(absolute_mean, total_n_);
}

//double DynamicContainer::CalculateError(){
//  NumericVector stable_moment_2 = log(moment_two_);
//  NumericVector stable_moment_1 = 2.*log(abs(moment_one_)) - log(cur_n_);
//  NumericVector current_error = log_minus(stable_moment_2, stable_moment_1);
//  NumericVector total_error = log_sum(saved_error_, current_error);
//  return log_sum(total_error);
//}

//void DynamicContainer::SaveAndReset(){
// Calculate Error
//  NumericVector stable_moment_2 = log(moment_two_);
//  NumericVector stable_moment_1 = 2.*log(abs(moment_one_)) - log(cur_n_);
//  NumericVector current_error = log_minus(stable_moment_2, stable_moment_1);
//  saved_error_ = log_sum(saved_error_, current_error);
// Reset
//  cur_n_ = 0;
//  moment_one_ = rep(0., moment_one_.length());
//  moment_two_ = rep(0., moment_two_.length());
//}

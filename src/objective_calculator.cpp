#include <Rcpp.h>
#include <string>
#include "objective_calculator.h"
using namespace Rcpp;

/* Initialize each vector in the objective calculator
 * to hold each batch sepperate, which will allow
 * independent calculation of the batch means
 */
ObjectiveCalculator::ObjectiveCalculator(int nsamples,
                                         double penalty,
                                         std::string model_type){
  objective_ = 0.;
  nsamp = nsamples;
  penalty_ = penalty;
  if (model_type == "reference"){pool_mean_ = true;} 
  else{pool_mean_ = false;}
  ZeroAccumulators();
}

ObjectiveCalculator::ObjectiveCalculator(const ObjectiveCalculator& prev){
  nsamp = prev.nsamp;
  per_sample_n_ = clone(prev.per_sample_n_);
  moment_one_ = clone(prev.moment_one_);
  moment_two_ = clone(prev.moment_two_);
  objective_ = prev.objective_;
  penalty_ = prev.penalty_;
  pool_mean_ = prev.pool_mean_;
}


void ObjectiveCalculator::ZeroAccumulators(){
  per_sample_n_ = rep(0, nsamp);
  moment_one_ =  rep(0., nsamp);
  moment_two_ = rep(0., nsamp);
}

void ObjectiveCalculator::SetObjective(double value){
  objective_ = value;
}

/* Calculating the minimum variance unbiased estimator
 * can be done entirely using E[X] and E[X**2].
 * This means that we can keep track of additive
 * sufficient statistics and only evaluate the loss when
 * it is needed.
 */
void ObjectiveCalculator::Update(NumericMatrix::Row values){
  for (int i = 0; i < nsamp; i ++){
    if (!NumericMatrix::is_na(values[i])){
      per_sample_n_[i] += 1;
      moment_one_[i] += values[i];
      moment_two_[i] += pow(values[i], 2);
    }
  }
}

/* Transform saved additive sufficient statistics into a squared loss.
 * Ignore batch slots that are absent of updates.
 * Then, add in saved path objectives and a penalty.
 */
double ObjectiveCalculator::GetObjective(){
  double loss_scalar = 0.;
  if (pool_mean_){
    int total_n = sum(per_sample_n_);
    if (total_n > 0){
      loss_scalar = sum(moment_two_) - pow(sum(moment_one_), 2)/total_n;
    }
    return objective_ + loss_scalar + penalty_;
  }
  
  NumericVector loss_vector(nsamp);
  for (int i = 0; i < nsamp; i++){
    if (per_sample_n_[i] != 0){
      loss_vector[i] = moment_two_[i] - pow(moment_one_[i], 2)/per_sample_n_[i];
    }
  }
  return objective_ + sum(loss_vector) + penalty_;
}


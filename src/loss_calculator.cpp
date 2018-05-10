#include <Rcpp.h>
#include "loss_calculator.h"
using namespace Rcpp;

/* Initialize each vector in the loss calculator
 * to hold each batch sepperate, which will allow
 * independent calculation of the batch means
 */
LossCalculator::LossCalculator(int nsamples){
  NumericVector per_sample_n_ = rep(0., nsamples);
  NumericVector moment_one_ = rep(0., nsamples);
  NumericVector moment_two_ = rep(0., nsamples);
}


/* Calculating the minimum variance unbiased estimator
 * can be done entirely using E[X] and E[X**2].
 * This means that we can keep track of additive
 * sufficient statistics and only evaluate the loss when
 * it is needed.
 */
void LossCalculator::Update(NumericMatrix::Row values){
  per_sample_n_ = ifelse(!(is_na(values)), per_sample_n_ + 1., per_sample_n_);
  moment_one_ = ifelse(!(is_na(values)), moment_one_ + values, moment_one_);
  moment_two_ = ifelse(!(is_na(values)), moment_two_ +  pow(values , 2), moment_two_); 
}


/* Transform saved additive sufficient statistics into a squared loss.
 * Ignore batch slots that are absent of updates.
 */
double LossCalculator::GetLoss(){
  NumericVector current_error = ifelse(!(per_sample_n_ == 0),
                                       moment_two_ - pow(moment_one_, 2)/per_sample_n_,
                                       0.);
  return sum(current_error);
}
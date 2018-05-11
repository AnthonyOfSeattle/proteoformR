#ifndef LOSS_CALCULATOR_H
#define LOSS_CALCULATOR_H

#include <Rcpp.h>
using namespace Rcpp;

/* Container to store and calculate loss over
 * a section of sequntial data data points.
 * Utilizes lazy evaluation of the loss by
 * storing additive sufficient statistics with
 * each update. Transforms statisitics into the 
 * squared loss against the mean with a call to GetLoss
 * 
 * Example:
 *     LossCalculator c = LossCalulator(3);
 *     c.Update(NumericVector::create(1.0, 2.0, 3.0));
 *     c.Update(NumericVector::create(2.0, 3.0, 4.0));
 *     double error = c.GetLoss;
 */
class LossCalculator{
  int nsamp;
  IntegerVector per_sample_n_;
  NumericVector moment_one_;
  NumericVector moment_two_;
public:
  LossCalculator(int);
  void Update(NumericMatrix::Row);
  double GetLoss();
};

#endif
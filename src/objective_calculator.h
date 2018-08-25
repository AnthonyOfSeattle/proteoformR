#ifndef OBJECTIVE_CALCULATOR_H
#define OBJECTIVE_CALCULATOR_H

#include <Rcpp.h>
#include <string>
using namespace Rcpp;

/* Container to store and calculate objective over
 * a section of sequential data points.
 * Utilizes lazy evaluation of the loss by
 * storing additive sufficient statistics with
 * each update. Transforms statisitics into the penalized
 * squared loss against the mean with a call to GetObjective.
 */
class ObjectiveCalculator{
  int nsamp;
  IntegerVector per_sample_n_;
  NumericVector moment_one_;
  NumericVector moment_two_;
  double objective_;
  double penalty_;
  bool pool_mean_;
public:
  ObjectiveCalculator(int, double, std::string);
  ObjectiveCalculator(const ObjectiveCalculator&);
  void ZeroAccumulators();
  void SetObjective(double);
  void Update(NumericMatrix::Row);
  double GetObjective();
};

#endif

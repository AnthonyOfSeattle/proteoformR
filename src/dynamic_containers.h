#ifndef DYNAMIC_CONTAINERS_H
#define DYNAMIC_CONTAINERS_H

#include <Rcpp.h>
using namespace Rcpp;

class DynamicContainer {
  int total_n_;
  NumericVector per_sample_n_;
  double path_loss_;
  NumericVector saved_error_;
  NumericVector moment_one_;
  NumericVector moment_two_;
  IntegerVector break_point_positions_;
  IntegerVector global_bp_indices_;
  int last_global_index_;
  int nsamp_;
public:
  bool isTerminal;
  
  DynamicContainer(int);
  void reset();
  void UpdateStats(NumericMatrix::Row);
  double CalculateError();
  bool InducesSinglet(int);
  void SaveAndReset();
  void SetLoss(double);
  void SetBreakPoints(IntegerVector);
  void UpdateBreakPoints(int);
  IntegerVector GetBreakPointPositions();
  NumericVector GetModel();
};

#endif
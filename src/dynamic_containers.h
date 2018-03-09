#ifndef DYNAMIC_CONTAINERS_H
#define DYNAMIC_CONTAINERS_H

#include <Rcpp.h>
using namespace Rcpp;

class DynamicContainer {
  //int cur_n_;
  NumericVector per_sample_n_;
  double path_loss_;
  NumericVector saved_error_;
  NumericVector moment_one_;
  NumericVector moment_two_;
  StringVector break_point_references_;
  IntegerVector break_point_positions_;
  IntegerVector global_bp_indices_;
  int last_global_index_;
public:
  bool isTerminal;
  DynamicContainer(int);
  void UpdateStats(NumericMatrix::Row);
  double CalculateError();
  bool InducesSinglet(int);
  void SaveAndReset();
  void SetLoss(double);
  void SetBreakPoints(StringVector, IntegerVector);
  void UpdateBreakPoints(std::string, int, int);
  StringVector GetBreakPointReferences();
  IntegerVector GetBreakPointPositions();
};

#endif
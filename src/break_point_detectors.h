#ifndef BREAK_POINT_DETECTORS_H
#define BREAK_POINT_DETECTORS_H

#include <Rcpp.h>
#include <vector>
#include "dynamic_containers.h"
using namespace Rcpp;

template <class T>
class BPDetector{
  std::vector<T> saved_values_;
  std::string old_ref_;
  StringVector references_;
  IntegerVector positions_;
  NumericMatrix values_;
  double lambda_;
public:
  BPDetector(StringVector, NumericMatrix, double);
  bool isNewReference(int);
  void UpdateReference(int);
  void SaveAndReset(int);
  void FindMinimum(int);
  IntegerVector Fit();
};

typedef BPDetector<DynamicContainer> PiecewiseConstantDetector;

IntegerVector FitBreakPoints(StringVector, NumericMatrix, double, StringVector);
NumericVector BuildModel(StringVector, NumericMatrix, IntegerVector, StringVector);

#endif

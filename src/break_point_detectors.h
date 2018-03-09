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
  BPDetector(StringVector, IntegerVector, NumericMatrix, double);
  bool isNewReference(int);
  void UpdateReference(int);
  void SaveAndReset(int);
  void FindMinimum(int);
  DataFrame Fit();
};

typedef BPDetector<DynamicContainer> PiecewiseConstantDetector;

DataFrame FitBreakPoints(StringVector, IntegerVector, NumericMatrix, double, StringVector);

#endif

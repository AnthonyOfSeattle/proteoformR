#ifndef R_INTERFACE_H
#define R_INTERFACE_H

#include <Rcpp.h>
#include "break_point_detector.h"
using namespace Rcpp;

IntegerVector DetectBreakpoints(NumericMatrix, double);
NumericVector FitModel(NumericMatrix, IntegerVector);

#endif

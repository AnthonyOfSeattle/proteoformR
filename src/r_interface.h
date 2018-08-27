#ifndef R_INTERFACE_H
#define R_INTERFACE_H

#include <Rcpp.h>
#include <string>
#include "break_point_detector.h"
using namespace Rcpp;

IntegerVector DetectBreakpoints(NumericMatrix, double, std::string);
NumericVector FitModel(NumericMatrix, IntegerVector);

#endif

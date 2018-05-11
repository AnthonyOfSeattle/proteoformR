#ifndef R_INTERFACE_H
#define R_INTERFACE_H

#include <Rcpp>
#include "break_point_detector.h"

IntegerVector DetectBreakpoints(NumericMatrix, double);

#endif

#include <Rcpp.h>
#include "r_interface.h"
using namespace Rcpp;

IntegerVector DetectBreakpoints(NumericMatrix values,
                                double lambda){
  IntegerVector model;
  BreakpointDetector detector(values,
                              lambda);
  model = detector.Fit();
  return model;
}


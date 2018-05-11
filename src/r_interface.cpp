#include <Rcpp.h>
#include "r_interface.h"
using namespace Rcpp;

// [[Rcpp::export]]

IntegerVector DetectBreakpoints(NumericMatrix values,
                                double lambda){
  IntegerVector model;
  BreakpointDetector detector(values,
                              lambda);
  model = detector.Fit();
  return model;
}


/*** R
# No breakpoints
  continuous = matrix(rnorm(100,0, 1), ncol = 1)
  print(DetectBreakpoints(continuous, lambda = 7))

# One breakpoint
continuous = cbind(matrix(c(rnorm(100,-.5, .5), rnorm(100,0.5, .5)), ncol = 1))
print(DetectBreakpoints(continuous, lambda = 7))

# Two breakpoint
continuous = cbind(matrix(c(rnorm(100,-.5, .5), 
                            rnorm(100,0.5, .5), 
                            rnorm(100,-0.5, .5)), ncol = 1))
print(DetectBreakpoints(continuous, lambda = 7))

*/
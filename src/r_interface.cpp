#include <Rcpp.h>
#include <string>
#include "r_interface.h"
using namespace Rcpp;

// [[Rcpp::export]]

IntegerVector DetectBreakpoints(NumericMatrix values,
                                double lambda, 
                                std::string model_type = "reference"){
  IntegerVector model;
  BreakpointDetector detector(values,
                              lambda,
                              model_type);
  model = detector.Fit();
  return model;
}

double GlobalMean(NumericMatrix values){
  IntegerVector per_sample_n = rep(0, values.cols());
  NumericVector moment_one = rep(0., values.cols());
  for (int row = 0; row < values.rows(); row++){
    for (int col = 0; col < values.cols(); col++){
      if (!NumericMatrix::is_na(values(row, col))){
        per_sample_n[col] += 1;
        moment_one[col] += values(row, col);
      }
    }
  }
  
  double global_mean = 0.;
  for (int col = 0; col < values.cols(); col++){
    if (per_sample_n[col] != 0){
      global_mean += moment_one[col]/per_sample_n[col];
    }
  }
  global_mean = global_mean/values.cols();
  return(global_mean);
}

// [[Rcpp::export]]
NumericVector FitModel(NumericMatrix values,
                       IntegerVector breakpoints){
  NumericVector model = rep(0.,values.rows());
  int last_break = 0;
  for (int i = 0; i < breakpoints.size(); i++){
    double mean = GlobalMean(values(Range(last_break, breakpoints[i] - 1), 
                                    _));
    for (int row = last_break; row < breakpoints[i]; row++){
      model[row] = mean;
    }
    last_break = breakpoints[i];
  }
  double mean = GlobalMean(values(Range(last_break, values.rows() - 1 ),
                                  _));
  for (int row = last_break; row < values.rows(); row++){
    model[row] = mean;
  }
  return model;
}




/*** R
# # No breakpoints
continuous = matrix(rnorm(10,0, 1), ncol = 1)
breaks <- DetectBreakpoints(continuous, lambda = 5)
print(breaks)
FitModel(continuous, breaks)

# # One breakpoint
continuous = matrix(c(rnorm(10, -1, .5), rnorm(10, 1, .5)), ncol = 1)
breaks <- DetectBreakpoints(continuous, lambda = 3)
print(breaks)
FitModel(continuous, breaks)

# # Two breakpoint
continuous = cbind(matrix(c(rnorm(10,-1, .1),
                            rnorm(10, 1, .1),
                            rnorm(10, -1, .1)), ncol = 1))
breaks <- DetectBreakpoints(continuous, lambda = 3)
print(breaks)
FitModel(continuous, breaks)

#Outlier
continuous = cbind(rnorm(10,0, .5), rnorm(10,0, .5))
continuous[1, ] = continuous[1,] + 3
breaks <- DetectBreakpoints(continuous, lambda = 5)
print(breaks)
FitModel(continuous, breaks)
*/
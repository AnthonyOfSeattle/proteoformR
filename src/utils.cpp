#include <Rcpp.h>
#include "utils.h"
using namespace Rcpp;

double na_safe_max(double x, double y){
  if ( !R_IsNA(x) & !R_IsNA(y)){
    return std::max(x,y);
  } else if (!R_IsNA(x)){
    return(x);
  } else if (!R_IsNA(y)){
    return(y);
  } else {
    return NA_REAL;
  }
}

double log_sum(double x, double y){
  if ( !R_IsNA(x) & !R_IsNA(y) ){
    double max_val = std::max(x,y);
    double val = log(exp(x - max_val) + exp(y - max_val));
    return (max_val + val);
  } else if (!R_IsNA(x)){
    return(x);
  } else if (!R_IsNA(y)){
    return(y);
  } else {
    return NA_REAL;
  }
  
}

NumericVector log_sum(NumericVector x, NumericVector y){
  // Safe if x.length() < y.length()
  NumericVector values = rep(NA_REAL, x.length());
  LogicalVector infinite_pos = is_infinite(x) | is_infinite(y);
  for ( int i = 0; i < x.length(); i++){
    if (infinite_pos[i]){
      values[i] = na_safe_max(x[i], y[i]);
    } else{
      values[i] = log_sum(x[i], y[i]);
    }
  }
  return values;
}

double log_sum(NumericVector x){
  double val = NA_REAL;
  LogicalVector infinite_pos = is_infinite(x);
  for ( int i = 0; i < x.length(); i++){
    if (infinite_pos[i]){
      val = na_safe_max(val, x[i]);
    } else{
      val = log_sum(val, x[i]);
    }
  }
  return val;
}

double log_minus(double x, double y){
  if (!R_IsNA(x) & !R_IsNA(y) ){
    return (x + log(1-exp(y-x)));
  } else if (!R_IsNA(x)){
    return(x);
  } else {
    return NA_REAL;
  }
}

NumericVector log_minus(NumericVector x, NumericVector y){
  // Safe if x.length() < y.length()
  NumericVector values = rep(0., x.length());
  LogicalVector infinite_pos = is_infinite(x) | is_infinite(y);
  for ( int i = 0; i < x.length(); i++){
    if (infinite_pos[i]){
      values[i] = na_safe_max(x[i], y[i]);
    } else{
      values[i] = log_minus(x[i], y[i]);
    }
  }
  return values;
}

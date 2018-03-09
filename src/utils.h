#ifndef UTILS_H
#define UTILS_H

#include <Rcpp.h>
using namespace Rcpp;

double na_safe_max(double, double);

double log_sum(double, double);
NumericVector log_sum(NumericVector, NumericVector);
double log_sum(NumericVector);

double log_minus(double, double);
NumericVector log_minus(NumericVector, NumericVector);


#endif
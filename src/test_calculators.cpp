#include <Rcpp.h>
#include "loss_calculator.h"
using namespace Rcpp;

int ErrorReport(std::string name,
                LogicalVector passing){
  if (is_true(all(passing))){//all(passing)){
    Rcout << name << " is PASSING" << std::endl;
    return 1;
  }
  else{
    Rcout << name << " is FAILING" << std::endl;
    for (int i = 0; i < passing.length(); i++){
      if (passing[i]){
        Rcout << "    " << "PASSED test " << i << std::endl;
      }
      else {
        Rcout << "    " << "FAILED test " << i << std::endl;
      }
    }
    return 0;
  }
}

// [[Rcpp::export]]

int test_LossCalculator() {
  LogicalVector passing(2);
  try {
    
    // Test 0: Fully specified matrix
    NumericMatrix xx(3,3);
    Rcout << xx << std::endl;
    for (int i  = 0. ; i < 3 ; i++){
      xx(i, _) = rep((double) i, 3);
    }
    Rcout << xx << std::endl;
    
    LossCalculator calc1(3);
    for (int i = 0; i < 3; i++){
      calc1.Update(xx(i,_));
    }
    
    if (std::abs(calc1.GetLoss() - 6) < 1e-5){
      passing[0] = TRUE;
    }
    Rcout << calc1.GetLoss() << std::endl;
    
    
    // Test 1: Missing values
    for (int i = 0; i < 3; i++){
      xx(i,i) = NA_REAL;
    }
    Rcout << xx << std::endl;
    
    LossCalculator calc2(3);
    for (int i = 0; i < 3; i++){
      calc2.Update(xx(i,_));
    }
    
    if (std::abs(calc2.GetLoss() - 3) < 1e-5){
      passing[1] = TRUE;
    }
    Rcout << calc2.GetLoss() << std::endl;
    
    return ErrorReport("LossCalculator", passing);
  }
  
  catch (...){
    return ErrorReport("LossCalculator", passing);
  }
}

/*** R
test_LossCalculator()
*/

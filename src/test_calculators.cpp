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
  LogicalVector passing(4);
  try {
    
    // Test 0: Fully specified matrix
    NumericMatrix xx(3,3);
    for (int i  = 0. ; i < 3 ; i++){
      xx(i, _) = rep((double) i, 3);
    }
    
    LossCalculator calc1(3);
    for (int i = 0; i < 3; i++){
      calc1.Update(xx(i,_));
    }
    
    if (std::abs(calc1.GetLoss() - 6) < 1e-5){
      passing[0] = TRUE;
    }
    
    
    // Test 1: Missing values
    for (int i = 0; i < 3; i++){
      xx(i,i) = NA_REAL;
    }
    
    LossCalculator calc2(3);
    for (int i = 0; i < 3; i++){
      calc2.Update(xx(i,_));
    }
    
    if (std::abs(calc2.GetLoss() - 3) < 1e-5){
      passing[1] = TRUE;
    }
    
    // Test2: Column missing
    for (int i  = 0. ; i < 3 ; i++){
      xx(i, _) = rep((double) i, 3);
      xx(i,2) = NA_REAL;
    }
    
    LossCalculator calc3(3);
    for (int i = 0; i < 3; i++){
      calc3.Update(xx(i,_));
    }
    
    if (std::abs(calc3.GetLoss() - 4) < 1e-5){
      passing[2] = TRUE;
    }
    
    
    // Test2: All missing
    for (int i  = 0. ; i < 3 ; i++){
      xx(i, _) = rep(NA_REAL, 3);
      xx(i,2) = NA_REAL;
    }
    
    LossCalculator calc4(3);
    for (int i = 0; i < 3; i++){
      calc4.Update(xx(i,_));
    }
    
    if (calc4.GetLoss() == 0){
      passing[3] = TRUE;
    }
    
    return ErrorReport("LossCalculator", passing);
  }
  
  catch (...){
    return ErrorReport("LossCalculator", passing);
  }
}

/*** R
test_LossCalculator()
*/

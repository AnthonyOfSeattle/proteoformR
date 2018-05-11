#include <Rcpp.h>
#include "loss_calculator.h"
#include "objective_calculator.h"
using namespace Rcpp;

int ErrorReport(std::string name,
                LogicalVector passing){
  if (is_true(all(passing))){
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

NumericMatrix MakeMatrix(int type){
  
  NumericMatrix xx(3,3);
  for (int i  = 0 ; i < 3 ; i++){
    // Test 0: Fully specified matrix
    xx(i, _) = rep((double) i, 3);
    switch (type){
    // Test 1: Missing values
    case 1: xx(i,i) = NA_REAL;
      break;
      // Test2: Column missing
    case 2: xx(i,2) = NA_REAL;
      break;
      // Test3: All missing
    case 3: xx(i, _) = rep(NA_REAL, 3);
      break;
    }
  }
  return xx;
}

// [[Rcpp::export]]

int test_LossCalculator() {
  NumericMatrix xx;
  LogicalVector passing(4);
  try {
    
    LossCalculator calc0(3);
    xx = MakeMatrix(0);
    for (int i = 0; i < 3; i++){
      calc0.Update(xx(i,_));
    }
    if (std::abs(calc0.GetLoss() - 6) < 1e-5){
      passing[0] = TRUE;
    }
    
    
    LossCalculator calc1(3);
    xx = MakeMatrix(1);
    for (int i = 0; i < 3; i++){
      calc1.Update(xx(i,_));
    }
    if (std::abs(calc1.GetLoss() - 3) < 1e-5){
      passing[1] = TRUE;
    }
    
    LossCalculator calc2(3);
    xx = MakeMatrix(2);
    for (int i = 0; i < 3; i++){
      calc2.Update(xx(i,_));
    }
    if (std::abs(calc2.GetLoss() - 4) < 1e-5){
      passing[2] = TRUE;
    }
    
    LossCalculator calc3(3);
    xx = MakeMatrix(3);
    for (int i = 0; i < 3; i++){
      calc3.Update(xx(i,_));
    }
    if (calc3.GetLoss() == 0){
      passing[3] = TRUE;
    }
    
    return ErrorReport("LossCalculator", passing);
  }
  
  catch (...){
    return ErrorReport("LossCalculator", passing);
  }
}

// [[Rcpp::export]]

int test_ObjectiveCalculator() {
  NumericMatrix xx;
  LogicalVector passing(5);
  try {
    
    ObjectiveCalculator calc0(3, 10.);
    xx = MakeMatrix(0);
    for (int i = 0; i < 3; i++){
      calc0.Update(xx(i,_));
    }
    if (std::abs(calc0.GetObjective() - 16) < 1e-5){
      passing[0] = TRUE;
    }
    
    
    ObjectiveCalculator calc1(3, 10.);
    xx = MakeMatrix(1);
    for (int i = 0; i < 3; i++){
      calc1.Update(xx(i,_));
    }
    if (std::abs(calc1.GetObjective() - 13) < 1e-5){
      passing[1] = TRUE;
    }
    
    ObjectiveCalculator calc2(3, 10);
    xx = MakeMatrix(2);
    for (int i = 0; i < 3; i++){
      calc2.Update(xx(i,_));
    }
    if (std::abs(calc2.GetObjective() - 14) < 1e-5){
      passing[2] = TRUE;
    }
    
    ObjectiveCalculator calc3(3, 10.);
    xx = MakeMatrix(3);
    for (int i = 0; i < 3; i++){
      calc3.Update(xx(i,_));
    }
    if (calc3.GetObjective() == 10){
      passing[3] = TRUE;
    }
    
    ObjectiveCalculator calc4(3, 10.);
    calc4.SetObjective(20.);
    xx = MakeMatrix(0);
    for (int i = 0; i < 3; i++){
      calc4.Update(xx(i,_));
    }
    if (std::abs(calc4.GetObjective() - 36) < 1e-5){
      passing[4] = TRUE;
    }
    
    return ErrorReport("ObjectiveCalculator", passing);
  }
  
  catch (...){
    return ErrorReport("ObjectiveCalculator", passing);
  }
}

/*** R
test_LossCalculator()
test_ObjectiveCalculator()
*/

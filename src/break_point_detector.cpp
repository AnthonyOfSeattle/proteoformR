#include <Rcpp.h>
#include "break_point_detector.h"
#include "dynamic_container.h"
using namespace Rcpp;

BreakpointDetector::BreakpointDetector(NumericMatrix values, double lambda){
  values_ = values;
  penalty_ = lambda;
  // Only the first dynamic container in the list it initialize,
  // Because each new container will be copy constructed. 
  // The initial container must have an objective set to -lambda
  // This is the only "breakpoint" in the lookup table which
  // we do not want to penalize.
  DynamicContainer *temp = new DynamicContainer(values.cols(), lambda, -lambda);
  temp -> SetPos(-1);
  lookup_table_.push_back(temp);
}

void BreakpointDetector::FindMinimum(int row_ind){
  Rcout << "Entering loop " << row_ind << std::endl;
  // Save the state of the previous postion
  // by using in a copy contructor.
  DynamicContainer *checkpoint =
    new DynamicContainer(
        *(lookup_table_.back() -> GetLastBreak())
  );
  checkpoint -> SetPos(row_ind);

  double minimum_objective = R_PosInf;
  double cur_objective;
  int minimum_ind;
  Rcout << "Enter loop" << std::endl;
  for (int it = 0;
       it != lookup_table_.size();
       it++){
    lookup_table_[it] -> Update( values_(row_ind - 1, _ ) );
    cur_objective = lookup_table_[it] -> GetObjective();
    Rcout << lookup_table_[it] -> GetPos() << ": " << cur_objective << std::endl;
    if ( (cur_objective < minimum_objective) ){
      minimum_objective = cur_objective;
      minimum_ind = it;
    }
  }
  Rcout << "Exit loop" << std::endl;
  
  if (minimum_ind == lookup_table_.size() - 1 ){
    Rcout << "Potential outlier" << std::endl;
    checkpoint -> Hold();
    Rcout << checkpoint -> GetObjective() << std::endl;
    lookup_table_.push_back(checkpoint);
    Rcout << "Handled outlier" << std::endl;
  }
  
  DynamicContainer *temp = new DynamicContainer( *(lookup_table_[minimum_ind]) );
  temp -> SetPos(row_ind);
  temp -> Reset();
  Rcout << "Temp stats" << std::endl;
  Rcout << temp -> GetObjective() << std::endl;
  Rcout << temp -> GetLastBreak() -> GetPos() << std::endl;
  Rcout << temp -> isHeld() << std::endl;
  lookup_table_.push_back(temp);
  Rcout << "<<<<<<<<>>>>>>>>>" << std::endl;
}

IntegerVector BreakpointDetector::Fit(){
  for (int row_ind = 1; row_ind <= values_.rows(); row_ind++){
    FindMinimum(row_ind);
  }
  return IntegerVector::create(1);
}

#include <Rcpp.h>
#include "break_point_detector.h"
#include "dynamic_container.h"
using namespace Rcpp;

BreakpointDetector::BreakpointDetector(NumericMatrix values, double lambda){
  values_ = values;
  penalty_ = lambda;
  // Only the first dynamic container in the list is initialized,
  // because each new container will be copy constructed. 
  // The initial container must also have an objective set to -lambda,
  // since this is the only "breakpoint" in the lookup table which
  // we do not want to penalize.
  DynamicContainer *temp = new DynamicContainer(values.cols(), lambda, -lambda);
  temp -> SetPos(0);
  lookup_table_.push_back(temp);
}

void BreakpointDetector::FindMinimum(int row_ind){
  double minimum_objective = R_PosInf;
  int minimum_ind;
  double cur_objective;
  for (int iter = 0;
       iter < lookup_table_.size();
       iter++){
    lookup_table_[iter] -> Update( values_(row_ind - 1, _ ) );
    cur_objective = lookup_table_[iter] -> GetObjective();
    if ( cur_objective < minimum_objective){
      minimum_objective = cur_objective;
      minimum_ind = iter;
    }
  }
  
  DynamicContainer *temp = new DynamicContainer( *(lookup_table_[minimum_ind]) );
  temp -> SetPos(row_ind);
  temp -> Reset();
  lookup_table_.push_back(temp);
}

IntegerVector BreakpointDetector::GetBreakpointVector(){
  IntegerVector breakpoint_vector = IntegerVector::create();
  DynamicContainer* last_break = lookup_table_.back() -> GetLastBreak();
  while (last_break -> GetPos() > 0){
    breakpoint_vector.push_front(last_break -> GetPos());
    last_break = last_break -> GetLastBreak();
  }
  return breakpoint_vector;
}

IntegerVector BreakpointDetector::Fit(){
  for (int row_ind = 1; row_ind <= values_.rows(); row_ind++){
    FindMinimum(row_ind);
  }
  return GetBreakpointVector();
}

#include <Rcpp>
#include "break_point_detector.h"
using namespace Rcpp;

BreakpointDetector::BreakpointDetector(NumericMatrix values, double lambda){
  for (int ind = 0; ind < values_.rows() + 1; ind++){
    lookup_table_.push_back( 
      ObjectiveCalculator( values.cols(), lambda ) 
    );
    breakpoint_positions_.push_back(
      IntegerVector(0)
    );
  }
  
  // Initialize container 0 to -lambda
  lookup_table_[0].SetObjective(-lambda);
}

void BreakpointDetector::FindMinimum(int row_ind){
  double minimum_objective = R_PosInf;
  double cur_objective;
  int minimum_ind;
  for (int back_track = 0; back_track < row_ind; back_track++){
    lookup_table_[back_track].UpdateStats( values_(row_ind - 1, _ ) );
    cur_objective = lookup_table_[back_track].GetObjective;
    if ( (cur_objective < minimum_objective) ){ 
      minimum_objective = cur_objective;
      minimum_ind = back_track;
    }
  }
  lookup_table_[row_ind].SetObjective(minimum_objective);
  breakpoint_positions_[row_ind] = breakpoint_positions_[minimum_ind];
  
  if (minimum_ind != 0){ 
    breakpoint_positions_[row_ind].push_back(minimum_ind);
  }
}

IntegerVector BreakpointDetector::Fit(){
  for (int row_ind = 1; row_ind <= values_.rows(); row_ind++){ 
    FindMinimum(row_ind);
  }
  return breakpoint_positions_[values_.rows()];
}
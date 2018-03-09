#include <Rcpp.h>
#include <vector>
#include "break_point_detectors.h"
using namespace Rcpp;

template <class T>
BPDetector<T>::BPDetector(StringVector references, 
                          IntegerVector positions,
                          NumericMatrix values, 
                          double lambda){
  // Save values
  references_ = references;
  positions_ = positions;
  values_ = values;
  lambda_ = lambda;
  
  // Initialize Containers
  for (int ind = 0; ind < values_.rows() + 1; ind++){
    saved_values_.push_back( T(values.cols()) );
  }
  
  // Initialize container 0 to -lambda
  saved_values_[0].SetLoss(-lambda);
  
  // Initialize cur_ref_ as initial reference
  old_ref_ = Rcpp::as< std::string >(references_(0));
}

template <class T>
bool BPDetector<T>::isNewReference(int row_ind){
  std::string cur_ref = Rcpp::as< std::string >(references_(row_ind - 1));
  return cur_ref != old_ref_;
}

template <class T>
void BPDetector<T>::UpdateReference(int row_ind){
  old_ref_ = Rcpp::as< std::string >(references_(row_ind - 1));
}

template <class T>
void BPDetector<T>::SaveAndReset(int row_ind){
  for (int back_track = 0; back_track < row_ind; back_track++){
    saved_values_[back_track].SaveAndReset();
    //Rcout << std::endl;
  }
}

template <class T>
void BPDetector<T>::FindMinimum(int row_ind){
  double minimum_loss = R_PosInf;
  double cur_loss = 0.;
  int minimum_ind = -1;
  for (int back_track = 0; back_track < row_ind; back_track++){
    if (saved_values_[back_track].isTerminal){ continue; }
    saved_values_[back_track].UpdateStats( values_(row_ind - 1, _ ) );
    cur_loss = saved_values_[back_track].CalculateError() + lambda_;
    //Rcout << cur_loss << " ";
    if ( (cur_loss < minimum_loss) &
         !(saved_values_[back_track].InducesSinglet(back_track))
         ){ 
      minimum_loss = cur_loss;
      minimum_ind = back_track;
    }
  }
  // Save the minimum path
  //Rcout << std::endl << minimum_loss << " " << minimum_ind << std::endl;
  saved_values_[row_ind].SetLoss(minimum_loss);
  saved_values_[row_ind].SetBreakPoints(saved_values_[minimum_ind].GetBreakPointReferences(),
                                        saved_values_[minimum_ind].GetBreakPointPositions());
  
  if (minimum_ind != 0){ 
    saved_values_[row_ind].UpdateBreakPoints(Rcpp::as< std::string >(references_[minimum_ind]),
                                             positions_[minimum_ind], minimum_ind);
  }
}

template <class T>
DataFrame BPDetector<T>::Fit(){
  for (int row_ind = 1; row_ind <= values_.rows(); row_ind++){ 
    // Reset all scores if we have moved to a new gene
    if (isNewReference(row_ind)){
      //Rcout << "Reseting" << std::endl;
      SaveAndReset(row_ind);
      UpdateReference(row_ind);
      saved_values_[row_ind - 1].isTerminal = 1;
    }
    //Rcout << row_ind << ":" << std::endl;
    // Look through all former nodes and find minimum
    FindMinimum(row_ind);
    //break;
  }
  return DataFrame::create(Named("BP_References") = saved_values_[values_.rows()].GetBreakPointReferences(),
                           Named("BP_Positions") = saved_values_[values_.rows()].GetBreakPointPositions());
}

// [[Rcpp::export]]

DataFrame FitBreakPoints(StringVector references, 
                         IntegerVector positions,
                         NumericMatrix values,
                         double lambda,
                         StringVector method = "constant"){
  std::string method_arg = Rcpp::as< std::string >(method[0]);
  DataFrame model;
  PiecewiseConstantDetector detector(references, 
                                     positions,
                                     values,
                                     lambda);
  
  model = detector.Fit();
  
  return model;
}

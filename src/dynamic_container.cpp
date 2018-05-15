#include <Rcpp.h>
#include "dynamic_container.h"
using namespace Rcpp;

DynamicContainer::DynamicContainer(int nsamples,
                                   double penalty){
  objective_main_ = new ObjectiveCalculator(nsamples, penalty);
}

#include "FitHistogram.h"

FitHistogram::FitHistogram(){

  doDebug = false;

}

FitHistogram::~FitHistogram(){

}

void FitHistogram::InitFitVar(){

}

void FitHistogram::Fit(){

  const int NPar = parRangeMins.size();

  fitFunc = new TF1(Name, functionalForm, fitVarRangeMin, fitVarRangeMax);

  for(int i=0; i<NPar; i++){
    if(doDebug){
    }

    double this_min = parRangeMins.at(i);
    double this_max = parRangeMaxs.at(i);
    double this_init = parInitSet.at(i) ? parInit.at(i) : (this_min+this_max)/2.;

    fitFunc->SetParLimits(i, this_min, this_max);
    fitFunc->SetParameter(i, this_init);

  }
  if(doDebug){

  }

  //fitResultPtr = hist_original->Fit(Name,"0QSWLIM","",fitRangeMin,fitRangeMax);

  fitResultPtr = hist_original->Fit(Name,"0QSWLI","",fitRangeMin,fitRangeMax);

  //(TVirtualFitter::GetFitter())->GetConfidenceIntervals(confBand);

}



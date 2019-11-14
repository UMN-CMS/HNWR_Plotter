#include "FitHistogram.h"

FitHistogram::FitHistogram(){

  doDebug = false;

}

FitHistogram::~FitHistogram(){

}

void FitHistogram::InitFitVar(){

  fitVar = new RooRealVar(fitVarName, fitVarName, fitVarRangeMin, fitVarRangeMax);
  TString newname = "rooDataHist_"+TString(hist_original->GetName());
  dataH = new RooDataHist(newname, newname, *fitVar, hist_original); 

}

void FitHistogram::Fit(){

  const int NPar = parNames.size();
  RooRealVar* rrvs[NPar];

  RooArgSet argset(*fitVar);
  for(int i=0; i<NPar; i++){
    if(doDebug){
      cout << "[FitHistogram::Fit()] i = " << i << endl;
      cout << "[FitHistogram::Fit()]   parNames.at(i) = " << parNames.at(i) << endl;
    }
    rrvs[i] = new RooRealVar(parNames.at(i), parNames.at(i), parRangeMins.at(i), parRangeMaxs.at(i));;
    argset.add(*(rrvs[i]));
    if(doDebug){
      cout << "[FitHistogram::Fit()]   argset : "; argset.Print();
    }
  }
  if(doDebug){
    cout << "[FitHistogram::Fit()] argset : "; argset.Print();
  }
  gPdf = new RooGenericPdf(Name, functionalForm, argset);

  fitResult = gPdf->fitTo(*dataH, Range(fitRangeMin, fitRangeMax),Save());

}



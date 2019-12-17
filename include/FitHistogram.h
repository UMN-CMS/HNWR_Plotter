#ifndef FitHistogram_h
#define FitHistogram_h

using namespace RooFit;

class FitHistogram{

public:

  FitHistogram();
  ~FitHistogram();

  bool doDebug;

  TString Name;

  inline void SetHist(TH1D *hist){
    hist_original = (TH1D *)hist->Clone();
  }
  inline TH1D *getHist() const { return hist_original; }
  inline RooDataHist* getDataHist() const { return dataH; }

  inline void SetFitVar(TString name, double min, double max){
    fitVarName = name;
    fitVarRangeMin = min;
    fitVarRangeMax = max;
  }
  TString getFitVarName() const { return fitVarName; }
  inline double getFitVarRangeMin() const { return fitVarRangeMin; }
  inline double getFitVarRangeMax() const { return fitVarRangeMax; }
  void InitFitVar();
  inline RooRealVar* getFitVar() const { return fitVar; }

  //==== fit function

  TString functionalForm;
  double fitRangeMin, fitRangeMax;

  inline void InitParameters(int n){
    for(int i=0; i<n; i++){
      parNames.push_back("");
      parRangeMins.push_back(0);
      parRangeMaxs.push_back(1);
      parInit.push_back(0.5);
      parInitSet.push_back(false);
    }
  }
  inline void ClearParameters(){
    parNames.clear();
    parRangeMins.clear();
    parRangeMaxs.clear();
    parInit.clear();
    parInitSet.clear();
  }
  void SetParameter(int i, double val, double min, double max){
    parInitSet.at(i) = true;
    parInit.at(i) = val;
    parRangeMins.at(i) = min;
    parRangeMaxs.at(i) = max;
  }
  vector<TString> parNames;
  vector<double> parRangeMins, parRangeMaxs, parInit;
  vector<bool> parInitSet;

  void Fit();

  RooGenericPdf* getPdf() const { return gPdf; }
  RooFitResult *getFitResult() const { return fitResult; }

private:

  TH1D *hist_original;

  TString fitVarName;
  double fitVarRangeMin, fitVarRangeMax;
  RooRealVar *fitVar;
  RooDataHist *dataH;

  RooGenericPdf* gPdf;
  RooFitResult* fitResult;

};

#endif

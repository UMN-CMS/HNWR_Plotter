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

  inline void SetFitVar(double min, double max){
    fitVarRangeMin = min;
    fitVarRangeMax = max;
  }
  inline double getFitVarRangeMin() const { return fitVarRangeMin; }
  inline double getFitVarRangeMax() const { return fitVarRangeMax; }
  void InitFitVar();

  //==== fit function

  TF1 *fitFunc;
  TH1D *confBand;
  TString functionalForm;
  double fitRangeMin, fitRangeMax;

  inline void InitParameters(int n){
    for(int i=0; i<n; i++){
      parRangeMins.push_back(0);
      parRangeMaxs.push_back(1);
      parInit.push_back(0.5);
      parInitSet.push_back(false);
    }
  }
  inline void ClearParameters(){
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
  void SetParameter(int i, double min, double max){
    parInitSet.at(i) = true;
    parInit.at(i) = (min+max)/2.;
    parRangeMins.at(i) = min;
    parRangeMaxs.at(i) = max;
  }
  vector<double> parRangeMins, parRangeMaxs, parInit;
  vector<bool> parInitSet;

  void Fit();

  //=== result
  TFitResultPtr fitResultPtr;

private:

  TH1D *hist_original;

  double fitVarRangeMin, fitVarRangeMax;


};

#endif

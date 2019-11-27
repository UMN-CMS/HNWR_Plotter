#ifndef PDFGenerator_h
#define PDFGenerator_h

#include "FitHistogram.C"

class PDFGenerator{

public:

  PDFGenerator();
  ~PDFGenerator();

  inline TString ParNameHelper(TString perfix, int index){
    return perfix+"_p"+TString::Itoa(index,10);
  }
  inline TString XVarHelper(TString xname){
    return xname+"/13000.";
  }

  void Run(FitHistogram& m, TString FuncType, int NPar, TString xname);

};

#endif

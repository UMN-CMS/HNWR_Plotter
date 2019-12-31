#ifndef FunctionGenerator_h
#define FunctionGenerator_h

#include "FitHistogram.C"

class FunctionGenerator{

public:

  FunctionGenerator();
  ~FunctionGenerator();

  double integralValue;
  void Run(FitHistogram& m, TString FuncType, int NPar);

};

#endif

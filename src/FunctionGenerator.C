#include "FunctionGenerator.h"

FunctionGenerator::FunctionGenerator(){
}

FunctionGenerator::~FunctionGenerator(){

}

void FunctionGenerator::Run(FitHistogram& m, TString FuncType, int NPar){

  TString X = "(x/13000.)";

  double this_normSF = integralValue/201.228;

  m.Name = FuncType+"_"+TString::Itoa(NPar,10)+"Par";

  if(FuncType=="Dijet"){

    if(NPar<3){
      NPar=3;
    }

    TString formula_num = "[0] * pow(1-"+X+", [1])";
    TString formula_den = "pow("+X+",[2]";
    for(int i=3; i<NPar; i++){
      formula_den += "+["+TString::Itoa(i,10)+"] * pow(log("+X+"), "+TString::Itoa(i-2,10)+")";
    }
    formula_den += ")";
    m.functionalForm = "( "+formula_num+" )/( "+formula_den+" )";

    m.InitParameters(NPar);
    for(int i=0; i<NPar; i++){
      if(i==0){
        m.parRangeMins.at(i) = 1E-6;
        m.parRangeMaxs.at(i) = 100;
      }
      else if(i==1){
        m.parRangeMins.at(i) = 1;
        m.parRangeMaxs.at(i) = 15;
      }
      else if(i==2){
        m.parRangeMins.at(i) = 1;
        m.parRangeMaxs.at(i) = 15;
      }
      else if(i==3){
        m.parRangeMins.at(i) = 0;
        m.parRangeMaxs.at(i) = 20;
      }
      else{
        m.parRangeMins.at(i) = 0;
        m.parRangeMaxs.at(i) = 1E-2;
      }
    }

  }

  else if(FuncType=="PolyExt"){

    if(NPar<4){
      NPar=4;
    }

    TString formula_den = "pow("+X+",[2]+[3]*log("+X+"))";

    TString formula_num = "[0] * pow(1-"+X+", [1])";
    TString formula_num2 = "1";
    for(int i=4; i<NPar; i++){
      formula_num2 += "+["+TString::Itoa(i,10)+"]*pow("+X+","+TString::Itoa(i-3,10)+")";
    }
    formula_num += " * ("+formula_num2+")";
    m.functionalForm = "( "+formula_num+" )/( "+formula_den+" )";

    m.InitParameters(NPar);
    for(int i=0; i<NPar; i++){
      if(i==0){
        m.parRangeMins.at(i) = 1E-6;
        m.parRangeMaxs.at(i) = 10;
      }
      else if(i==1){
        m.parRangeMins.at(i) = 1;
        m.parRangeMaxs.at(i) = 15.;
      }
      else if(i==2){
        m.parRangeMins.at(i) = 1;
        m.parRangeMaxs.at(i) = 5;
      }
      else{
        m.parRangeMins.at(i) = 0;
        m.parRangeMaxs.at(i) = 1;
      }
    }

  }

  else if(FuncType=="ATLAS"){

    if(NPar<3){
      NPar=3;
    }

    TString formula_den = "pow("+X+",[1])";
    TString formula_num = "[0]";
    TString formula_num2 = "";
    for(int i=2; i<NPar; i++){
      formula_num2 += "-["+TString::Itoa(i,10)+"]*pow("+X+","+TString::Itoa(i-1,10)+")";
    }
    formula_num += " * exp("+formula_num2+")";

    m.functionalForm = "( "+formula_num+" )/( "+formula_den+" )";

    m.InitParameters(NPar);
    for(int i=0; i<NPar; i++){
      if(i==0){
        m.parRangeMins.at(i) = 1E-3*this_normSF;
        m.parRangeMaxs.at(i) = 10*this_normSF;
      }
      else if(i==1){
        m.parRangeMins.at(i) = 1;
        m.parRangeMaxs.at(i) = 15.;
      }
      else if(i==2){
        m.parRangeMins.at(i) = 0;
        m.parRangeMaxs.at(i) = 20.;
      }
      else{
        m.parRangeMins.at(i) = 0;
        m.parRangeMaxs.at(i) = 20.;
      }
    }

  }

  else if(FuncType=="ModifiedExpo"){

    if(NPar!=4){
      NPar=4;
    }

    m.functionalForm = "[0] * exp( -1. * pow( ("+X+"-[1])/[2] , [3]) )";

    m.InitParameters(NPar);
    for(int i=0; i<NPar; i++){
      if(i==0){
        m.parRangeMins.at(i) = 0*this_normSF;
        m.parRangeMaxs.at(i) = 10000*this_normSF;
      }
      else if(i==1){
        m.parRangeMins.at(i) = (100./13000.);
        m.parRangeMaxs.at(i) = (800./13000.);
      }
      else if(i==2){
        m.parRangeMins.at(i) = 0;
        m.parRangeMaxs.at(i) = (1000./13000.);
      }
      else if(i==3){
        m.parRangeMins.at(i) = 0;
        m.parRangeMaxs.at(i) = 3;
      } 
    }
  }


}


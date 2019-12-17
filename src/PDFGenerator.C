#include "PDFGenerator.h"

PDFGenerator::PDFGenerator(){
}

PDFGenerator::~PDFGenerator(){

}

void PDFGenerator::Run(FitHistogram& m, TString FuncType, int NPar, TString xname){

  if(FuncType=="Dijet"){

    if(NPar<3){
      NPar=3;
    }

    TString prefix = "Dijet_"+TString::Itoa(NPar,10)+"Par";

    m.Name = prefix;

    TString formula_num = ParNameHelper(prefix,0)+" * pow(1-"+XVarHelper(xname)+", "+ParNameHelper(prefix,1)+")";
    TString formula_den = "pow("+XVarHelper(xname)+","+ParNameHelper(prefix,2);
    for(int i=3; i<NPar; i++){
      formula_den += "+"+ParNameHelper(prefix,i)+" * pow(log("+XVarHelper(xname)+"), "+TString::Itoa(i-2,10)+")";
    }
    formula_den += ")";
    m.functionalForm = "( "+formula_num+" )/( "+formula_den+" )";

    m.InitParameters(NPar);
    for(int i=0; i<NPar; i++){
      m.parNames.at(i) = m.Name+"_p"+TString::Itoa(i,10);
      if(i==0){
        m.parRangeMins.at(i) = 1;
        m.parRangeMaxs.at(i) = 1;
      }
      else if(i==1){
        m.parRangeMins.at(i) = 0;
        m.parRangeMaxs.at(i) = 30.;
      }
      else if(i==2){
        m.parRangeMins.at(i) = 0;
        m.parRangeMaxs.at(i) = 5;
      }
      else{
        m.parRangeMins.at(i) = 0;
        m.parRangeMaxs.at(i) = 1E-5;
      }
    }

  }

  else if(FuncType=="PolyExt"){

    if(NPar<4){
      NPar=4;
    }

    TString prefix = "PolyExt_"+TString::Itoa(NPar,10)+"Par";

    m.Name = prefix;

    TString formula_num = ParNameHelper(prefix,0)+" * pow(1-"+XVarHelper(xname)+", "+ParNameHelper(prefix,1)+")";
    TString formula_den = "pow("+XVarHelper(xname)+","+ParNameHelper(prefix,2)+"+"+ParNameHelper(prefix,3)+" * log("+XVarHelper(xname)+"))";

    TString formula_num2 = "1";
    for(int i=4; i<NPar; i++){
      formula_num2 += "+"+ParNameHelper(prefix,i)+" * pow("+XVarHelper(xname)+", "+TString::Itoa(i-3,10)+")";
    }
    formula_num += " * ("+formula_num2+")";
    m.functionalForm = "( "+formula_num+" )/( "+formula_den+" )";

    m.InitParameters(NPar);
    for(int i=0; i<NPar; i++){
      m.parNames.at(i) = m.Name+"_p"+TString::Itoa(i,10);
      if(i==0){
        m.parRangeMins.at(i) = 1;
        m.parRangeMaxs.at(i) = 1;
      }
      else{
        m.parRangeMins.at(i) = 0.;
        m.parRangeMaxs.at(i) = 100.;
      }
    }

  }

  else if(FuncType=="ATLAS"){

    if(NPar<4){
      NPar=4;
    }

    TString prefix = "ATLAS_"+TString::Itoa(NPar,10)+"Par";

    m.Name = prefix;

    TString formula_num = ParNameHelper(prefix,0);
    TString formula_den = "pow("+XVarHelper(xname)+","+ParNameHelper(prefix,1)+")";

    TString formula_num2 = "";
    for(int i=2; i<NPar; i++){
      formula_num2 += "-"+ParNameHelper(prefix,i)+" * pow("+XVarHelper(xname)+", "+TString::Itoa(i-1,10)+")";
    }
    formula_num += " * exp("+formula_num2+")";
    m.functionalForm = "( "+formula_num+" )/( "+formula_den+" )";

    m.InitParameters(NPar);
    for(int i=0; i<NPar; i++){
      m.parNames.at(i) = m.Name+"_p"+TString::Itoa(i,10);
      if(i==0){
        m.parRangeMins.at(i) = 1;
        m.parRangeMaxs.at(i) = 1;
      }
      else{
        m.parRangeMins.at(i) = -20;
        m.parRangeMaxs.at(i) = 20.;
      }
    }

  }

  else if(FuncType=="ModifiedExpo"){

    if(NPar<2){
      NPar=2;
    }

    TString prefix = "ModifiedExpo_"+TString::Itoa(NPar,10)+"Par";

    m.Name = prefix;

    TString insideExpo = ParNameHelper(prefix,1)+" * pow("+XVarHelper(xname)+","+ParNameHelper(prefix,2)+")";

    for(int i=3; i<NPar; i++){
      insideExpo += "+"+ParNameHelper(prefix,i)+" * pow(log("+XVarHelper(xname)+"), "+TString::Itoa(i-2,10)+")";
    }
    


  }

  //cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl;
  //cout << "Formula = " << m.functionalForm << endl;
  //cout << "Pars : " << endl;
  for(int i=0; i<NPar; i++){
    cout << m.parNames.at(i) << endl;
  }


}


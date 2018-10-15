#ifndef AnalysisInputs_h
#define AnalysisInputs_h

class AnalysisInputs{
public:
  map<TString, double> MCNormSF, MCNormSF_uncert, CalculatedSysts;
  bool DoPrint;

  AnalysisInputs();
  void SetMCSF(TString filepath, vector<TString> allbkgs);
  void SetCalculatedSysts(TString filepath);

};

AnalysisInputs::AnalysisInputs(){

  DoPrint = true;
  MCNormSF.clear();
  MCNormSF_uncert.clear();
  CalculatedSysts.clear();

}

void AnalysisInputs::SetMCSF(TString filepath, vector<TString> allbkgs){

  //==== Initializing
  for(unsigned int i=0; i<allbkgs.size(); i++){
    MCNormSF[allbkgs.at(i)] = 1.;
    MCNormSF_uncert[allbkgs.at(i)] = 0.;
  }

  if(DoPrint) cout << "[AnalysisInputs::SetMCSF] Get MC SF from " << filepath << endl;

  string elline;
  ifstream in(filepath);
  while(getline(in,elline)){
    std::istringstream is( elline );
    TString sample;
    double MCSF, MCSF_err;
    is >> sample;
    is >> MCSF;
    is >> MCSF_err;

    if(DoPrint) cout << "[AnalysisInputs::SetMCSF] sample : " << sample << endl;
    if(DoPrint) cout << "[AnalysisInputs::SetMCSF] => MCSF = " << MCSF << ", MCSF_err = " << MCSF_err << endl;

    MCNormSF[sample] = MCSF;
    MCNormSF_uncert[sample] = MCSF_err;

  }


}

void AnalysisInputs::SetCalculatedSysts(TString filepath){

  string elline_syst;
  ifstream in_syst(filepath);

  if(DoPrint)  cout << "[AnalysisInputs::SetCalculatedSysts] Get Systematics from " << filepath << endl;

  while(getline(in_syst,elline_syst)){
    std::istringstream is( elline_syst );
    TString source;
    double value;
    is >> source;
    is >> value;
    if(DoPrint) cout << "[AnalysisInputs::SetCalculatedSysts] source : " << source << endl;
    if(DoPrint) cout << "[AnalysisInputs::SetCalculatedSysts] => % sysematic = " << value << endl;
    CalculatedSysts[source] = value;
  }

  double uncert_lumi = CalculatedSysts["Luminosity"];
  double uncert_fake = CalculatedSysts["FakeLooseID"];

}







#endif




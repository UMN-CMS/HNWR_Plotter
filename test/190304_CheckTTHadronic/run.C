#include "canvas_margin.h"
#include "mylib.h"

void run(){

  TString Year = "2017";

  gStyle->SetOptStat(0);

  TH1::SetDefaultSumw2(true);
  TH1::AddDirectory(kFALSE);

  TString WORKING_DIR = getenv("PLOTTER_WORKING_DIR");
  TString dataset = getenv("CATANVERSION");
  TString ENV_PLOT_PATH = getenv("PLOT_PATH");

  TString temp_dir = "Inclusive";

  TString base_filepath = WORKING_DIR+"/rootfiles/"+dataset+"/Regions/"+Year+"/";
/*
  TString base_plotpath = ENV_PLOT_PATH+"/"+dataset+"/TTHadronicCheck/"+Year+"/"+temp_dir+"/";
  if( !gSystem->mkdir(base_plotpath, kTRUE) ){
    cout
    << "###################################################" << endl
    << "Directoy " << base_plotpath << " is created" << endl
    << "###################################################" << endl
    << endl;
  }
*/
  vector<TString> samples = {
"TTJJ_powheg",
"TTLJ_powheg",
"TTLL_powheg",
  };

  vector<TString> regions = {
//==== CR
      "HNWR_SingleElectron_Resolved_DYCR",
      "HNWR_SingleMuon_Resolved_DYCR",
      "HNWR_SingleElectron_OnZ",
      "HNWR_SingleMuon_OnZ",
      "HNWR_SingleElectron_Boosted_DYCR",
      "HNWR_SingleMuon_Boosted_DYCR",
      "HNWR_SingleElectron_EMu_Boosted_CR",
      "HNWR_SingleMuon_EMu_Boosted_CR",
      "HNWR_EMu_Resolved_SR",
      "HNWR_EMu_Resolved_DYCR",
//==== SR
      "HNWR_SingleElectron_Resolved_SR",
      "HNWR_SingleElectron_Boosted_SR",
      "HNWR_SingleMuon_Resolved_SR",
      "HNWR_SingleMuon_Boosted_SR",
  };

  for(unsigned int it_region=0; it_region<regions.size(); it_region++){

    TString region = regions.at(it_region);

    cout << region;

    for(unsigned int it_sample=0; it_sample<samples.size(); it_sample++){

      TString sample = samples.at(it_sample);

      TFile *file = new TFile(base_filepath+"HNWRAnalyzer_"+sample+".root");
      TH1D *hist = (TH1D *)file->Get(region+"/NEvent_"+region);
      double yield = 0.;
      if(hist) yield = hist->GetBinContent(1);
      cout << "\t" << yield;

    }
    cout << endl;

  }


}

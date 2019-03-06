#include "canvas_margin.h"
#include "mylib.h"
#include "LRSMSignalInfo.h"

void run(){

  TString Year = "2016";

  gStyle->SetOptStat(0);

  TH1::SetDefaultSumw2(true);
  TH1::AddDirectory(kFALSE);

  TString WORKING_DIR = getenv("PLOTTER_WORKING_DIR");
  TString dataset = getenv("CATANVERSION");
  TString ENV_PLOT_PATH = getenv("PLOT_PATH");

  TString base_filepath = WORKING_DIR+"/rootfiles/"+dataset+"/Regions/"+Year+"/Signal/";

  vector<TString> channels = {
    "SingleElectron",
    "SingleMuon"
  };

  vector<TString> samples = {
"WRtoNLtoLLJJ_WR1000_N100",
"WRtoNLtoLLJJ_WR1000_N500",
"WRtoNLtoLLJJ_WR4000_N100",
"WRtoNLtoLLJJ_WR4000_N500",
"WRtoNLtoLLJJ_WR4000_N1000",
  };
  vector<TString> samplealiases = {
    "1000\t100",
    "1000\t500",
    "4000\t100",
    "4000\t500",
    "4000\t1000",
  };

  for(unsigned int it_ch=0; it_ch<channels.size(); it_ch++){

    TString channel = channels.at(it_ch);

    cout << "channel = " << channel << endl;

    for(unsigned int it_sample=0; it_sample<samples.size(); it_sample++){

      TString sample = samples.at(it_sample);
      TString samplealias = samplealiases.at(it_sample);

      //==== PYTHIA
      TFile *file_PYTHIA = new TFile(base_filepath+"/HNWRAnalyzer_FullSIM_"+sample+"_13TeV_TuneCUETP8M1.root");
      TH1D *hist_PYTHIA_NoCut = (TH1D *)file_PYTHIA->Get("NoCut_HNWR");
      TH1D *hist_PYTHIA_Resolved = (TH1D *)file_PYTHIA->Get("HNWR_"+channel+"_Resolved_SR/NEvent_HNWR_"+channel+"_Resolved_SR");
      TH1D *hist_PYTHIA_Boosted  = (TH1D *)file_PYTHIA->Get("HNWR_"+channel+"_Boosted_SR/NEvent_HNWR_"+channel+"_Boosted_SR");

      //==== MG
      TFile *file_MG = new TFile(base_filepath+"/HNWRAnalyzer_FullSIM_"+sample+"_MG.root");
      TH1D *hist_MG_NoCut = (TH1D *)file_MG->Get("NoCut_HNWR");
      TH1D *hist_MG_Resolved = (TH1D *)file_MG->Get("HNWR_"+channel+"_Resolved_SR/NEvent_HNWR_"+channel+"_Resolved_SR");
      TH1D *hist_MG_Boosted  = (TH1D *)file_MG->Get("HNWR_"+channel+"_Boosted_SR/NEvent_HNWR_"+channel+"_Boosted_SR");


      cout << samplealias << "\t";
      cout << 2.*hist_PYTHIA_Resolved->GetEntries()/hist_PYTHIA_NoCut->GetEntries() << "\t" << 2.*hist_MG_Resolved->GetEntries()/hist_MG_NoCut->GetEntries() << "\t" << hist_MG_Resolved->GetEntries()/hist_PYTHIA_Resolved->GetEntries() << "\t";
      cout << 2.*hist_PYTHIA_Boosted->GetEntries()/hist_PYTHIA_NoCut->GetEntries() << "\t" << 2.*hist_MG_Boosted->GetEntries()/hist_MG_NoCut->GetEntries() << "\t" << hist_MG_Boosted->GetEntries()/hist_PYTHIA_Boosted->GetEntries() << endl;

    } // END sample loop

  }

}

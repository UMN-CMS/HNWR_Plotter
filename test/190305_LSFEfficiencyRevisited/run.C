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

  TString base_filepath = WORKING_DIR+"/rootfiles/"+dataset+"/HNWRSignalStudy/"+Year+"/";

  vector<TString> channels = {
    "EE",
    "MuMu"
  };

  vector<TString> samples = {
"WRtoNLtoLLJJ_WR1000_N100",
//"WRtoNLtoLLJJ_WR1000_N500",
"WRtoNLtoLLJJ_WR4000_N100",
"WRtoNLtoLLJJ_WR4000_N500",
//"WRtoNLtoLLJJ_WR4000_N1000",
  };
  vector<TString> samplealiases = {
    "1000\t100",
    //"1000\t500",
    "4000\t100",
    "4000\t500",
    //"4000\t1000",
  };

  for(unsigned int it_ch=0; it_ch<channels.size(); it_ch++){

    TString channel = channels.at(it_ch);

    cout << "channel = " << channel << endl;

    for(unsigned int it_sample=0; it_sample<samples.size(); it_sample++){

      TString sample = samples.at(it_sample);
      TString samplealias = samplealiases.at(it_sample);

      //==== PYTHIA
      TFile *file_PYTHIA = new TFile(base_filepath+"/HNWRSignalStudy_FullSIM_"+sample+"_13TeV_TuneCUETP8M1.root");
      TH1D *hist_PYTHIA_Den = (TH1D *)file_PYTHIA->Get(channel+"/GenStudy__fatjet_matched_gen_N__NotEmpty_"+channel);
      TH1D *hist_PYTHIA_SDMass = (TH1D *)file_PYTHIA->Get(channel+"/GenStudy__fatjet_matched_gen_N__SDMass_"+channel);
      TH1D *hist_PYTHIA_LSF = (TH1D *)file_PYTHIA->Get(channel+"/GenStudy__fatjet_matched_gen_N__LSF_"+channel);
      TH1D *hist_PYTHIA_MergedJet = (TH1D *)file_PYTHIA->Get(channel+"/GenStudy__fatjet_matched_gen_N__PassUMNTag_"+channel);

      double den_PYTHIA = hist_PYTHIA_Den->GetBinContent(1);
      double eff_SDMass_PYTHIA = hist_PYTHIA_SDMass->Integral( hist_PYTHIA_SDMass->FindBin(40), 999999 ) / den_PYTHIA;
      double eff_LSF_PYTHIA = hist_PYTHIA_LSF->Integral( hist_PYTHIA_LSF->FindBin(0.7), 999999 ) / den_PYTHIA;
      double eff_Merged_PYTHIA = hist_PYTHIA_MergedJet->GetBinContent(1) / den_PYTHIA;

      //==== MG
      TFile *file_MG = new TFile(base_filepath+"/HNWRSignalStudy_FullSIM_"+sample+"_MG.root");
      TH1D *hist_MG_Den = (TH1D *)file_MG->Get(channel+"/GenStudy__fatjet_matched_gen_N__NotEmpty_"+channel);
      TH1D *hist_MG_SDMass = (TH1D *)file_MG->Get(channel+"/GenStudy__fatjet_matched_gen_N__SDMass_"+channel);
      TH1D *hist_MG_LSF = (TH1D *)file_MG->Get(channel+"/GenStudy__fatjet_matched_gen_N__LSF_"+channel);
      TH1D *hist_MG_MergedJet = (TH1D *)file_MG->Get(channel+"/GenStudy__fatjet_matched_gen_N__PassUMNTag_"+channel);

      double den_MG = hist_MG_Den->GetBinContent(1);
      double eff_SDMass_MG = hist_MG_SDMass->Integral( hist_MG_SDMass->FindBin(40), 999999 ) / den_MG;
      double eff_LSF_MG = hist_MG_LSF->Integral( hist_MG_LSF->FindBin(0.7), 999999 ) / den_MG;
      double eff_Merged_MG = hist_MG_MergedJet->GetBinContent(1) / den_MG;


      cout << samplealias << "\t" << "PYTHIA" << "\t" << eff_SDMass_PYTHIA << "\t" << eff_LSF_PYTHIA << "\t" << eff_Merged_PYTHIA << endl;
      cout << samplealias << "\t" << "MG" << "\t" << eff_SDMass_MG << "\t" << eff_LSF_MG << "\t" << eff_Merged_MG << endl;

    } // END sample loop

  }

}

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
"WRtoNLtoLLJJ_WR1000_N100_13TeV_TuneCUETP8M1",
"WRtoNLtoLLJJ_WR1000_N500_13TeV_TuneCUETP8M1",
"WRtoNLtoLLJJ_WR4000_N100_13TeV_TuneCUETP8M1",
"WRtoNLtoLLJJ_WR4000_N500_13TeV_TuneCUETP8M1",
"WRtoNLtoLLJJ_WR4000_N1000_13TeV_TuneCUETP8M1",
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

      TFile *file_Full = new TFile(base_filepath+"/HNWRSignalStudy_FullSIM_"+sample+".root");

      TH1D *hist_UMN = (TH1D *)file_Full->Get(channel+"/GenStudy__fatjet_matched_gen_N__PassUMNTag_"+channel);
      TH1D *hist_UMN_secLep = (TH1D *)file_Full->Get(channel+"/GenStudy__lep_mathced_gen_secLep_Found_"+channel);

      cout << samplealias << "\t" << hist_UMN_secLep->GetEntries()/hist_UMN->GetEntries() << endl;

    } // END sample loop

  }

}

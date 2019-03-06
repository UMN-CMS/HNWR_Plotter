#include "canvas_margin.h"
#include "mylib.h"

void run(){

  TString Year = "2016";

  gStyle->SetOptStat(0);

  TH1::SetDefaultSumw2(true);
  TH1::AddDirectory(kFALSE);

  TString WORKING_DIR = getenv("PLOTTER_WORKING_DIR");
  TString dataset = getenv("CATANVERSION");
  TString ENV_PLOT_PATH = getenv("PLOT_PATH");

  TString temp_dir = "Inclusive";

  TString base_filepath = WORKING_DIR+"/rootfiles/"+dataset+"/SKFlatValidation/"+Year+"/";
  TString base_plotpath = ENV_PLOT_PATH+"/"+dataset+"/CompareHTBinned/"+Year+"/"+temp_dir+"/";

  if( !gSystem->mkdir(base_plotpath, kTRUE) ){
    cout
    << "###################################################" << endl
    << "Directoy " << base_plotpath << " is created" << endl
    << "###################################################" << endl
    << endl;
  }

  vector<TString> samples = {
    "WJets_MG",
    "DYJets_MG",
  };

  vector<TString> regions = {
    "",
  };


}

#include "canvas_margin.h"
#include "mylib.h"

void Get_DYNormalization(int xxx=0){

  //gErrorIgnoreLevel = kFatal;

  setTDRStyle();

  TString Year = "2016";
  if(xxx==1) Year = "2017";

  gStyle->SetOptStat(0);

  TH1::SetDefaultSumw2(true);
  TH1::AddDirectory(kFALSE);

  TString WORKING_DIR = getenv("PLOTTER_WORKING_DIR");
  TString dataset = getenv("CATANVERSION");
  TString ENV_PLOT_PATH = getenv("PLOT_PATH");

  TString base_filepath = WORKING_DIR+"/rootfiles/"+dataset+"/Regions/"+Year+"/";
  TString base_plotpath = ENV_PLOT_PATH+"/"+dataset+"/DYNormalization/"+Year+"/";

/*
  if( !gSystem->mkdir(base_plotpath, kTRUE) ){
    cout
    << "###################################################" << endl
    << "Directoy " << base_plotpath << " is created" << endl
    << "###################################################" << endl
    << endl;
  }
*/

  //==== MCFR Samples

  vector<TString> bkgds;

  if(Year=="2016"){

    bkgds = {
      "WJets_MG",
      "WZ_pythia", "ZZ_pythia", "WW_pythia",
      "TT_powheg",
    };

  }
  if(Year=="2017"){

    bkgds = {
      "TTLL_powheg", "TTLJ_powheg", "TTJJ_powheg",
      "WJets_MG",
      "ttW", "ttZ", "TTG",
      "WZ_pythia", "ZZ_pythia", "WW_pythia",
      "WWW", "WWZ", "WZZ", "ZZZ",
    };

  }


  TString samplename_DY10to50 = "DYJets10to50";
  TString samplename_DY50 = "DYJets";
  if(Year=="2017"){
    samplename_DY10to50 = "DYJets10to50_MG";
    samplename_DY50 = "DYJets";
  }

  TFile *file_DY10to50 = new TFile(base_filepath+"/HNWRAnalyzer_"+samplename_DY10to50+".root");
  TFile *file_DY50 = new TFile(base_filepath+"/HNWRAnalyzer_"+samplename_DY50+".root");

  for(int it_fl=0; it_fl<2; it_fl++){

    TString leptonFlavour = "Electron";
    if(it_fl==1){
      leptonFlavour = "Muon";
    }

    TString dirname = "HNWR_Single"+leptonFlavour+"_OnZ";
    TString histname = "ZCand_Mass_"+dirname;

    TFile *file_DATA = new TFile(base_filepath+"/HNWRAnalyzer_data_Single"+leptonFlavour+".root");
    TH1D *hist_DATA = (TH1D *)file_DATA->Get(dirname+"/"+histname);

    for(unsigned int it_bkgd=0; it_bkgd<bkgds.size(); it_bkgd++){
      TFile *file_bkgd = new TFile(base_filepath+"/HNWRAnalyzer_"+bkgds.at(it_bkgd)+".root");
      TH1D *hist_bkgd = (TH1D *)file_bkgd->Get(dirname+"/"+histname);
      if(!hist_bkgd) continue;
      hist_DATA->Add(hist_bkgd, -1.);
    }

    TH1D *hist_DY10to50 = (TH1D *)file_DY10to50->Get(dirname+"/"+histname);
    TH1D *hist_DY50 = (TH1D *)file_DY50->Get(dirname+"/"+histname);

    if(hist_DY10to50) hist_DY50->Add(hist_DY10to50);

    cout << leptonFlavour << "\t" << hist_DATA->Integral() / hist_DY50->Integral() << endl;


  }

}

#include "canvas_margin.h"
#include "mylib.h"
#include "ObsPredComp.h"

void Draw_BinnedSample(int xxx=0){

  bool DrawCompPlot = true;

  setTDRStyle();

  TString Year = "2016";
  TString TotalLumi = "35.9 fb^{-1} (13 TeV)";
  if(xxx==1){
    Year = "2017";
    TotalLumi = "41.5 fb^{-1} (13 TeV)";
  }
  if(xxx==2){
    Year = "2018";
    TotalLumi = "60. fb^{-1} (13 TeV)";
  }

  gStyle->SetOptStat(0);

  TH1::SetDefaultSumw2(true);
  TH1::AddDirectory(kFALSE);

  TString WORKING_DIR = getenv("PLOTTER_WORKING_DIR");
  TString dataset = getenv("CATANVERSION");
  TString ENV_PLOT_PATH = getenv("PLOT_PATH");

  TString base_filepath = WORKING_DIR+"/rootfiles/"+dataset+"/Regions/"+Year+"/";
  TString base_plotpath = ENV_PLOT_PATH+"/"+dataset+"/BinnedSample/"+Year+"/";

  if( !gSystem->mkdir(base_plotpath, kTRUE) ){
    cout
    << "###################################################" << endl
    << "Directoy " << base_plotpath << " is created" << endl
    << "###################################################" << endl
    << endl;
  }

  vector< TString > samples_Incl = {
    "DYJets",
    "WJets_MG",
  };
  vector< vector<TString> > lists_Binned = {
    {"DYJets_MG_HT-70To100", "DYJets_MG_HT-100To200", "DYJets_MG_HT-200To400", "DYJets_MG_HT-400To600", "DYJets_MG_HT-600To800", "DYJets_MG_HT-800To1200", "DYJets_MG_HT-1200To2500", "DYJets_MG_HT-2500ToInf"},
    {"WJets_MG_HT-70To100", "WJets_MG_HT-100To200", "WJets_MG_HT-200To400", "WJets_MG_HT-400To600", "WJets_MG_HT-600To800", "WJets_MG_HT-800To1200", "WJets_MG_HT-1200To2500", "WJets_MG_HT-2500ToInf"},
  };

  //==== Regions

  vector<TString> regions = {
    "HNWR_SingleElectron_Resolved_SR",
    "HNWR_SingleMuon_Resolved_SR",
    "HNWR_SingleElectron_Boosted_SR",
    "HNWR_SingleMuon_Boosted_SR",
  };

  //==== Variables

  vector<TString> vars = {
    "WRCand_Mass",
  };

  vector<TString> xtitles = {
    "m(W_{R}) (GeV)",
  };

  vector<int> rebins = {
    50,
  };

  for(unsigned int it_Incl=0; it_Incl<samples_Incl.size(); it_Incl++){

    TString sample_Incl = samples_Incl.at(it_Incl);
    vector<TString> list_Binned = lists_Binned.at(it_Incl);

    TFile *file_Incl = new TFile(base_filepath+"/HNWRAnalyzer_"+sample_Incl+".root");
    vector<TFile *> files_Binned;
    for(unsigned int it_Binned=0; it_Binned<list_Binned.size(); it_Binned++){
      TFile *file_Binned = new TFile(base_filepath+"/HNWRAnalyzer_"+list_Binned.at(it_Binned)+".root");;
      files_Binned.push_back( file_Binned );
    }

    for(unsigned it_region=0; it_region<regions.size(); it_region++){

      TString region = regions.at(it_region);

      for(unsigned int it_var=0; it_var<vars.size(); it_var++){

        TString var = vars.at(it_var);

        TH1D *hist_Incl = (TH1D *)file_Incl->Get(region+"/"+var+"_"+region);
        if(!hist_Incl){
          //cout << region+"/"+var+"_"+region << endl;
          continue;
        }

        TH1D *hist_Binned = NULL;
        for(unsigned int it_Binned=0; it_Binned<list_Binned.size(); it_Binned++){
          TH1D *this_hist_Binned = (TH1D *)files_Binned.at(it_Binned)->Get(region+"/"+var+"_"+region);
          if(!this_hist_Binned) continue;
          if(!hist_Binned) hist_Binned = (TH1D *)this_hist_Binned->Clone();
          else hist_Binned->Add(this_hist_Binned);
        }

        //==== rebin
        int nrebin = rebins.at(it_var);
        hist_Incl->Rebin(nrebin);
        hist_Binned->Rebin(nrebin);

        ObsPredComp m_obj;
        m_obj.hist_Obs = hist_Incl;
        m_obj.hist_Pred = hist_Binned;
        m_obj.alias_Obs = "Inclusive";
        m_obj.alias_Pred = "Binned";
        m_obj.x_title = var;
        m_obj.TotalLumi = TotalLumi;
        TString this_outputdir = base_plotpath+sample_Incl+"/";
        gSystem->mkdir(this_outputdir,kTRUE);
        m_obj.outputpath = this_outputdir+var+"_"+region;
        m_obj.Run();
        
      }

    }

  }
  

}

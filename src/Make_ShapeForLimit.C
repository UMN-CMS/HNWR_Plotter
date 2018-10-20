#include "canvas_margin.h"
#include "LRSMSignalInfo.h"

void Make_ShapeForLimit(){

  TString ShaprVarName = "WRCand_Mass";
  int n_rebin = 10;

  double const_bkgderr = 0.20;
  double const_sigerr = 0.1;

  double signal_scale = 10.*0.001; 

  gStyle->SetOptStat(0);

  TH1::SetDefaultSumw2(true);
  TH1::AddDirectory(kFALSE);

  TString WORKING_DIR = getenv("PLOTTER_WORKING_DIR");
  TString dataset = getenv("CATANVERSION");
  TString ENV_PLOT_PATH = getenv("PLOT_PATH");

  TString base_filepath = WORKING_DIR+"/rootfiles/"+dataset+"/Regions/";
  TString base_plotpath = ENV_PLOT_PATH+"/"+dataset+"/ShapeForLimit/";

  LRSMSignalInfo lrsminfo;
  lrsminfo.GetMassMaps();

  vector<TString> regions = {
    "OneLepton_AwayFatJetWithLepton100GeV",
    "TwoLepton_TwoJet_mllgt150_OS",
    "TwoLepton_TwoJet_mllgt150_SS",
  };
  vector<TString> bkgds_OneLepton = {
"TTLL_powheg", "TTLJ_powheg", "TTJJ_powheg",
"WJets_MG",
"DYJets10to50_MG", "DYJets",
"WZ_pythia", "ZZ_pythia", "WW_pythia",
  };
  vector<TString> bkgds_OS = {
"TTLL_powheg",
"DYJets10to50_MG", "DYJets",
"ZZTo2L2Q", "ZZTo4L_powheg", "WZTo2L2Q", "WZTo3LNu", "WWTo2L2Nu_powheg",
  };
  vector<TString> bkgds_SS = {
"fake",
"chargeflip",
"ZZTo4L_powheg", "WZTo3LNu",
  };
  vector< vector<TString> > bkgdss = {
    bkgds_OneLepton,
    bkgds_OS,
    bkgds_SS,
  };

  vector<TString> channels = {
    //"EE",
    "MuMu",
  };
  vector< TString > Suffixs = {
    //"HNWR_SingleElectron",
    "HNWR_SingleMuon_IsoMu27",
  };

  for(unsigned int it_region=0; it_region<regions.size(); it_region++){

    TString region = regions.at(it_region);
    vector<TString> bkgds = bkgdss.at(it_region);

    for(unsigned int it_channel=0; it_channel<channels.size(); it_channel++){

      TString channel = channels.at(it_channel);
      TString Suffix = Suffixs.at(it_channel);

      TString dirname = Suffix+"_"+region;
      TString histname = ShaprVarName+"_"+dirname;

      TString PD = "SingleElectron";
      if(channel=="MuMu") PD = "SingleMuon";

      //gSystem->mkdir(base_plotpath+"/"+region+"/",kTRUE);
      TFile *out = new TFile(base_plotpath+"/"+channel+"_"+region+".root","RECREATE");

      //==== DATA

      TFile *file_DATA = new TFile(base_filepath+"/HNWRAnalyzer_data_"+PD+".root");
      TDirectory *dir_DATA = (TDirectory *)file_DATA->Get(dirname);
      TH1D *hist_DATA = (TH1D *)dir_DATA->Get(histname);
      hist_DATA->SetName("data_obs");
      hist_DATA->Rebin(n_rebin);

      //==== Bkgd

      TH1D *hist_bkgdtotal = NULL;
      TH1D *hist_bkgdtotal_Up = NULL;
      TH1D *hist_bkgdtotal_Down = NULL;
      for(unsigned int it_bkgd=0; it_bkgd<bkgds.size(); it_bkgd++){
        TString bkgd = bkgds.at(it_bkgd);
        TString filename = "HNWRAnalyzer_"+bkgd+".root";
        if(bkgd=="fake"||bkgd=="chargeflip") filename = "HNWRAnalyzer_"+bkgd+"_"+PD+".root";

        TFile *file_bkgd = new TFile(base_filepath+"/"+filename);
        TDirectory *dir_bkgd = (TDirectory *)file_bkgd->Get(dirname);
        if(dir_bkgd){
          TH1D *hist_bkgd = (TH1D *)dir_bkgd->Get(histname);
          if(hist_bkgd){

            hist_bkgd->Rebin(n_rebin);

            TH1D *hist_bkgd_Up = (TH1D *)hist_bkgd->Clone();
            hist_bkgd_Up->Scale(1.+const_bkgderr);
            TH1D *hist_bkgd_Down = (TH1D *)hist_bkgd->Clone();
            hist_bkgd_Down->Scale(1.-const_bkgderr);

            if(!hist_bkgdtotal){
              hist_bkgdtotal = (TH1D *)hist_bkgd->Clone();
              hist_bkgdtotal_Up = (TH1D *)hist_bkgd_Up->Clone();
              hist_bkgdtotal_Down = (TH1D *)hist_bkgd_Down->Clone();
            }
            else{
             hist_bkgdtotal->Add(hist_bkgd);
             hist_bkgdtotal_Up->Add(hist_bkgd_Up);
             hist_bkgdtotal_Down->Add(hist_bkgd_Down);
            }

          }
        }
        file_bkgd->Close();

      }
      hist_bkgdtotal->SetName("background");
      hist_bkgdtotal_Up->SetName("background_alphaUp");
      hist_bkgdtotal_Down->SetName("background_alphaDown");

      out->cd();
      hist_DATA->Write();
      hist_bkgdtotal->Write();
      hist_bkgdtotal_Up->Write();
      hist_bkgdtotal_Down->Write();

      //==== Signals

      for(map< int, vector<int> >::iterator it=lrsminfo.maps_WR_to_N.begin(); it!=lrsminfo.maps_WR_to_N.end(); it++){

        int m_WR = it->first;
        vector<int> this_m_Ns = it->second;

        for(int it_N=0; it_N<this_m_Ns.size(); it_N++){

          int m_N = this_m_Ns.at(it_N);

          TString this_filename = "HNWRAnalyzer_WR_"+channel+"JJ_WR"+TString::Itoa(m_WR,10)+"_N"+TString::Itoa(m_N,10)+".root";
          TFile *file_sig = new TFile(base_filepath+"/Signal/"+this_filename);
          TDirectory *dir_sig = (TDirectory *)file_sig->Get(dirname);

          if(dir_sig){

            TH1D *hist_sig = (TH1D *)dir_sig->Get(histname);

            if(hist_sig){
              hist_sig->Rebin(n_rebin);
              hist_sig->SetName("WR"+TString::Itoa(m_WR,10)+"_N"+TString::Itoa(m_N,10));

              hist_sig->Scale(signal_scale);

              TH1D *hist_sig_Up = (TH1D *)hist_sig->Clone();
              hist_sig_Up->Scale(1.+const_sigerr);
              hist_sig_Up->SetName("WR"+TString::Itoa(m_WR,10)+"_N"+TString::Itoa(m_N,10)+"_sigmaUp");

              TH1D *hist_sig_Down = (TH1D *)hist_sig->Clone();
              hist_sig_Down->Scale(1.-const_sigerr);
              hist_sig_Down->SetName("WR"+TString::Itoa(m_WR,10)+"_N"+TString::Itoa(m_N,10)+"_sigmaDown");

              out->cd();
              hist_sig->Write();
              hist_sig_Up->Write();
              hist_sig_Down->Write();
            }
            file_sig->Close();

          }

        }

      }

      out->Close();

      file_DATA->Close();


    }

  }

}

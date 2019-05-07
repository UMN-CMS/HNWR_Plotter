#include "canvas_margin.h"
#include "LRSMSignalInfo.h"
#include "mylib.h"

void Make_ShapeForLimit(int Year=2017){

  TString ShaprVarName = "WRCand_Mass";
  int n_rebin = 20;

  double signal_scale = 10.*0.001; // r value in fb

  TString filename_prefix = "HNWRAnalyzer_SkimTree_LRSMHighPt_";

  //==== FIXME now only have 2017 signals
  if(Year==2016){
    signal_scale *= 35.92/41.53;
  }
  else if(Year==2018){
    signal_scale *= 59.74/41.53;
  }

  vector<TString> systs = {
    "Central",
    "JetResUp", "JetResDown",
    "JetEnUp", "JetEnDown",
    "MuonEnUp", "MuonEnDown",
    "MuonIDSFUp", "MuonIDSFDown",
    "ElectronResUp", "ElectronResDown",
    "ElectronEnUp", "ElectronEnDown",
    "ElectronIDSFUp", "ElectronIDSFDown",
  };

  gStyle->SetOptStat(0);

  TH1::SetDefaultSumw2(true);
  TH1::AddDirectory(kFALSE);

  TString WORKING_DIR = getenv("PLOTTER_WORKING_DIR");
  TString dataset = getenv("CATANVERSION");
  TString ENV_PLOT_PATH = getenv("PLOT_PATH");

  TString base_filepath = WORKING_DIR+"/rootfiles/"+dataset+"/Regions/"+TString::Itoa(Year,10)+"/";
  TString base_plotpath = ENV_PLOT_PATH+"/"+dataset+"/ShapeForLimit/"+TString::Itoa(Year,10)+"/";

  gSystem->mkdir(base_plotpath,kTRUE);

  LRSMSignalInfo lrsminfo;
  lrsminfo.GetMassMaps();

  //==== bkgds

  map< TString, vector<TString> > map_sample_string_to_list;

  map_sample_string_to_list["ZJets_Reweighted"] = {"DYJets10to50_MG_Reweighted", "DYJets_Reweighted"};
  if(Year==2016){
    map_sample_string_to_list["ZJets_Reweighted"] = {"DYJets10to50_Reweighted", "DYJets_Reweighted"};
  }
  else if(Year==2017){
    map_sample_string_to_list["ZJets_Reweighted"] = {"DYJets10to50_MG_Reweighted", "DYJets_Reweighted"};
  }
  else if(Year==2018){
    map_sample_string_to_list["ZJets_Reweighted"] = {"DYJets10to50_MG_Reweighted", "DYJets_MG_Reweighted"};
  }
  map_sample_string_to_list["WJets_MG"] = {"WJets_MG"};
  map_sample_string_to_list["VV_incl"] = {"WZ_pythia", "ZZ_pythia", "WW_pythia"};
  map_sample_string_to_list["EMuMethod"] = {"EMuMethod_TTLX_powheg"};
  map_sample_string_to_list["VVV"] = {"WWW", "WWZ", "WZZ", "ZZZ"};
  map_sample_string_to_list["ttX"] = {"ttW", "ttZ"};

  vector<TString> regions = {
      "Resolved_SR",
      "Boosted_SR",
  };
  vector<TString> bkgds = {
"VVV", "VV_incl", "ttX", "SingleTop", "WJets_MG", "ZJets_Reweighted", "EMuMethod",
  };

  vector<TString> channels = {
    //"EE",
    "MuMu",
  };
  vector< TString > Suffixs = {
    //"HNWR_SingleElectron",
    "HNWR_SingleMuon",
  };

  bool SamplePrinted = false;

  for(unsigned int it_region=0; it_region<regions.size(); it_region++){

    TString region = regions.at(it_region);

    for(unsigned int it_channel=0; it_channel<channels.size(); it_channel++){

      TString channel = channels.at(it_channel);
      TString Suffix = Suffixs.at(it_channel);

      TString dirname = Suffix+"_"+region;
      TString histname = ShaprVarName+"_"+dirname;

      TString PD = "SingleElectron";
      if(channel=="MuMu") PD = "SingleMuon";

      TFile *out = new TFile(base_plotpath+"/"+channel+"_"+region+".root","RECREATE");

      //==== DATA

      TFile *file_DATA = new TFile(base_filepath+"/"+filename_prefix+"data_"+PD+".root");
      TDirectory *dir_DATA = (TDirectory *)file_DATA->Get(dirname);
      TH1D *hist_DATA = (TH1D *)dir_DATA->Get(histname);
      hist_DATA->SetName("data_obs");

      hist_DATA->Rebin(n_rebin);

      out->cd();
      hist_DATA->Write();

      //==== sample

      for(unsigned it_syst=0; it_syst<systs.size(); it_syst++){

        TString syst = systs.at(it_syst);
        TString shapehistname_suffix = "";

        if(syst=="Central"){
          dirname = Suffix+"_"+region;
          histname = ShaprVarName+"_"+dirname;
          shapehistname_suffix = "";
        }
        else{
          dirname = "Syst_"+syst+"_"+Suffix+"_"+region;
          histname = ShaprVarName+"_"+dirname;
          shapehistname_suffix = "_"+syst;
        }

        vector<TString> samplelist;
        for(unsigned int i=0; i<bkgds.size(); i++){
          samplelist.insert(samplelist.end(),
                            map_sample_string_to_list[bkgds.at(i)].begin(),
                            map_sample_string_to_list[bkgds.at(i)].end()
                            );
        }

        if(!SamplePrinted){
          for(unsigned int i=0; i<samplelist.size(); i++){
            cout << samplelist.at(i) << endl;
          }
          SamplePrinted = true;
        }

        for(unsigned int it_sample=0; it_sample<samplelist.size(); it_sample++){
          TString sample = samplelist.at(it_sample);
          TString filename = filename_prefix+sample+".root";

          TFile *file_sample = new TFile(base_filepath+"/"+filename);
          TDirectory *dir_sample = (TDirectory *)file_sample->Get(dirname);
          //cout << "file = " << base_filepath+"/"+filename << endl;
          //cout << "dirname = " << dirname << endl;
          if(dir_sample){
            TH1D *hist_bkgd = (TH1D *)dir_sample->Get(histname);

            //==== DY Norm
            if(sample.Contains("DYJets_")){
              hist_bkgd->Scale( GetDYNormSF(Year, PD) );
            }

            if(hist_bkgd){

              hist_bkgd->Rebin(n_rebin);

              //==== If MC
              if(sample=="EMuMethod_TTLX_powheg" && syst=="Central"){

                hist_bkgd->SetName("EMu"+shapehistname_suffix);

                TH1D *hist_bkgdUp = GetScaleUpDown(hist_bkgd,+0.20);
                hist_bkgdUp->SetName("EMu_SystUp");
                TH1D *hist_bkgdDown = GetScaleUpDown(hist_bkgd,-0.20);
                hist_bkgdDown->SetName("EMu_SystDown");

                TH1D *hist_bkgd_StatUp = GetStatUpDown(hist_bkgd,+1);
                hist_bkgd_StatUp->SetName("EMu_StatUp");
                TH1D *hist_bkgd_StatDown = GetStatUpDown(hist_bkgd,-1);
                hist_bkgd_StatDown->SetName("EMu_StatDown");

                out->cd();

                hist_bkgd->Write();
                hist_bkgdUp->Write();
                hist_bkgdDown->Write();
                hist_bkgd_StatUp->Write();
                hist_bkgd_StatDown->Write();

              }
              else{

                if(syst=="Central"){

                  TH1D *hist_bkgdstatup = GetStatUpDown(hist_bkgd,+1);
                  hist_bkgdstatup->SetName(sample+"_StatUp");
                  TH1D *hist_bkgdstatdown = GetStatUpDown(hist_bkgd,-1);
                  hist_bkgdstatdown->SetName(sample+"_StatDown");

                  out->cd();
                  hist_bkgdstatup->Write();
                  hist_bkgdstatdown->Write();

                }

                hist_bkgd->SetName(sample+shapehistname_suffix);

                out->cd();
                hist_bkgd->Write();

              }

            }

          }
          file_sample->Close();

        }


        //==== Signals

        for(map< double, vector<double> >::iterator it=lrsminfo.maps_WR_to_N.begin(); it!=lrsminfo.maps_WR_to_N.end(); it++){

          double m_WR = it->first;
          vector<double> this_m_Ns = it->second;

          for(int it_N=0; it_N<this_m_Ns.size(); it_N++){

            double m_N = this_m_Ns.at(it_N);

            TString this_filename = "HNWRAnalyzer_WR_"+channel+"JJ_WR"+TString::Itoa(m_WR,10)+"_N"+TString::Itoa(m_N,10)+".root";

            //==== FIXME now only have 2017 signals
            TString temp_base_filepath = WORKING_DIR+"/rootfiles/"+dataset+"/Regions/2017/";
            TFile *file_sig = new TFile(temp_base_filepath+"/Signal/"+this_filename);
            TDirectory *dir_sig = (TDirectory *)file_sig->Get(dirname);

            if(dir_sig){

              TH1D *hist_sig = (TH1D *)dir_sig->Get(histname);

              if(hist_sig){
                hist_sig->Rebin(n_rebin);
                hist_sig->SetName("WR"+TString::Itoa(m_WR,10)+"_N"+TString::Itoa(m_N,10)+shapehistname_suffix);

                for(int ix=1;ix<hist_sig->GetXaxis()->GetNbins();ix++){
                  double y=hist_sig->GetBinContent(ix);
                  double e=hist_sig->GetBinError(ix);
                  hist_sig->SetBinContent(ix,y*signal_scale);
                  hist_sig->SetBinError(ix,e*signal_scale);
                }

                out->cd();
                hist_sig->Write();


                if(syst=="Central"){
                  TH1D *hist_sigstatup = GetStatUpDown(hist_sig,+1);
                  hist_sigstatup->SetName("WR"+TString::Itoa(m_WR,10)+"_N"+TString::Itoa(m_N,10)+"_StatUp");
                  TH1D *hist_sigstatdown = GetStatUpDown(hist_sig,+1);
                  hist_sigstatdown->SetName("WR"+TString::Itoa(m_WR,10)+"_N"+TString::Itoa(m_N,10)+"_StatDown");
                  hist_sigstatup->Write();
                  hist_sigstatdown->Write();
                }


              }
              file_sig->Close();

            }

          }

        } // End Loop signal

      }

      out->Close();

      file_DATA->Close();


    }

  }

}

#include "canvas_margin.h"
#include "LRSMSignalInfo.h"
#include "mylib.h"

void Make_ShapeForLimit(){

  TString ShaprVarName = "WRCand_Mass";
  int n_rebin = 50;

  double signal_scale = 10.*0.001; // r value in fb

  vector<TString> systs = {
    "Central",
    "JetResUp", "JetResDown",
    "JetEnUp", "JetEnDown",
    "MuonEnUp", "MuonEnDown",
    "ElectronResUp", "ElectronResDown",
    "ElectronEnUp", "ElectronEnDown",
  };

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

  //==== bkgds

  map< TString, vector<TString> > map_sample_string_to_list;
  map_sample_string_to_list["DY"] = {"DYJets10to50_MG", "DYJets"};
  map_sample_string_to_list["ZToLL"] = {"DYJets10to50_MG", "ZToLL"};
  map_sample_string_to_list["WJets_MG"] = {"WJets_MG"};
  map_sample_string_to_list["VV_incl"] = {"WZ_pythia", "ZZ_pythia", "WW_pythia"};
  map_sample_string_to_list["VV_excl"] = {"ZZTo2L2Q", "ZZTo4L_powheg", "WZTo2L2Q", "WZTo3LNu", "WWTo2L2Nu_powheg"};
  map_sample_string_to_list["ttbar"] = {"TTLL_powheg", "TTLJ_powheg", "TTJJ_powheg"};
  map_sample_string_to_list["VVV"] = {"WWW", "WWZ", "WZZ", "ZZZ"};
  map_sample_string_to_list["SingleTop"] = {"SingleTop_sch", "SingleTop_tW_antitop", "SingleTop_tW_top", "SingleTop_tch_antitop", "SingleTop_tch_top"};
  map_sample_string_to_list["ttX"] = {"ttW", "ttZ", "TTG"};
  map_sample_string_to_list["chargeflip"] = {"chargeflip"};
  map_sample_string_to_list["fake"] = {"fake"};



  vector<TString> regions = {
    "OneLepton_AwayFatJetWithSFLepton100GeV",
    "TwoLepton_TwoJet_mllgt150_OS",
    "TwoLepton_TwoJet_mllgt150_SS",
  };
  vector<TString> bkgds_OneLepton = {
"DY", "VVV", "VV_incl", "ttX", "SingleTop", "WJets_MG", "ttbar"
  };
  vector<TString> bkgds_OS = {
"SingleTop", "VVV", "WJets_MG", "VV_incl", "ttbar", "DY"
  };
  vector<TString> bkgds_SS = {
"VVV", "VV_incl", "fake", "chargeflip"
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
    "HNWR_SingleMuon",
  };

  for(unsigned int it_region=0; it_region<regions.size(); it_region++){

    TString region = regions.at(it_region);
    vector<TString> bkgds = bkgdss.at(it_region);

    //==== prepare rebin
    vector<double> vec_NewRebin;
    for(int i=0;i<=10;i++){
      vec_NewRebin.push_back( 200.*i );
      //==== 0~2000
    }

    if(region=="OneLepton_AwayFatJetWithSFLepton100GeV"){

      for(int i=1;i<=2;i++){
        vec_NewRebin.push_back( 2000. + 500.*i );
        //==== 2500, 3000
      }
      vec_NewRebin.push_back(8000.);
    }
    else if(region=="TwoLepton_TwoJet_mllgt150_OS"){
      for(int i=1;i<=4;i++){
        vec_NewRebin.push_back( 2000. + 500.*i );
        //==== 2500, 3000, 3500, 4000.
      }
      vec_NewRebin.push_back(8000.);
    }
    else if(region=="TwoLepton_TwoJet_mllgt150_SS"){
      for(int i=1;i<=3;i++){
        vec_NewRebin.push_back( 2000. + 500.*i );
        //==== 2500, 3000, 3500,
      }
      vec_NewRebin.push_back(8000.);
    }  

    int n_NewBin = vec_NewRebin.size()-1;
    double NewRebin[n_NewBin+1];
    cout << "#### Check rebinning ####" << endl;
    cout << "region = " << region << endl;
    for(unsigned int i=0; i<n_NewBin+1; i++){
      NewRebin[i] = vec_NewRebin.at(i);
      cout << i << "\t" << NewRebin[i] << endl;
    }

    for(unsigned int it_channel=0; it_channel<channels.size(); it_channel++){

      TString channel = channels.at(it_channel);
      TString Suffix = Suffixs.at(it_channel);

      TString dirname = Suffix+"_"+region;
      TString histname = ShaprVarName+"_"+dirname;

      TString PD = "SingleElectron";
      if(channel=="MuMu") PD = "SingleMuon";

      TFile *out = new TFile(base_plotpath+"/"+channel+"_"+region+".root","RECREATE");

      //==== DATA

      TFile *file_DATA = new TFile(base_filepath+"/HNWRAnalyzer_data_"+PD+".root");
      TDirectory *dir_DATA = (TDirectory *)file_DATA->Get(dirname);
      TH1D *hist_DATA = (TH1D *)dir_DATA->Get(histname);
      hist_DATA->SetName("data_obs");

      hist_DATA->Rebin(n_rebin);
      //hist_DATA = (TH1D *)hist_DATA->Rebin(n_NewBin, hist_DATA->GetName(), NewRebin);

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

        TH1D *hist_bkgd_MC = NULL;
        TH1D *hist_bkgd_fake = NULL;
        TH1D *hist_bkgd_chargeflip = NULL;

        vector<TString> samplelist;
        for(unsigned int i=0; i<bkgds.size(); i++){
          samplelist.insert(samplelist.end(),
                            map_sample_string_to_list[bkgds.at(i)].begin(),
                            map_sample_string_to_list[bkgds.at(i)].end()
                            );
        }

        for(unsigned int it_sample=0; it_sample<samplelist.size(); it_sample++){
          TString sample = samplelist.at(it_sample);
          TString filename = "HNWRAnalyzer_"+sample+".root";
          if(sample=="fake"||sample=="chargeflip"){
            if(syst!="Central") continue;
            filename = "HNWRAnalyzer_"+sample+"_"+PD+".root";
          }

          TFile *file_sample = new TFile(base_filepath+"/"+filename);
          TDirectory *dir_sample = (TDirectory *)file_sample->Get(dirname);
          //cout << "file = " << base_filepath+"/"+filename << endl;
          //cout << "dirname = " << dirname << endl;
          if(dir_sample){
            TH1D *hist_bkgd = (TH1D *)dir_sample->Get(histname);

            if(hist_bkgd){

              hist_bkgd->Rebin(n_rebin);
              //hist_bkgd = (TH1D *)hist_bkgd->Rebin(n_NewBin, hist_bkgd->GetName(), NewRebin);

              //==== If MC
              if(sample=="fake"){
                hist_bkgd_fake = (TH1D *)hist_bkgd->Clone();
              }
              else if(sample=="chargeflip"){
                hist_bkgd_chargeflip = (TH1D *)hist_bkgd->Clone();
              }
              else{
                if(!hist_bkgd_MC){
                  hist_bkgd_MC = (TH1D *)hist_bkgd->Clone();
                }
                else{
                 hist_bkgd_MC->Add(hist_bkgd);
                }
              }

            }

          }
          file_sample->Close();

        }
        hist_bkgd_MC->SetName("bkgd_MC"+shapehistname_suffix);

        out->cd();
        hist_bkgd_MC->Write();


        //==== Make Stat up/down
        if(syst=="Central"){
          TH1D *hist_bkgdstatup = GetStatUpDown(hist_bkgd_MC,+1);
          hist_bkgdstatup->SetName("bkgd_MC_StatUp");
          TH1D *hist_bkgdstatdown = GetStatUpDown(hist_bkgd_MC,-1);
          hist_bkgdstatdown->SetName("bkgd_MC_StatDown");
          hist_bkgdstatup->Write();
          hist_bkgdstatdown->Write();

          if(hist_bkgd_fake){
            hist_bkgd_fake->SetName("bkgd_fake"+shapehistname_suffix);

            TH1D *hist_bkgd_fakeUp = GetScaleUpDown(hist_bkgd_fake,+0.30);
            hist_bkgd_fakeUp->SetName("bkgd_fake_FakeSystUp");
            TH1D *hist_bkgd_fakeDown = GetScaleUpDown(hist_bkgd_fake,+0.30);
            hist_bkgd_fakeDown->SetName("bkgd_fake_FakeSystDown");

            TH1D *hist_bkgd_fake_StatUp = GetStatUpDown(hist_bkgd_fake,+1);
            hist_bkgd_fake_StatUp->SetName("bkgd_fake_StatUp");
            TH1D *hist_bkgd_fake_StatDown = GetStatUpDown(hist_bkgd_fake,-1);
            hist_bkgd_fake_StatDown->SetName("bkgd_fake_StatDown");

            hist_bkgd_fake->Write();
            hist_bkgd_fakeUp->Write();
            hist_bkgd_fakeDown->Write();
            hist_bkgd_fake_StatUp->Write();
            hist_bkgd_fake_StatDown->Write();

          }
          if(hist_bkgd_chargeflip){

            hist_bkgd_chargeflip->SetName("bkgd_chargeflip"+shapehistname_suffix);

            TH1D *hist_bkgd_chargeflipUp = GetScaleUpDown(hist_bkgd_chargeflip,+0.30);
            hist_bkgd_chargeflipUp->SetName("bkgd_chargeflip_chargeflipSystUp");
            TH1D *hist_bkgd_chargeflipDown = GetScaleUpDown(hist_bkgd_chargeflip,+0.30);
            hist_bkgd_chargeflipDown->SetName("bkgd_chargeflip_chargeflipSystDown");

            TH1D *hist_bkgd_chargeflip_StatUp = GetStatUpDown(hist_bkgd_chargeflip,+1);
            hist_bkgd_chargeflip_StatUp->SetName("bkgd_chargeflip_StatUp");
            TH1D *hist_bkgd_chargeflip_StatDown = GetStatUpDown(hist_bkgd_chargeflip,-1);
            hist_bkgd_chargeflip_StatDown->SetName("bkgd_chargeflip_StatDown");

            hist_bkgd_chargeflip->Write();
            hist_bkgd_chargeflipUp->Write();
            hist_bkgd_chargeflipDown->Write();
            hist_bkgd_chargeflip_StatUp->Write();
            hist_bkgd_chargeflip_StatDown->Write();

          }

        }


        //==== Signals

        for(map< double, vector<double> >::iterator it=lrsminfo.maps_WR_to_N.begin(); it!=lrsminfo.maps_WR_to_N.end(); it++){

          double m_WR = it->first;
          vector<double> this_m_Ns = it->second;

          for(int it_N=0; it_N<this_m_Ns.size(); it_N++){

            double m_N = this_m_Ns.at(it_N);

            TString this_filename = "HNWRAnalyzer_WR_"+channel+"JJ_WR"+TString::Itoa(m_WR,10)+"_N"+TString::Itoa(m_N,10)+".root";
            TFile *file_sig = new TFile(base_filepath+"/Signal/"+this_filename);
            TDirectory *dir_sig = (TDirectory *)file_sig->Get(dirname);

            if(dir_sig){

              TH1D *hist_sig = (TH1D *)dir_sig->Get(histname);

              if(hist_sig){
                hist_sig->Rebin(n_rebin);
                //hist_sig = (TH1D *)hist_sig->Rebin(n_NewBin, hist_sig->GetName(), NewRebin);
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

        }

      }

      out->Close();

      file_DATA->Close();


    }

  }

}

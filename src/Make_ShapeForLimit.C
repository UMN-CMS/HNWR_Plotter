#include "canvas_margin.h"
#include "LRSMSignalInfo.h"
#include "SignalSystematics.h"
#include "mylib.h"

bool IsCorrelated(TString syst){

  if(syst.Contains("JetRes")) return true;
  if(syst.Contains("JetEn")) return true;

  return false;

}

void Make_ShapeForLimit(int Year=2016){

  TString str_Year = TString::Itoa(Year,10);

  bool UseCustomRebin = true;

/*
  bool UncorrelateSyst = false;
  TString nuisancePrefix = "";
  if(UncorrelateSyst) nuisancePrefix = "Run"+str_Year+"_";
*/

  TString ShapeVarName = "WRCand_Mass";
  int n_rebin = 40;

  double signal_scale = 0.001; // r value in fb
  signal_scale *= 0.1; // r value will be multiplied by 1/0.1 = 10

  double ScaleLumi = 1.;

  TString filename_prefix = "HNWRAnalyzer_SkimTree_LRSMHighPt_";

  if(Year==2016){
    ScaleLumi *= 1.;
  }
  else if(Year==2017){
    ScaleLumi *=  1.;
  }
  else if(Year==2018){
    ScaleLumi *= 1;
  }
  else if(Year==20162017){
    Year=2016;
    ScaleLumi *= (35918.219+41527.540)/35918.219;
    str_Year = "2016";
  }

  vector<TString> systs = {
    "Central",
    "JetResUp", "JetResDown",
    "JetEnUp", "JetEnDown",
    "MuonRecoSFUp", "MuonRecoSFDown",
    "MuonEnUp", "MuonEnDown",
    "MuonIDSFUp", "MuonIDSFDown",
    "MuonISOSFUp", "MuonISOSFDown",
    "MuonTriggerSFUp", "MuonTriggerSFDown",
    "ElectronRecoSFUp", "ElectronRecoSFDown",
    "ElectronResUp", "ElectronResDown",
    "ElectronEnUp", "ElectronEnDown",
    "ElectronIDSFUp", "ElectronIDSFDown",
    "ElectronTriggerSFUp", "ElectronTriggerSFDown",
    "LSFSFUp", "LSFSFDown",
    "PUUp", "PUDown",
    "ZPtRwUp", "ZPtRwDown",
  };
  if(Year<=2017){
    systs.push_back( "PrefireUp" );
    systs.push_back( "PrefireDown" );
  }

  gStyle->SetOptStat(0);

  TH1::SetDefaultSumw2(true);
  TH1::AddDirectory(kFALSE);

  TString WORKING_DIR = getenv("PLOTTER_WORKING_DIR");
  TString dataset = getenv("CATANVERSION");
  TString ENV_PLOT_PATH = getenv("PLOT_PATH");

  TString base_filepath = WORKING_DIR+"/rootfiles/"+dataset+"/Regions/"+TString::Itoa(Year,10)+"/";
  TString base_plotpath = ENV_PLOT_PATH+"/"+dataset+"/ShapeForLimit/"+TString::Itoa(Year,10)+"/";

  //==== FIXME test, scaling to 137.19
  //ScaleLumi *= 137.19/35.92;
  //base_plotpath = ENV_PLOT_PATH+"/"+dataset+"/ShapeForLimit/ScaledToFullRun2/";

  gSystem->mkdir(base_plotpath,kTRUE);

  LRSMSignalInfo lrsminfo;
  lrsminfo.GetMassMaps();

  //==== bkgds

  map< TString, vector<TString> > map_sample_string_to_list;

  if(Year==2016){

    map_sample_string_to_list["VVV"] = {"VVV"};
    map_sample_string_to_list["VV_incl"] = {"VV"};
    map_sample_string_to_list["ttX"] = {"ttX"};
    map_sample_string_to_list["SingleTop"] = {"SingleTop"};
    map_sample_string_to_list["WJets_MG_HT"] = {"WJets_MG_HT"};

    //map_sample_string_to_list["ZJets_MG_HT_Reweighted"] = {"FromFit_DYJets_MG_HT_Reweighted"};
    //map_sample_string_to_list["EMuMethod"] = {"FromFit_EMuMethod_TTLX_powheg"};

    map_sample_string_to_list["ZJets_MG_HT_Reweighted"] = {"DYJets10to50_MG_Reweighted", "DYJets_MG_HT_Reweighted"};
    map_sample_string_to_list["EMuMethod"] = {"EMuMethod_TTLX_powheg"};

  }
  else if(Year==2017){

    map_sample_string_to_list["VVV"] = {"VVV"};
    map_sample_string_to_list["VV_incl"] = {"VV"};
    map_sample_string_to_list["ttX"] = {"ttX"};
    map_sample_string_to_list["SingleTop"] = {"SingleTop"};
    map_sample_string_to_list["WJets_MG_HT"] = {"WJets_MG_HT"};

    //map_sample_string_to_list["ZJets_MG_HT_Reweighted"] = {"FromFit_DYJets_MG_HT_Reweighted"};
    //map_sample_string_to_list["EMuMethod"] = {"FromFit_EMuMethod_TTLX_powheg"};

    map_sample_string_to_list["ZJets_MG_HT_Reweighted"] = {"DYJets10to50_MG_Reweighted", "DYJets_MG_HT_Reweighted"};
    map_sample_string_to_list["EMuMethod"] = {"EMuMethod_TTLX_powheg"};


  }
  else if(Year==2018){

    map_sample_string_to_list["VVV"] = {"VVV"};
    map_sample_string_to_list["VV_incl"] = {"VV"};
    map_sample_string_to_list["ttX"] = {"ttX"};
    map_sample_string_to_list["SingleTop"] = {"SingleTop"};
    map_sample_string_to_list["WJets_MG_HT"] = {"WJets_MG_HT"};

    //map_sample_string_to_list["ZJets_MG_HT_Reweighted"] = {"FromFit_DYJets_MG_HT_Reweighted"};
    //map_sample_string_to_list["EMuMethod"] = {"FromFit_EMuMethod_TTLX_powheg"};

    map_sample_string_to_list["ZJets_MG_HT_Reweighted"] = {"DYJets10to50_MG_Reweighted", "DYJets_MG_HT_Reweighted"};
    map_sample_string_to_list["EMuMethod"] = {"EMuMethod_TTLX_powheg"};

  }

  vector<TString> regions = {
      "Resolved_SR",
      "Boosted_SR",
  };
  vector<TString> bkgds = {
"VVV", "VV_incl", "ttX", "SingleTop", "WJets_MG_HT", "ZJets_MG_HT_Reweighted", "EMuMethod"
  };

  vector<TString> channels = {
    "EE",
    "MuMu",
  };
  vector< TString > Suffixs = {
    "HNWR_SingleElectron",
    "HNWR_SingleMuon",
  };

  bool SamplePrinted = false;

  for(unsigned int it_region=0; it_region<regions.size(); it_region++){

    TString region = regions.at(it_region);

    cout << "@@@@ region = " << region << endl;

    for(unsigned int it_channel=0; it_channel<channels.size(); it_channel++){

      TString channel = channels.at(it_channel);
      TString Suffix = Suffixs.at(it_channel);

      cout << "@@@@   channel = " << channel << endl;

      TString dirname = Suffix+"_"+region;
      TString histname = ShapeVarName+"_"+dirname;

      TString PD = "SingleElectron";
      if(channel=="MuMu") PD = "SingleMuon";

      TFile *out_bkgd = new TFile(base_plotpath+"/"+channel+"_"+region+"_Bkgd.root","RECREATE");
      TFile *out_sig = new TFile(base_plotpath+"/"+channel+"_"+region+"_Signal.root","RECREATE");

      //==== DATA

      TFile *file_DATA = new TFile(base_filepath+"/"+filename_prefix+"data_"+PD+".root");
      TDirectory *dir_DATA = (TDirectory *)file_DATA->Get(dirname);
      TH1D *hist_DATA = (TH1D *)dir_DATA->Get(histname);
      hist_DATA->SetName("data_obs");

      if(UseCustomRebin) hist_DATA = RebinWRMass(hist_DATA, Suffix+"_"+region, Year);
      else               hist_DATA->Rebin(n_rebin);

      //==== temporary lumi scaling; scale content, sqrt() sqruare stat
      for(int ibin=1;ibin<=hist_DATA->GetXaxis()->GetNbins();ibin++){
        hist_DATA->SetBinContent(ibin, hist_DATA->GetBinContent(ibin)*ScaleLumi);
        hist_DATA->SetBinError(ibin, hist_DATA->GetBinError(ibin)*sqrt(ScaleLumi));
      } 

      out_bkgd->cd();
      hist_DATA->Write();

      //==== sample

      //=== big systematic loop starts here
      for(unsigned it_syst=0; it_syst<systs.size(); it_syst++){

        TString syst = systs.at(it_syst);
        TString shapehistname_suffix = "";

        //==== for correlated 
        TString nuisancePrefix = "";
        //==== for uncorrelated
        if( !IsCorrelated(syst) ) nuisancePrefix = "Run"+str_Year+"_";

        cout << "@@@@     syst = " << syst << endl;

        if(syst=="Central"){
          dirname = Suffix+"_"+region;
          histname = ShapeVarName+"_"+dirname;
          shapehistname_suffix = "";
        }
        else{
          dirname = "Syst_"+syst+"_"+Suffix+"_"+region;
          histname = ShapeVarName+"_"+dirname;
          shapehistname_suffix = "_"+nuisancePrefix+syst;
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

          //cout << "@@@@     sample = " << sample << endl;

          TFile *file_sample = new TFile(base_filepath+"/"+filename);
          TDirectory *dir_sample = (TDirectory *)file_sample->Get(dirname);
          //cout << "file = " << base_filepath+"/"+filename << endl;
          //cout << "dirname = " << dirname << endl;
          if(dir_sample){
            TH1D *hist_bkgd = (TH1D *)dir_sample->Get(histname);

            //==== DY Norm
            if(sample.Contains("DYJets_")){
              hist_bkgd->Scale( GetDYNormSF(Year, PD+"_"+region) );
            }

            if(hist_bkgd){

              if(sample.Contains("DYJets") || sample.Contains("EMuMethod")){
                //==== these are already rebinned
                if(UseCustomRebin) hist_bkgd = RebinWRMass(hist_bkgd, Suffix+"_"+region, Year);
              }
              else{
                if(UseCustomRebin) hist_bkgd = RebinWRMass(hist_bkgd, Suffix+"_"+region, Year);
                else               hist_bkgd->Rebin(n_rebin);
              }

              //==== remove negative bins
              for(int ibin=1; ibin<=hist_bkgd->GetXaxis()->GetNbins(); ibin++){
                if(hist_bkgd->GetBinContent(ibin) < 0.){
                  //hist_bkgd->SetBinContent(ibin, 0.);
                }

                hist_bkgd->SetBinContent(ibin, hist_bkgd->GetBinContent(ibin)*ScaleLumi);
                hist_bkgd->SetBinError(ibin, hist_bkgd->GetBinError(ibin)*sqrt(ScaleLumi));

              }

              //==== If EMu-method
              if(sample.Contains("EMuMethod") && syst=="Central"){

                cout << "@@@@ EMu : " << region << "\t" << channel << endl;
                hist_bkgd->SetName(sample+shapehistname_suffix);

                double EMuSyst = 0.20;
                if( region.Contains("Boosted") ) EMuSyst = 0.30;

                TH1D *hist_bkgdUp = GetScaleUpDown(hist_bkgd,+1.*EMuSyst);
                hist_bkgdUp->SetName(sample+"_"+nuisancePrefix+"EMuSystUp");
                TH1D *hist_bkgdDown = GetScaleUpDown(hist_bkgd,-1.*EMuSyst);
                hist_bkgdDown->SetName(sample+"_"+nuisancePrefix+"EMuSystDown");

                TH1D *hist_bkgd_StatUp = GetStatUpDown(hist_bkgd,+1);
                hist_bkgd_StatUp->SetName(sample+"_"+nuisancePrefix+"StatUp");
                TH1D *hist_bkgd_StatDown = GetStatUpDown(hist_bkgd,-1);
                hist_bkgd_StatDown->SetName(sample+"_"+nuisancePrefix+"StatDown");

                out_bkgd->cd();

                hist_bkgd->Write();
                hist_bkgdUp->Write();
                hist_bkgdDown->Write();
                hist_bkgd_StatUp->Write();
                hist_bkgd_StatDown->Write();

              }
              else{

                if(syst=="Central"){

                  TH1D *hist_bkgdstatup = GetStatUpDown(hist_bkgd,+1);
                  hist_bkgdstatup->SetName(sample+"_"+nuisancePrefix+"StatUp");
                  TH1D *hist_bkgdstatdown = GetStatUpDown(hist_bkgd,-1);
                  hist_bkgdstatdown->SetName(sample+"_"+nuisancePrefix+"StatDown");

                  out_bkgd->cd();
                  hist_bkgdstatup->Write();
                  hist_bkgdstatdown->Write();

                  hist_bkgd->SetName(sample+shapehistname_suffix);

                }
                else{

                  hist_bkgd->SetName(sample+shapehistname_suffix);

                }

                out_bkgd->cd();
                hist_bkgd->Write();

              }

            }

          }
          else{
            //==== If no histogram
            //==== e.g., SingleTop_sch_Lep has one entry, and with JetEnUp, that event is gone, so no histogram is filled
            //==== make a empty histogram; bin info from hist_DATA
            TH1D *hist_empty = (TH1D *)hist_DATA->Clone();

            if(syst=="Central"){
              hist_empty->SetName(sample+shapehistname_suffix);
            }
            else{
              hist_empty->SetName(sample+shapehistname_suffix);
            }

            EmptyHistogram(hist_empty);
            out_bkgd->cd();
            hist_empty->Write();
          }

          file_sample->Close();
          delete file_sample;

        } // END Loop over bkgd samples


        //==== Signals

        for(map< double, vector<double> >::iterator it=lrsminfo.maps_WR_to_N.begin(); it!=lrsminfo.maps_WR_to_N.end(); it++){

          double m_WR = it->first;
          vector<double> this_m_Ns = it->second;

          for(int it_N=0; it_N<this_m_Ns.size(); it_N++){

            double m_N = this_m_Ns.at(it_N);

            TString this_filename = "HNWRAnalyzer_WRtoNLtoLLJJ_WR"+TString::Itoa(m_WR,10)+"_N"+TString::Itoa(m_N,10)+".root";


            TString temp_base_filepath = WORKING_DIR+"/rootfiles/"+dataset+"/Regions/"+str_Year+"/";
            TFile *file_sig = new TFile(temp_base_filepath+"/Signal/"+this_filename);
            TDirectory *dir_sig = (TDirectory *)file_sig->Get(dirname);

/*
            //==== TODO DEBUGGING
            TFile *file_sig = new TFile("/data6/Users/jskim/SKFlatOutput/Run2Legacy_v4/HNWRAnalyzer/"+str_Year+"/RunSyst__Signal__RunXsecSyst__RunNewPDF__NNPDF23_lo_as_0130_qed__/"+this_filename);
            TDirectory *dir_sig = (TDirectory *)file_sig->Get(dirname);
*/
            if(dir_sig){

              TH1D *hist_sig = (TH1D *)dir_sig->Get(histname);

              if(hist_sig){
                if(UseCustomRebin) hist_sig = RebinWRMass(hist_sig, Suffix+"_"+region, Year);
                else               hist_sig->Rebin(n_rebin);

                if(syst=="Central"){
                  hist_sig->SetName("WR"+TString::Itoa(m_WR,10)+"_N"+TString::Itoa(m_N,10)+shapehistname_suffix);
                }
                else{
                  hist_sig->SetName("WR"+TString::Itoa(m_WR,10)+"_N"+TString::Itoa(m_N,10)+shapehistname_suffix);
                }

                //==== remove negative bins
                for(int ibin=1; ibin<=hist_sig->GetXaxis()->GetNbins(); ibin++){
                  if(hist_sig->GetBinContent(ibin) < 0.){
                    //hist_sig->SetBinContent(ibin, 0.);
                  }
                  //==== Scale lumi
                  hist_sig->SetBinContent(ibin, hist_sig->GetBinContent(ibin)*ScaleLumi);
                  hist_sig->SetBinError(ibin, hist_sig->GetBinError(ibin)*sqrt(ScaleLumi));

                  //==== Scale signal
                  hist_sig->SetBinContent(ibin, hist_sig->GetBinContent(ibin)*signal_scale);
                  hist_sig->SetBinError(ibin, hist_sig->GetBinError(ibin)*signal_scale);

                }

                out_sig->cd();
                hist_sig->Write();

                if(syst=="Central"){

                  //==== Stat


                  TH1D *hist_sigstatup = GetStatUpDown(hist_sig,+1);
                  hist_sigstatup->SetName("WR"+TString::Itoa(m_WR,10)+"_N"+TString::Itoa(m_N,10)+"_StatUp");
                  TH1D *hist_sigstatdown = GetStatUpDown(hist_sig,-1);
                  hist_sigstatdown->SetName("WR"+TString::Itoa(m_WR,10)+"_N"+TString::Itoa(m_N,10)+"_StatDown");

                  out_sig->cd();
                  hist_sigstatup->Write();
                  hist_sigstatdown->Write();


                  //==== xsec

                  SignalSystematics m;
                  m.DataYear = Year;
                  m.file = file_sig;
                  //m.DoDebug = true;

                  TH1D *hist_sig_SignalFlavour = (TH1D *)file_sig->Get("SignalFlavour");

                  m.ChannelFrac = 1./hist_sig_SignalFlavour->GetEntries();
                  if(channel=="EE") m.ChannelFrac *= hist_sig_SignalFlavour->GetBinContent(2);
                  else if(channel=="MuMu") m.ChannelFrac *= hist_sig_SignalFlavour->GetBinContent(3);
                  else{
                    cout << "WTF?? channel = " << channel << endl;
                    return;
                  }

                  m.region = dirname;
                  m.UseCustomRebin = UseCustomRebin;
                  m.n_rebin = n_rebin;
                  m.hist_Central = hist_sig;
                  //m.isReplica = true;//TODO DEBUGGING
                  m.Run();

                  m.hist_ScaleUp->SetName("WR"+TString::Itoa(m_WR,10)+"_N"+TString::Itoa(m_N,10)+"_ScaleUp");
                  m.hist_ScaleDn->SetName("WR"+TString::Itoa(m_WR,10)+"_N"+TString::Itoa(m_N,10)+"_ScaleDown");
                  m.hist_ScaleIntegral->SetName("WR"+TString::Itoa(m_WR,10)+"_N"+TString::Itoa(m_N,10)+"_ScaleIntegralSyst");
                  m.hist_PDFErrorUp->SetName("WR"+TString::Itoa(m_WR,10)+"_N"+TString::Itoa(m_N,10)+"_PDFErrorUp");
                  m.hist_PDFErrorDn->SetName("WR"+TString::Itoa(m_WR,10)+"_N"+TString::Itoa(m_N,10)+"_PDFErrorDown");
                  m.hist_AlphaSUp->SetName("WR"+TString::Itoa(m_WR,10)+"_N"+TString::Itoa(m_N,10)+"_AlphaSUp");
                  m.hist_AlphaSDn->SetName("WR"+TString::Itoa(m_WR,10)+"_N"+TString::Itoa(m_N,10)+"_AlphaSDown");

                  out_sig->cd();

                  m.hist_ScaleUp->Write();
                  m.hist_ScaleDn->Write();
                  m.hist_ScaleIntegral->Write();

                  m.hist_PDFErrorUp->Write();
                  m.hist_PDFErrorDn->Write();

                  m.hist_AlphaSUp->Write();
                  m.hist_AlphaSDn->Write();


                } // END if central


              }

            }

            file_sig->Close();
            delete file_sig;

          } // END Loop over N

        } // END Loop WR

      } // END Loop Systematic source

      out_bkgd->Close();
      out_sig->Close();

      file_DATA->Close();
      delete file_DATA;


    } // END Loop channel

  }

}


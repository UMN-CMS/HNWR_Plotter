#include "canvas_margin.h"
#include "LRSMSignalInfo.h"
#include "SignalSystematics.h"
#include "mylib.h"

void Make_EMuShape(int Year=2016, int int_ch=0){

  TString channel = "EE";
  if(int_ch==1) channel = "MuMu";

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
    "JetMassUp", "JetMassDown",
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
    "ZPtRwQCDScaleUp", "ZPtRwQCDScaleDown",
    "ZPtRwQCDPDFErrorUp", "ZPtRwQCDPDFErrorDown",
    "ZPtRwQCDPDFAlphaSUp", "ZPtRwQCDPDFAlphaSDown",
    "ZPtRwEW1Up", "ZPtRwEW1Down",
    "ZPtRwEW2Up", "ZPtRwEW2Down",
    "ZPtRwEW3Up", "ZPtRwEW3Down",
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

  LRSMSignalInfo lrsminfo;
  lrsminfo.GetMassMaps();

  gSystem->mkdir(base_plotpath,kTRUE);

  //==== bkgds

  map< TString, vector<TString> > map_sample_string_to_list;

  map_sample_string_to_list["Others"] = {"Others"};
  map_sample_string_to_list["NonPrompt"] = {"NonPrompt"};
  map_sample_string_to_list["TT_TW"] = {"TT_TW"};
  map_sample_string_to_list["DY"] = {"DYJets_MG_HT_ReweightedQCDErrorEWCorr_Reshaped"};
  vector<TString> bkgds = {
    "Others",
    "NonPrompt",
    "TT_TW",
    "DY",
  };

  vector<TString> regions = {
    "HNWR_EMu_Resolved_SR",
    "HNWR_SingleElectron_EMu_Boosted_CR",
    "HNWR_SingleMuon_EMu_Boosted_CR",
  };

  bool SamplePrinted = false;

  for(unsigned int it_region=0; it_region<regions.size(); it_region++){

    TString region = regions.at(it_region);

    cout << "@@@@   region = " << region << endl;

    TString dirname = region;
    TString histname = ShapeVarName+"_"+dirname;

    TString PD = "SingleMuon";
    if(region=="HNWR_SingleElectron_EMu_Boosted_CR") PD = "SingleElectron";

    TFile *out_bkgd = new TFile(base_plotpath+"/"+channel+"_"+region+".root","RECREATE");
    //TFile *out_sig = new TFile(base_plotpath+"/"+region+"_Signal.root","RECREATE");

    //==== DATA

    TFile *file_DATA = new TFile(base_filepath+"/"+filename_prefix+"data_"+PD+".root");
    TDirectory *dir_DATA = (TDirectory *)file_DATA->Get(dirname);
    TH1D *hist_DATA = (TH1D *)dir_DATA->Get(histname);
    hist_DATA->SetName("data_obs");

    if(UseCustomRebin) hist_DATA = RebinWRMass(hist_DATA, region, Year, true);
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
        dirname = region;
        histname = ShapeVarName+"_"+dirname;
        shapehistname_suffix = "";
      }
      else{
        dirname = "Syst_"+syst+"_"+region;
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

        cout << "@@@@     sample = " << sample << endl;

        TFile *file_sample = NULL;
        if(sample=="EMuMethod_TTLX_powheg"){
          file_sample = new TFile(ENV_PLOT_PATH+"/"+dataset+"/EMuRatio/"+TString::Itoa(Year,10)+"/EMuDataSubtracted_"+region+".root");
        }
        else{
          file_sample = new TFile(base_filepath+"/"+filename);
        }
        TDirectory *dir_sample = (TDirectory *)file_sample->Get(dirname);
        //cout << "file = " << base_filepath+"/"+filename << endl;
        //cout << "dirname = " << dirname << endl;
        if(dir_sample){
          TH1D *hist_bkgd = (TH1D *)dir_sample->Get(histname);

          if(hist_bkgd){

            if(sample.Contains("DYJets") || sample.Contains("EMuMethod")){
              //==== these are already rebinned
              if(UseCustomRebin) hist_bkgd = RebinWRMass(hist_bkgd, region, Year, true);
            }
            else{
              if(UseCustomRebin) hist_bkgd = RebinWRMass(hist_bkgd, region, Year, true);
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

              cout << "@@@@ EMu : " << region << endl;
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

                //==== TODO test emu shape unct
                if(sample=="TTLX_powheg"){
                  TString shapefilename = "ResolvedShapeUnct";
                  if(region=="HNWR_EMu_Resolved_SR") shapefilename = "ResolvedShapeUnct";
                  else if(region=="HNWR_SingleElectron_EMu_Boosted_CR") shapefilename = "BoostedMuJetShapeUnct";
                  else if(region=="HNWR_SingleMuon_EMu_Boosted_CR") shapefilename = "BoostedEJetShapeUnct";
                  else{
                    cout << "WTF?? region = " << region << endl;
                    exit(EXIT_FAILURE);
                  }
                  TFile *f_ttchape = new TFile(ENV_PLOT_PATH+"/"+dataset+"/TTBarShapes/"+TString::Itoa(Year,10)+"/shapes_"+shapefilename+".root");
                  TH1D *f_ttchape_Up = (TH1D *)f_ttchape->Get(shapefilename+"Up");
                  TH1D *f_ttchape_Down = (TH1D *)f_ttchape->Get(shapefilename+"Down");

                  TH1D *hist_tt_ShapeUp = (TH1D *)hist_bkgd->Clone();
                  hist_tt_ShapeUp->SetName(sample+"_Run"+str_Year+"_"+shapefilename+"Up");
                  TH1D *hist_tt_ShapeDown = (TH1D *)hist_bkgd->Clone();
                  hist_tt_ShapeDown->SetName(sample+"_Run"+str_Year+"_"+shapefilename+"Down");
                  for(int c=1; c<=hist_tt_ShapeUp->GetXaxis()->GetNbins(); c++){
                    double x_l_1 = hist_tt_ShapeUp->GetXaxis()->GetBinLowEdge(c);
                    double x_r_1 = hist_tt_ShapeUp->GetXaxis()->GetBinUpEdge(c);
                    double x_l_2 = f_ttchape_Up->GetXaxis()->GetBinLowEdge(c);
                    double x_r_2 = f_ttchape_Up->GetXaxis()->GetBinUpEdge(c);
                    hist_tt_ShapeUp->SetBinContent(c, hist_tt_ShapeUp->GetBinContent(c) * f_ttchape_Up->GetBinContent(c));
                    hist_tt_ShapeDown->SetBinContent(c, hist_tt_ShapeDown->GetBinContent(c) * f_ttchape_Down->GetBinContent(c));
                  }
                  hist_tt_ShapeUp->Scale( hist_bkgd->Integral() / hist_tt_ShapeUp->Integral() );
                  hist_tt_ShapeDown->Scale( hist_bkgd->Integral() / hist_tt_ShapeDown->Integral() );

                  out_bkgd->cd();
                  hist_tt_ShapeUp->Write();
                  hist_tt_ShapeDown->Write();
                }
                else if(sample.Contains("DYJets_")){

                  //==== making DYReshapeSyst shapes

                  TString ResolvedORBoosted = "Resolved";
                  if(region.Contains("Boosted")){
                    ResolvedORBoosted = "Boosted";
                  }

                  TH1D *hist_DYReshapeSystUp = (TH1D *)file_sample->Get("Syst_DYReshapeSystUp_"+dirname+"/WRCand_Mass_Syst_DYReshapeSystUp_"+dirname);
                  hist_DYReshapeSystUp = RebinWRMass(hist_DYReshapeSystUp, region, Year, true);
                  //hist_DYReshapeSystUp->Scale( GetDYNormSF(Year, PD+"_"+region) );

                  TH1D *hist_DYReshapeSystDown = (TH1D *)file_sample->Get("Syst_DYReshapeSystDown_"+dirname+"/WRCand_Mass_Syst_DYReshapeSystDown_"+dirname);
                  hist_DYReshapeSystDown = RebinWRMass(hist_DYReshapeSystDown, region, Year, true);
                  //hist_DYReshapeSystDown->Scale( GetDYNormSF(Year, PD+"_"+region) );

                  for(int z=1; z<=hist_bkgd->GetXaxis()->GetNbins(); z++){

                    double this_nominal = hist_bkgd->GetBinContent(z);
                    double this_Up = hist_DYReshapeSystUp->GetBinContent(z);
                    double this_Down = hist_DYReshapeSystDown->GetBinContent(z);

                    TH1D *hist_DYShapeUp =   (TH1D *)hist_bkgd->Clone(sample+"_"+ResolvedORBoosted+"DYReshapeSystBin"+TString::Itoa(z-1,10)+"Up");
                    TH1D *hist_DYShapeDown = (TH1D *)hist_bkgd->Clone(sample+"_"+ResolvedORBoosted+"DYReshapeSystBin"+TString::Itoa(z-1,10)+"Down");

                    hist_DYShapeUp->SetBinContent(z, this_Up);
                    hist_DYShapeDown->SetBinContent(z, this_Down);

                    hist_DYShapeUp->Write();
                    hist_DYShapeDown->Write();

                  }

                  out_bkgd->cd();
                } // END if DYJets


              } // END if Central
              else{

                if(syst.Contains("DYReshapeEEMM")){
                  hist_bkgd->SetName(sample+"_"+channel+shapehistname_suffix);
                }
                else{
                  hist_bkgd->SetName(sample+shapehistname_suffix);
                }


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

            if(syst.Contains("DYReshapeEEMM")){
              hist_empty->SetName(sample+"_"+channel+shapehistname_suffix);
            }
            else{
              hist_empty->SetName(sample+shapehistname_suffix);
            }

          }

          EmptyHistogram(hist_empty);
          out_bkgd->cd();
          hist_empty->Write();
        }

        file_sample->Close();
        delete file_sample;

      } // END Loop over bkgd samples

      if(0){

      //==== Signal

      for(map< double, vector<double> >::iterator it=lrsminfo.maps_WR_to_N.begin(); it!=lrsminfo.maps_WR_to_N.end(); it++){

        double m_WR = it->first;
        vector<double> this_m_Ns = it->second;

        for(int it_N=0; it_N<this_m_Ns.size(); it_N++){

          double m_N = this_m_Ns.at(it_N);

          TString this_filename = "HNWRAnalyzer_WRtoNLtoLLJJ_WR"+TString::Itoa(m_WR,10)+"_N"+TString::Itoa(m_N,10)+".root";


          TString temp_base_filepath = WORKING_DIR+"/rootfiles/"+dataset+"/Regions/"+str_Year+"/";
          TFile *file_sig = new TFile(temp_base_filepath+"/Signal_"+channel+"/"+this_filename);
          TDirectory *dir_sig = (TDirectory *)file_sig->Get(dirname);

          //==== forcing no signal in emu CRs
          dir_sig = NULL;
          if(dir_sig){

            TH1D *hist_sig = (TH1D *)dir_sig->Get(histname);

            if(hist_sig){
              if(UseCustomRebin) hist_sig = RebinWRMass(hist_sig, region, Year, true);
              else               hist_sig->Rebin(n_rebin);

              //==== CR contains many negative or zero bins
              for(int ix=1; ix<=hist_sig->GetXaxis()->GetNbins(); ix++){
                hist_sig->SetBinContent(ix, max(0.000001/signal_scale, hist_sig->GetBinContent(ix)) );
              }

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

              out_bkgd->cd();
              hist_sig->Write();

              if(syst=="Central"){

                //==== Stat


                TH1D *hist_sigstatup = GetStatUpDown(hist_sig,+1);
                hist_sigstatup->SetName("WR"+TString::Itoa(m_WR,10)+"_N"+TString::Itoa(m_N,10)+"_StatUp");
                TH1D *hist_sigstatdown = GetStatUpDown(hist_sig,-1);
                hist_sigstatdown->SetName("WR"+TString::Itoa(m_WR,10)+"_N"+TString::Itoa(m_N,10)+"_StatDown");

                out_bkgd->cd();
                hist_sigstatup->Write();
                hist_sigstatdown->Write();


                //==== xsec

                SignalSystematics m;
                m.DataYear = Year;
                m.file = file_sig;
                //m.DoDebug = true;

                TH1D *hist_sig_SignalFlavour = (TH1D *)file_sig->Get("SignalFlavour");

/*
                m.ChannelFrac = 1./hist_sig_SignalFlavour->GetEntries();
                if(channel=="EE") m.ChannelFrac *= hist_sig_SignalFlavour->GetBinContent(2);
                else if(channel=="MuMu") m.ChannelFrac *= hist_sig_SignalFlavour->GetBinContent(3);
                else{
                  cout << "WTF?? channel = " << channel << endl;
                  return;
                }
*/
                m.ChannelFrac = 1.;

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

                out_bkgd->cd();

                m.hist_ScaleUp->Write();
                m.hist_ScaleDn->Write();
                m.hist_ScaleIntegral->Write();

/*
                //==== DEBUG TODO CHECK
                TH1D *tmp_hist_PDFErrorUp = (TH1D *)hist_sig->Clone("WR"+TString::Itoa(m_WR,10)+"_N"+TString::Itoa(m_N,10)+"_PDFErrorUp");
                TH1D *tmp_hist_PDFErrorDn = (TH1D *)hist_sig->Clone("WR"+TString::Itoa(m_WR,10)+"_N"+TString::Itoa(m_N,10)+"_PDFErrorDown");
                double tmp_sig_pdferr = 0.10;
                for(int z=1; z<=hist_sig->GetXaxis()->GetNbins(); z++){
                  tmp_hist_PDFErrorUp->SetBinContent(z, hist_sig->GetBinContent(z) * (1.+tmp_sig_pdferr));
                  tmp_hist_PDFErrorDn->SetBinContent(z, hist_sig->GetBinContent(z) * (1.-tmp_sig_pdferr));
                }
                tmp_hist_PDFErrorUp->Write();
                tmp_hist_PDFErrorDn->Write();
*/
                m.hist_PDFErrorUp->Write();
                m.hist_PDFErrorDn->Write();

                m.hist_AlphaSUp->Write();
                m.hist_AlphaSDn->Write();


              } // END if central


            }

          }
          else{

            //==== write a empty histogram
            TH1D *hist_Sig_Central = new TH1D("WR"+TString::Itoa(m_WR,10)+"_N"+TString::Itoa(m_N,10)+shapehistname_suffix, "", 800, 0., 8000.);
            if(UseCustomRebin) hist_Sig_Central = RebinWRMass(hist_Sig_Central, region, Year, true);
            else               hist_Sig_Central->Rebin(n_rebin);
            for(int ix=1; ix<=hist_Sig_Central->GetXaxis()->GetNbins(); ix++){
              hist_Sig_Central->SetBinContent(ix,0.000001);
            }
            out_bkgd->cd();
            hist_Sig_Central->Write();

            if(syst=="Central"){

              out_bkgd->cd();

              hist_Sig_Central->SetName("WR"+TString::Itoa(m_WR,10)+"_N"+TString::Itoa(m_N,10)+"_ScaleUp");
              hist_Sig_Central->Write();
              hist_Sig_Central->SetName("WR"+TString::Itoa(m_WR,10)+"_N"+TString::Itoa(m_N,10)+"_ScaleDown");
              hist_Sig_Central->Write();

              TH1D *hist_Sig_ScaleIntegral = new TH1D("WR"+TString::Itoa(m_WR,10)+"_N"+TString::Itoa(m_N,10)+"_ScaleIntegralSyst", "", 1, 0., 1.);
              hist_Sig_ScaleIntegral->SetBinContent(1, 1);

              hist_Sig_Central->SetName("WR"+TString::Itoa(m_WR,10)+"_N"+TString::Itoa(m_N,10)+"_PDFErrorUp");
              hist_Sig_Central->Write();
              hist_Sig_Central->SetName("WR"+TString::Itoa(m_WR,10)+"_N"+TString::Itoa(m_N,10)+"_PDFErrorDown");
              hist_Sig_Central->Write();
              hist_Sig_Central->SetName("WR"+TString::Itoa(m_WR,10)+"_N"+TString::Itoa(m_N,10)+"_AlphaSUp");
              hist_Sig_Central->Write();
              hist_Sig_Central->SetName("WR"+TString::Itoa(m_WR,10)+"_N"+TString::Itoa(m_N,10)+"_AlphaSDown");
              hist_Sig_Central->Write();
            }

          }

          file_sig->Close();
          delete file_sig;

        } // END Loop over N

      } // END Loop WR

      } // END comment out signal

    } // END Loop Systematic source

    out_bkgd->Close();
    //out_sig->Close();

    file_DATA->Close();
    delete file_DATA;


  } // END Loop channel

}


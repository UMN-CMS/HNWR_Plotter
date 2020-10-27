#include "canvas_margin.h"
#include "mylib.h"

void Get_DYReshapeRatio(int int_Year=2016){

  TString MeasureRegion = "DYCR";
  TString filename_prefix = "HNWRAnalyzer_SkimTree_LRSMHighPt_";

  //gErrorIgnoreLevel = kFatal;

  setTDRStyle();

  TString Year = "2016";
  double LumiError = 0.025;
  if(int_Year==2016){
    Year = "2016";
    LumiError = 0.025;
  }
  else if(int_Year==2017){
    Year = "2017";
    LumiError = 0.023;
  }
  else if(int_Year==2018){
    Year = "2018";
    LumiError = 0.025;
  }
  else{
    cout << "Wrong int_Year : " << int_Year << endl;
    exit(EXIT_FAILURE);
  }

  vector<TString> Systs = {
  "JetRes",
  "JetEn",
  "MuonRecoSF",
  "MuonEn",
  "MuonIDSF",
  "MuonISOSF",
  "MuonTriggerSF",
  "ElectronRecoSF",
  "ElectronRes",
  "ElectronEn",
  "ElectronIDSF",
  "ElectronTriggerSF",
  "LSFSF",
  "PU",
  "ZPtRw",
  "Prefire",
  };
  //Systs.clear();

  gStyle->SetOptStat(0);

  TH1::SetDefaultSumw2(true);
  TH1::AddDirectory(kFALSE);

  TString WORKING_DIR = getenv("PLOTTER_WORKING_DIR");
  TString dataset = getenv("CATANVERSION");
  TString ENV_PLOT_PATH = getenv("PLOT_PATH");

  TString base_filepath = WORKING_DIR+"/rootfiles/"+dataset+"/Regions/"+Year+"/";
  TString base_plotpath = ENV_PLOT_PATH+"/"+dataset+"/DYReshapeRatio/"+Year+"/";
  system("mkdir -p "+base_plotpath);

  TFile *file_out = new TFile(base_plotpath+"/shapes.root","RECREATE");

  //==== MCs

  vector<TString> bkgds = {
    "TTLX_powheg", "Others",
  };
  TString samplename_DY50 = "DYJets_MG_HT_Reweighted";

  TFile *file_DY50 = new TFile(base_filepath+"/"+filename_prefix+samplename_DY50+".root");
  TString var = "WRCand_Mass";

  for(int it_region=0; it_region<2; it_region++){

    TString region = "";
    if(it_region==0) region = "Resolved";
    else region = "Boosted";

    cout << "---------------------------------------" << endl;
    cout << "Region = " << region << endl;

    //=====================
    //==== 1) ee+mm first
    //=====================

    int it_fl_i = 0;
    int it_fl_f = 1;

    TH1D *hist_DATA_AllCh = NULL;
    TH1D *hist_DY50_AllCh = NULL;

    for(int it_fl=it_fl_i; it_fl<=it_fl_f; it_fl++){

      TString leptonFlavour = "Electron";
      if(it_fl==1){
        leptonFlavour = "Muon";
      }

      TString dirname = "HNWR_Single"+leptonFlavour+"_"+region+"_"+MeasureRegion;

      TFile *file_DATA = new TFile(base_filepath+"/"+filename_prefix+"data_Single"+leptonFlavour+".root");
      TH1D *hist_DATA = (TH1D *)file_DATA->Get(dirname+"/"+var+"_"+dirname);

      for(unsigned int it_bkgd=0; it_bkgd<bkgds.size(); it_bkgd++){
        TFile *file_bkgd = new TFile(base_filepath+"/"+filename_prefix+bkgds.at(it_bkgd)+".root");
        TH1D *hist_bkgd = (TH1D *)file_bkgd->Get(dirname+"/"+var+"_"+dirname);
        if(!hist_bkgd) continue;

        if(hist_bkgd->GetBinContent(1)!=hist_bkgd->GetBinContent(1)){
          cout << base_filepath+"/"+filename_prefix+bkgds.at(it_bkgd)+".root" << endl;
        }

        hist_DATA->Add(hist_bkgd, -1.);
      }

      TH1D *hist_DY50 = (TH1D *)file_DY50->Get(dirname+"/"+var+"_"+dirname);
      //==== Scale DY
      double this_DYNormSF = GetDYNormSF(int_Year, leptonFlavour+"_"+region);
      hist_DY50->Scale(this_DYNormSF);

      //==== Rebin
      hist_DATA = RebinWRMass(hist_DATA, region, int_Year);
      hist_DY50 = RebinWRMass(hist_DY50, region, int_Year);

      if(hist_DATA_AllCh){
        hist_DATA_AllCh->Add(hist_DATA);
        hist_DY50_AllCh->Add(hist_DY50);
      }
      else{
        hist_DATA_AllCh = (TH1D *)hist_DATA->Clone();
        hist_DY50_AllCh = (TH1D *)hist_DY50->Clone();
      }
    } // END ch loop



    //==== shape
    for(unsigned it_Syst=0; it_Syst<Systs.size(); it_Syst++){

      TString Syst = Systs.at(it_Syst);

      TH1D *hist_Up_AllCh = NULL;
      TH1D *hist_Down_AllCh = NULL;
      for(int it_fl=it_fl_i; it_fl<=it_fl_f; it_fl++){

        TString leptonFlavour = "Electron";
        if(it_fl==1){
          leptonFlavour = "Muon";
        }

        TString dirname = "HNWR_Single"+leptonFlavour+"_"+region+"_"+MeasureRegion;
        TH1D *hist_DY50 = (TH1D *)file_DY50->Get(dirname+"/"+var+"_"+dirname);

        TDirectory *dir_Up = (TDirectory *)file_DY50->Get("Syst_"+Syst+"Up_"+dirname);
        TH1D *hist_Up = NULL;
        if(dir_Up){
          hist_Up = (TH1D *)dir_Up->Get( var+"_Syst_"+Syst+"Up_"+dirname );
        }
        else{
          hist_Up = (TH1D *)hist_DY50->Clone();
        }

        TDirectory *dir_Down = (TDirectory *)file_DY50->Get("Syst_"+Syst+"Down_"+dirname);
        TH1D *hist_Down = NULL;
        if(dir_Down){
          hist_Down = (TH1D *)dir_Down->Get( var+"_Syst_"+Syst+"Down_"+dirname );
        }
        else{
          hist_Down = (TH1D *)hist_DY50->Clone();
        }

        //==== Rebin
        hist_Up = RebinWRMass(hist_Up, region, int_Year);
        hist_Down = RebinWRMass(hist_Down, region, int_Year);

        double this_DYNormSF = GetDYNormSF(int_Year, leptonFlavour+"_"+region);

        hist_Up->Scale(this_DYNormSF);
        hist_Down->Scale(this_DYNormSF);

        if(hist_Up_AllCh){
          hist_Up_AllCh->Add(hist_Up);
          hist_Down_AllCh->Add(hist_Down);
        }
        else{
          hist_Up_AllCh = (TH1D *)hist_Up->Clone();
          hist_Down_AllCh = (TH1D *)hist_Down->Clone();
        }

      } // END ch loop

      //==== bin-by-bin propagation
      for(int ix=1; ix<=hist_DY50_AllCh->GetXaxis()->GetNbins(); ix++){
        double x_l = hist_DY50_AllCh->GetXaxis()->GetBinLowEdge(ix);
        double x_r = hist_DY50_AllCh->GetXaxis()->GetBinUpEdge(ix);
        double y_Nominal = hist_DY50_AllCh->GetBinContent(ix);
        double y_CurrentErr = hist_DY50_AllCh->GetBinError(ix);

        double y_Up = hist_Up_AllCh->GetBinContent(ix);
        double y_Down = hist_Down_AllCh->GetBinContent(ix);

        double diff_Up = fabs(y_Up-y_Nominal);
        double diff_Down = fabs(y_Down-y_Nominal);
        double this_syst = sqrt( (diff_Up*diff_Up + diff_Down*diff_Down) / 2. );

        double new_err = sqrt( y_CurrentErr*y_CurrentErr + this_syst*this_syst );
        hist_DY50_AllCh->SetBinError(ix, new_err);

      }

    } // END lepton channel loop

    //==========================
    //==== 2) ee, only central
    //==========================

    it_fl_i = 0;
    it_fl_f = 0;

    TH1D *hist_DATA_ee = NULL;
    TH1D *hist_DY50_ee = NULL;

    for(int it_fl=it_fl_i; it_fl<=it_fl_f; it_fl++){

      TString leptonFlavour = "Electron";
      if(it_fl==1){
        leptonFlavour = "Muon";
      }

      TString dirname = "HNWR_Single"+leptonFlavour+"_"+region+"_"+MeasureRegion;

      TFile *file_DATA = new TFile(base_filepath+"/"+filename_prefix+"data_Single"+leptonFlavour+".root");
      TH1D *hist_DATA = (TH1D *)file_DATA->Get(dirname+"/"+var+"_"+dirname);

      for(unsigned int it_bkgd=0; it_bkgd<bkgds.size(); it_bkgd++){
        TFile *file_bkgd = new TFile(base_filepath+"/"+filename_prefix+bkgds.at(it_bkgd)+".root");
        TH1D *hist_bkgd = (TH1D *)file_bkgd->Get(dirname+"/"+var+"_"+dirname);
        if(!hist_bkgd) continue;

        if(hist_bkgd->GetBinContent(1)!=hist_bkgd->GetBinContent(1)){
          cout << base_filepath+"/"+filename_prefix+bkgds.at(it_bkgd)+".root" << endl;
        }

        hist_DATA->Add(hist_bkgd, -1.);
      }

      TH1D *hist_DY50 = (TH1D *)file_DY50->Get(dirname+"/"+var+"_"+dirname);
      //==== Scale DY
      double this_DYNormSF = GetDYNormSF(int_Year, leptonFlavour+"_"+region);
      hist_DY50->Scale(this_DYNormSF);

      //==== Rebin
      hist_DATA = RebinWRMass(hist_DATA, region, int_Year);
      hist_DY50 = RebinWRMass(hist_DY50, region, int_Year);

      if(hist_DATA_ee){
        hist_DATA_ee->Add(hist_DATA);
        hist_DY50_ee->Add(hist_DY50);
      }
      else{
        hist_DATA_ee = (TH1D *)hist_DATA->Clone();
        hist_DY50_ee = (TH1D *)hist_DY50->Clone();
      }
    } // END ch loop
    hist_DATA_ee->Divide(hist_DY50_ee);

    //==========================
    //==== 3) mm, only central
    //==========================

    it_fl_i = 1;
    it_fl_f = 1;


    TH1D *hist_DATA_mm = NULL;
    TH1D *hist_DY50_mm = NULL;

    for(int it_fl=it_fl_i; it_fl<=it_fl_f; it_fl++){

      TString leptonFlavour = "Electron";
      if(it_fl==1){
        leptonFlavour = "Muon";
      }

      TString dirname = "HNWR_Single"+leptonFlavour+"_"+region+"_"+MeasureRegion;

      TFile *file_DATA = new TFile(base_filepath+"/"+filename_prefix+"data_Single"+leptonFlavour+".root");
      TH1D *hist_DATA = (TH1D *)file_DATA->Get(dirname+"/"+var+"_"+dirname);

      for(unsigned int it_bkgd=0; it_bkgd<bkgds.size(); it_bkgd++){
        TFile *file_bkgd = new TFile(base_filepath+"/"+filename_prefix+bkgds.at(it_bkgd)+".root");
        TH1D *hist_bkgd = (TH1D *)file_bkgd->Get(dirname+"/"+var+"_"+dirname);
        if(!hist_bkgd) continue;

        if(hist_bkgd->GetBinContent(1)!=hist_bkgd->GetBinContent(1)){
          cout << base_filepath+"/"+filename_prefix+bkgds.at(it_bkgd)+".root" << endl;
        }

        hist_DATA->Add(hist_bkgd, -1.);
      }

      TH1D *hist_DY50 = (TH1D *)file_DY50->Get(dirname+"/"+var+"_"+dirname);
      //==== Scale DY
      double this_DYNormSF = GetDYNormSF(int_Year, leptonFlavour+"_"+region);
      hist_DY50->Scale(this_DYNormSF);

      //==== Rebin
      hist_DATA = RebinWRMass(hist_DATA, region, int_Year);
      hist_DY50 = RebinWRMass(hist_DY50, region, int_Year);

      if(hist_DATA_mm){
        hist_DATA_mm->Add(hist_DATA);
        hist_DY50_mm->Add(hist_DY50);
      }
      else{
        hist_DATA_mm = (TH1D *)hist_DATA->Clone();
        hist_DY50_mm = (TH1D *)hist_DY50->Clone();
      }
    } // END ch loop
    for(int ix=1; ix<=hist_DATA_mm->GetXaxis()->GetNbins(); ix++){
      double x_l = hist_DATA_mm->GetXaxis()->GetBinLowEdge(ix);
      double x_r = hist_DATA_mm->GetXaxis()->GetBinUpEdge(ix);
      //cout << "["<<x_l<<","<<x_r<<"] Data = " << hist_DATA_mm->GetBinContent(ix) << ", DY = " << hist_DY50_mm->GetBinContent(ix) << endl;
    }
    hist_DATA_mm->Divide(hist_DY50_mm);

    //==== calculate ratio
    TH1D *hist_ratio_AllCh = (TH1D *)hist_DY50_AllCh->Clone(region+"_ratio_AllCh");
    TH1D *hist_ratio_eemmSyst = (TH1D *)hist_DY50_AllCh->Clone(region+"_ratio_eemmSyst");
    for(int ix=1; ix<=hist_DY50_AllCh->GetXaxis()->GetNbins(); ix++){
      double x_l = hist_DY50_AllCh->GetXaxis()->GetBinLowEdge(ix);
      double x_r = hist_DY50_AllCh->GetXaxis()->GetBinUpEdge(ix);

      double y_Data = hist_DATA_AllCh->GetBinContent(ix);
      double err_Data = hist_DATA_AllCh->GetBinError(ix);
      double relerr_Data = err_Data/y_Data;

      double y_MC = hist_DY50_AllCh->GetBinContent(ix);
      double err_MC = hist_DY50_AllCh->GetBinError(ix);
      double relerr_MC = err_MC/y_MC;

      //==== HOTFIX adding lumi
      //relerr_MC = sqrt( relerr_MC*relerr_MC + LumiError*LumiError );
      //err_MC = relerr_MC * y_MC;

      double ratio = y_Data/y_MC;
      double relerr_ratio = sqrt( relerr_Data*relerr_Data + relerr_MC*relerr_MC );
      double err_ratio = relerr_ratio*ratio;

      if(y_MC==0.){
        ratio = 1.;
        err_ratio = 0.;
      }

      hist_ratio_AllCh->SetBinContent(ix, ratio);
      hist_ratio_AllCh->SetBinError(ix, err_ratio);

      double y_ee = hist_DATA_ee->GetBinContent(ix);
      double y_mm = hist_DATA_mm->GetBinContent(ix);
      double diff_eemm = fabs(y_ee-y_mm);
      hist_ratio_eemmSyst->SetBinContent(ix, ratio);
      hist_ratio_eemmSyst->SetBinError(ix, diff_eemm);

      cout << "["<<x_l<<","<<x_r<<"] : data = " << y_Data << " +- " << 100.*err_Data/y_Data << " %, MC = " << y_MC << " +- " << 100.*err_MC/y_MC << " %" << " --> ratio = " << ratio << " +- " << 100.*err_ratio << " %, ee = " << y_ee << ", mm = " << y_mm << endl;
    } 

    //==== Write

    file_out->cd();
    hist_ratio_AllCh->Write();
    hist_ratio_eemmSyst->Write();

    hist_DATA_ee->SetName(region+"_EEOnlyRatio");
    hist_DATA_ee->Write();
    hist_DATA_mm->SetName(region+"_MuMuOnlyRatio");
    hist_DATA_mm->Write();

    //==== plotting

    TCanvas *c1 = new TCanvas("c1", "", 800, 800);
    c1->cd();

    TH1D *hist_dummy = new TH1D("hist_dummy", "", 800, 0., 8000.);
    hist_dummy->GetXaxis()->SetTitle("m_{W_{RECO}} (GeV)");
    hist_dummy->GetYaxis()->SetRangeUser(0.5, 1.5);
    hist_dummy->GetYaxis()->SetTitle("(Data-non-DY)/DY");

    hist_dummy->GetYaxis()->SetLabelSize(0.04);
    hist_dummy->GetYaxis()->SetTitleSize(0.054);
    hist_dummy->GetYaxis()->SetTitleOffset(1.30);
    hist_dummy->GetXaxis()->SetLabelSize(0.03);
    hist_dummy->GetXaxis()->SetTitleSize(0.05);

    hist_dummy->Draw("hist");

    hist_ratio_eemmSyst->SetMarkerColor(0);
    hist_ratio_eemmSyst->SetMarkerSize(0);
    hist_ratio_eemmSyst->SetFillStyle(3013);
    hist_ratio_eemmSyst->SetFillColor(kBlack);
    hist_ratio_eemmSyst->SetLineColor(0);
    //hist_ratio_eemmSyst->Draw("e2same");

    hist_DATA_ee->SetBinContent(1,1);
    hist_DATA_ee->SetBinError(1,0);
    hist_DATA_ee->SetLineColor(kGreen);

    hist_DATA_mm->SetBinContent(1,1);
    hist_DATA_mm->SetBinError(1,0);
    hist_DATA_mm->SetLineColor(kBlue);

    hist_DATA_ee->Draw("histsame");
    hist_DATA_mm->Draw("histsame");

    TLegend *lg = new TLegend(0.6, 0.6, 0.9, 0.9);
    lg->AddEntry(hist_ratio_AllCh, "Nominal ratio", "le");
    //lg->AddEntry(hist_ratio_eemmSyst, "#Delta(ee-only,#mu#mu-only) ", "f");
    lg->AddEntry(hist_DATA_ee, "ee-only ratio", "l");
    lg->AddEntry(hist_DATA_mm, "#mu#mu-only ratio", "l");
    lg->Draw();

    TLatex channelname;
    channelname.SetNDC();
    channelname.SetTextSize(0.037);
    channelname.DrawLatex(0.2, 0.88, region);

    hist_ratio_AllCh->Draw("histe1same");
    hist_ratio_AllCh->SetLineColor(kBlack);

    c1->SaveAs(base_plotpath+"/"+region+".pdf");
    c1->Close();


  } // End region loop

  file_out->Close();
}

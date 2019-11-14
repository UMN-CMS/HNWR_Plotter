using namespace RooFit;
#include "mylib.h"
#include "canvas_margin.h"
#include "FitHistogram.C"

//==== TODO add year here
void FitBackgrounds(int i_region=0, int i_channel=0){

  setTDRStyle();
  SumW2Error(kTRUE);
  gStyle->SetOptStat(0);

  TH1::SetDefaultSumw2(true);
  TH1::AddDirectory(kFALSE);

  TString WORKING_DIR = getenv("PLOTTER_WORKING_DIR");
  TString dataset = getenv("CATANVERSION");
  TString ENV_PLOT_PATH = getenv("PLOT_PATH");

  TString Year = "2016";
  TString region = "Resolved_SR";
  TString channel = "Electron";
  if(i_region==0) region = "Resolved_SR";
  else if(i_region==1) region = "Boosted_SR";
  if(i_channel==0) channel = "Electron";
  else if(i_channel==1) channel = "Muon";

/*
  TString sample = "DYJets_MG_HT_Reweighted";
  TString samplealias = "DY";
*/

  TString sample = "EMuMethod_TTLX_powheg";
  TString samplealias = "tt";

  TString dirname = "HNWR_Single"+channel+"_"+region;
  TString base_filepath = WORKING_DIR+"/rootfiles/"+dataset+"/Regions/"+Year+"/";
  TString base_plotpath = ENV_PLOT_PATH+"/"+dataset+"/FitBackgrounds/"+Year+"/";
  gSystem->mkdir(base_plotpath, kTRUE);



  int NRebin = 20;
  double FitRange_l = 800;
  double FitRange_r = 8000;

  TFile *file = new TFile(base_filepath+"HNWRAnalyzer_SkimTree_LRSMHighPt_"+sample+".root");
  TH1D *hist = (TH1D *)file->Get(dirname+"/WRCand_Mass_"+dirname);
  double integral = hist->Integral();
  hist->Rebin(NRebin);

  TCanvas *c = new TCanvas("c","",600,600);
  canvas_margin(c);
  c->cd();

  RooPlot* xframe;
  for(int i_fit=0; i_fit<2; i_fit++){

    FitHistogram m;
    m.doDebug = false;
    m.SetHist(hist);
    m.SetFitVar("mwr", 800., 8000.);
    m.InitFitVar();

    Color_t color;
    if(i_fit==0){

      m.Name = "DijetFunction";
      m.functionalForm = "func_dijet_p0 * pow(1-mwr/13000.,func_dijet_p1)/ pow(mwr/13000., func_dijet_p2+func_dijet_p3*TMath::Log10(mwr/13000.))";
      m.fitRangeMin = 800.;
      m.fitRangeMax = 8000.;

      m.InitParameters(4);

      m.parNames.at(0) = "func_dijet_p0";
      m.parRangeMins.at(0) = 1E-5;
      m.parRangeMaxs.at(0) = 20.;

      m.parNames.at(1) = "func_dijet_p1";
      m.parRangeMins.at(1) = 0.;
      m.parRangeMaxs.at(1) = 20.;

      m.parNames.at(2) = "func_dijet_p2";
      m.parRangeMins.at(2) = 0.;
      m.parRangeMaxs.at(2) = 20.;

      m.parNames.at(3) = "func_dijet_p3";
      m.parRangeMins.at(3) = 0.;
      m.parRangeMaxs.at(3) = 20.;

      color = kGreen;

    }
    else if(i_fit==1){

      m.Name = "TrijetFunction";
      m.functionalForm = "func_trijet_p0 * (func_trijet_p2 * mwr/13000. - 1) / pow(mwr/13000., func_trijet_p1 + func_trijet_p3 * TMath::Log10(mwr/13000.) + func_trijet_p4 * TMath::Log10(mwr/13000.) * TMath::Log10(mwr/13000.))";
      m.fitRangeMin = 800.;
      m.fitRangeMax = 8000.;

      m.InitParameters(5);

      m.parNames.at(0) = "func_trijet_p0";
      m.parRangeMins.at(0) = 1E-5;
      m.parRangeMaxs.at(0) = 100.;

      m.parNames.at(1) = "func_trijet_p1";
      m.parRangeMins.at(1) = 0.;
      m.parRangeMaxs.at(1) = 50.;

      m.parNames.at(2) = "func_trijet_p2";
      m.parRangeMins.at(2) = 13000./800., 100.;
      m.parRangeMaxs.at(2) = 100.;

      m.parNames.at(3) = "func_trijet_p3";
      m.parRangeMins.at(3) = 0.;
      m.parRangeMaxs.at(3) = 50.;

      m.parNames.at(4) = "func_trijet_p4";
      m.parRangeMins.at(4) = 0.;
      m.parRangeMaxs.at(4) = 50.;

      color = kViolet;

    }

    m.Fit();

    RooRealVar* rrv = m.getFitVar();
    RooDataHist* rdh = m.getDataHist();

    if(i_fit==0){
      xframe = rrv->frame();
      rdh->plotOn(xframe);
    }

    m.getPdf()->plotOn(xframe,LineColor(color), Range(800,8000));


  }

  xframe->Draw();
  xframe->SetMinimum(1E-6);
  xframe->SetMaximum(500);

  c->SetLogy();

  c->SaveAs(base_plotpath+"/"+region+"_"+channel+"_"+samplealias+".pdf");
  c->SaveAs(base_plotpath+"/"+region+"_"+channel+"_"+samplealias+".png");
  c->Close();

}

using namespace RooFit;
#include "mylib.h"
#include "canvas_margin.h"

void test(int i_region=0, int i_channel=0){

  setTDRStyle();
  SumW2Error(kTRUE);
  gStyle->SetOptStat(0);

  TH1::SetDefaultSumw2(true);
  TH1::AddDirectory(kFALSE);

  TString WORKING_DIR = getenv("PLOTTER_WORKING_DIR");
  TString dataset = getenv("CATANVERSION");
  TString ENV_PLOT_PATH = getenv("PLOT_PATH");

  TString region = "Resolved_SR";
  TString channel = "Electron";
  if(i_region==0) region = "Resolved_SR";
  else if(i_region==1) region = "Boosted_SR";

  if(i_channel==0) channel = "Electron";
  else if(i_channel==1) channel = "Muon";

  int NRebin = 20;
  TString Year = "2016";

  TString dirname = "HNWR_Single"+channel+"_"+region;

  //=== If not, use geenral
  TString base_filepath = WORKING_DIR+"/rootfiles/"+dataset+"/Regions/"+Year+"/";

  //=============
  //==== DY
  //=============

  double FitRange_l_DY = 800;
  double FitRange_r_DY = 8000;

  TFile *file_DY = new TFile(base_filepath+"HNWRAnalyzer_SkimTree_LRSMHighPt_DYJets_MG_HT_Reweighted.root");
  TH1D *hist_DY = (TH1D *)file_DY->Get(dirname+"/WRCand_Mass_"+dirname);
  double integral_DY = hist_DY->Integral();
  hist_DY->Rebin(NRebin);

  //TF1 *f1 = new TF1("f1","[0]*pow(1-(x/13000.),[1])/pow(x/13000.,[2])",FitRange_l_DY,FitRange_r_DY);
  TF1 *f1 = new TF1("f1","[0]*pow(1-(x/13000.),[1])/pow(x/13000.,[2]+[3]*log(x/13000.))",FitRange_l_DY,FitRange_r_DY);
  f1->SetParLimits(0,1, 1000);
  f1->SetParLimits(1, 0,30);
  f1->SetParLimits(2, 0,5);
  f1->SetParLimits(3,0,1);

  //==== TH1:Fit test
  hist_DY->Fit("f1","P","",FitRange_l_DY,FitRange_r_DY);

  TCanvas *c1 = new TCanvas("c1","",600,600);
  canvas_margin(c1);
  c1->cd();
  hist_DY->Draw();
  c1->SetLogy();




}

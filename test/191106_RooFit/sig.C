using namespace RooFit;
#include "mylib.h"
#include "canvas_margin.h"

void sig(int i_region=0, int i_channel=0){

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

  int NRebin = 10;
  TString Year = "2016";

  TString dirname = "HNWR_Single"+channel+"_"+region;

  //=== If not, use geenral
  TString base_filepath = WORKING_DIR+"/rootfiles/"+dataset+"/Regions/"+Year+"/";

  //=============
  //==== DY
  //=============

  double FitRange_l_sig = 800;
  double FitRange_r_sig = 8000;

  int mWR = 4000;
  int mN = 3000;
  TString mass = "WR"+TString::Itoa(mWR,10)+"_N"+TString::Itoa(mN,10);
  TFile *file_sig = new TFile(base_filepath+"Signal/HNWRAnalyzer_WRtoNLtoLLJJ_"+mass+".root");
  TH1D *hist_sig = (TH1D *)file_sig->Get(dirname+"/WRCand_Mass_"+dirname);
  double integral_sig = hist_sig->Integral();
  hist_sig->Rebin(NRebin);

  //==== data from histogram
  RooRealVar mwr_sig("mwr_sig", "mwr_sig", 800., 8000.);
  RooDataHist rooDataHist_sig("rooDataHist_sig", "rooDataHist_sig", mwr_sig, hist_sig);

  RooRealVar CBall_1_mean_sig("CBall_1_mean_sig", "CBall_1_mean_sig", mWR*0.5, mWR*1.5);
  RooRealVar CBall_1_sigma_sig("CBall_1_sigma_sig", "CBall_1_sigma_sig", 0., mWR);
  RooRealVar CBall_1_alpha_sig("CBall_1_alpha_sig", "CBall_1_alpha_sig", 0., 10.);
  RooRealVar CBall_1_n_sig("CBall_1_n_sig", "CBall_1_n_sig", 0., 5.);
  RooCBShape CBall_1_sig("CBall_1_sig", "CBall_1_sig", mwr_sig, CBall_1_mean_sig, CBall_1_sigma_sig, CBall_1_alpha_sig, CBall_1_n_sig);

  RooRealVar CBall_2_mean_sig("CBall_2_mean_sig", "CBall_2_mean_sig", mWR*0.5, mWR*1.5);
  RooRealVar CBall_2_sigma_sig("CBall_2_sigma_sig", "CBall_2_sigma_sig", 0., mWR);
  RooRealVar CBall_2_alpha_sig("CBall_2_alpha_sig", "CBall_2_alpha_sig", -10., 0.);
  RooRealVar CBall_2_n_sig("CBall_2_n_sig", "CBall_2_n_sig", 0., 5.);
  RooCBShape CBall_2_sig("CBall_2_sig", "CBall_2_sig", mwr_sig, CBall_2_mean_sig, CBall_2_sigma_sig, CBall_2_alpha_sig, CBall_2_n_sig);

  RooRealVar CBall_frac("CBall_frac", "", 0.5);
  RooAddPdf CBall_sum("CBall_sum", "CBall_sum", RooArgList(CBall_1_sig, CBall_2_sig), RooArgList(CBall_frac));

  //==== Draw
  RooPlot* xframe_sig = mwr_sig.frame();

  TCanvas *c_sig = new TCanvas("c_sig","",600,600);
  canvas_margin(c_sig);
  c_sig->cd();
  rooDataHist_sig.plotOn(xframe_sig);

  CBall_sum.fitTo(rooDataHist_sig, Range(FitRange_l_sig,FitRange_r_sig));
  CBall_sum.plotOn(xframe_sig,LineColor(kRed), Range(800,8000));

  xframe_sig->Draw();
  //xframe_sig->SetMinimum(1E-6);
  //xframe_sig->SetMaximum(500);

  //==== summary
  CBall_1_mean_sig.Print();
  CBall_1_sigma_sig.Print();
  CBall_1_alpha_sig.Print();
  CBall_1_n_sig.Print();
  CBall_2_mean_sig.Print();
  CBall_2_sigma_sig.Print();
  CBall_2_alpha_sig.Print();
  CBall_2_n_sig.Print();

  c_sig->SaveAs(region+"_"+channel+"_"+mass+".pdf");
  c_sig->SaveAs(region+"_"+channel+"_"+mass+".png");
  c_sig->Close();

}

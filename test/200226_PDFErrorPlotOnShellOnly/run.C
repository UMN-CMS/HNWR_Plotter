#include "canvas_margin.h"
#include "mylib.h"

void run(int mWR, int mN){

  setTDRStyle();

  int DataYear = 2016;
  TString mass = "WR"+TString::Itoa(mWR,10)+"_N"+TString::Itoa(mN,10);
  TString channel = "Muon";
  TString region = "Resolved";

  TFile *file = new TFile("/data6/Users/jskim/HNWR_Plotter/rootfiles/Run2Legacy_v4__Default/Regions/"+TString::Itoa(DataYear,10)+"/Signal/HNWRAnalyzer_WRtoNLtoLLJJ_"+mass+".root");

  //==== FlavFrac
  TH1D *hist_SignalFlavour = (TH1D *)file->Get("SignalFlavour");
  double flavFrac = 0.;
  if(channel=="Electron") flavFrac = hist_SignalFlavour->GetBinContent(2) / hist_SignalFlavour->GetEntries();
  else flavFrac = hist_SignalFlavour->GetBinContent(3) / hist_SignalFlavour->GetEntries();

  cout << "flavFrac = " << flavFrac << endl;

  TDirectoryFile *dir_Den = (TDirectoryFile *)file->Get("XsecSyst_Den");
  TDirectoryFile *dir_Num = (TDirectoryFile *)file->Get("XsecSyst_Num_HNWR_Single"+channel+"_"+region+"_SR");

  TCanvas *c_Den = new TCanvas("c_Den", "", 600, 600);
  canvas_margin(c_Den);

  TCanvas *c_Num = new TCanvas("c_Num", "", 600, 600);
  canvas_margin(c_Num);

  TH1D *hist_DenDistrib = new TH1D("hist_DenDistrib", "", int(3./0.01), 0., 3.);
  TH1D *hist_Num_Nominal = NULL;

  TLegend *lg = new TLegend(0.20, 0.70, 0.40, 0.80);

  for(unsigned int i=0; i<=100; i++){

    TH1D *hist_Den_ThisSet = (TH1D *)dir_Den->Get("PDFWeights_Error_"+TString::Itoa(i,10)+"_XsecSyst_Den");
    double this_Den = hist_Den_ThisSet->GetBinContent(1);
    hist_DenDistrib->Fill( this_Den );

    TH1D *hist_Num_ThisSet = (TH1D *)dir_Num->Get("PDFWeights_Error_"+TString::Itoa(i,10)+"_XsecSyst_Num_HNWR_Single"+channel+"_"+region+"_SR");
    hist_Num_ThisSet = RebinWRMass(hist_Num_ThisSet, "HNWR_Single"+channel+"_"+region+"_SR", DataYear);
    hist_Num_ThisSet->Scale(1./this_Den/flavFrac);

    if(i==0){
      hist_Num_ThisSet->SetLineWidth(3);
      hist_Num_ThisSet->SetLineColor(kRed);
      hist_Num_ThisSet->GetYaxis()->SetRangeUser(0., 1.0);
      hist_axis(hist_Num_ThisSet);

      hist_Num_ThisSet->GetXaxis()->SetTitle("m(W_{R}) (GeV)");
      hist_Num_ThisSet->GetYaxis()->SetTitle("Acceptance");

      hist_Num_Nominal = (TH1D *)hist_Num_ThisSet->Clone();

      lg->AddEntry(hist_Num_ThisSet, "Nominal", "l");
    }
    else{
      hist_Num_ThisSet->SetLineColor(kBlack);
    }

    if(i==1){
      lg->AddEntry(hist_Num_ThisSet, "Error sets", "l");
    }

    c_Num->cd();
    hist_Num_ThisSet->Draw("histsame");

  }

  c_Den->cd();
  hist_DenDistrib->Draw("hist");
  c_Den->SaveAs(mass+"_"+channel+"_"+region+"_"+"Den.pdf");
  c_Den->Close();

  c_Num->cd();
  hist_Num_Nominal->Draw("histsame");

  TLatex latex_info;
  latex_info.SetNDC();
  latex_info.SetTextSize(0.035);
  latex_info.DrawLatex(0.20, 0.85, "m(W_{R}) = "+TString::Itoa(mWR,10)+" GeV, m(N) = "+TString::Itoa(mN,10)+" GeV");
  lg->Draw();

  c_Num->SaveAs(mass+"_"+channel+"_"+region+"_"+"Num.pdf");
  c_Num->Close();

}

#include "canvas_margin.h"
#include "mylib.h"
#include "LRSMSignalInfo.h"

void Draw(int mWR, int mN){

  TString outdir = "200818_PDFTest";

  setTDRStyle();

  int DataYear = 2017;
  TString mass = "WR"+TString::Itoa(mWR,10)+"_N"+TString::Itoa(mN,10);

  TString dataset = getenv("CATANVERSION");
  TString ENV_PLOT_PATH = getenv("PLOT_PATH");

  TString channel = "Muon";
  TString region = "Boosted";

  TString SigDirName = (channel=="Electron") ? "Signal_EE" : "Signal_MuMu";

  TFile *file = new TFile("/data6/Users/jskim/HNWR_Plotter/rootfiles/Run2Legacy_v4__Default/Regions/"+TString::Itoa(DataYear,10)+"/"+SigDirName+"/HNWRAnalyzer_WRtoNLtoLLJJ_"+mass+".root");

  //==== FlavFrac
  TH1D *hist_SignalFlavour = (TH1D *)file->Get("SignalFlavour");
  double flavFrac = 0.;
  if(channel=="Electron") flavFrac = hist_SignalFlavour->GetBinContent(2) / hist_SignalFlavour->Integral();
  else flavFrac = hist_SignalFlavour->GetBinContent(3) / hist_SignalFlavour->Integral();

  flavFrac = 1.;

  TDirectoryFile *dir_Den = (TDirectoryFile *)file->Get("XsecSyst_Den");
  TDirectoryFile *dir_Num = (TDirectoryFile *)file->Get("XsecSyst_Num_HNWR_Single"+channel+"_"+region+"_SR");

  TCanvas *c_Den = new TCanvas("c_Den", "", 600, 600);
  canvas_margin(c_Den);

  TCanvas *c_Num = new TCanvas("c_Num", "", 600, 600);
  canvas_margin(c_Num);

  TH1D *hist_DenDistrib = new TH1D("hist_DenDistrib", "", int(3./0.01), 0., 3.);
  TH1D *hist_Num_Nominal = NULL;

  TLegend *lg = new TLegend(0.20, 0.70, 0.40, 0.80);

  int iBinMax;
  double y_Nominal;

  double err = 0.;

  for(unsigned int i=0; i<=100; i++){

    TH1D *hist_Den_ThisSet = (TH1D *)dir_Den->Get("PDFWeights_Error_"+TString::Itoa(i,10)+"_XsecSyst_Den");
    double this_Den = hist_Den_ThisSet->GetBinContent(1);
    hist_DenDistrib->Fill( this_Den );

    TH1D *hist_Num_ThisSet = (TH1D *)dir_Num->Get("PDFWeights_Error_"+TString::Itoa(i,10)+"_XsecSyst_Num_HNWR_Single"+channel+"_"+region+"_SR");
    hist_Num_ThisSet = RebinWRMass(hist_Num_ThisSet, "HNWR_Single"+channel+"_"+region+"_SR", DataYear);
    hist_Num_ThisSet->Scale(1./this_Den/flavFrac);

    //==== Posotive and Negative
    TH1D *hist_Positive = (TH1D *)dir_Den->Get("PDFWeights_Error_"+TString::Itoa(i,10)+"_XsecSyst_Den_Positive");
    TH1D *hist_Negative = (TH1D *)dir_Den->Get("PDFWeights_Error_"+TString::Itoa(i,10)+"_XsecSyst_Den_Negative");

    double N_Positive(0), Y_Positive(0);
    double N_Negative(0), Y_Negative(0);
    if(hist_Positive){
      N_Positive = hist_Positive->GetEntries();
      Y_Positive = hist_Positive->GetBinContent(1);
    }
    if(hist_Negative){
      N_Negative = hist_Negative->GetEntries();
      Y_Negative = hist_Negative->GetBinContent(1);
    }

    if(i==0){

      hist_Num_ThisSet->SetLineWidth(3);
      hist_Num_ThisSet->SetLineColor(kRed);
      hist_Num_ThisSet->GetYaxis()->SetRangeUser(0., 0.10);
      hist_axis(hist_Num_ThisSet);

      hist_Num_ThisSet->GetXaxis()->SetTitle("m(W_{R}) (GeV)");
      hist_Num_ThisSet->GetYaxis()->SetTitle("Acceptance");

      hist_Num_Nominal = (TH1D *)hist_Num_ThisSet->Clone();

      double tmp_max(-9999.);
      for(int ix=1; ix<=hist_Num_Nominal->GetXaxis()->GetNbins(); ix++){
        if(hist_Num_Nominal->GetBinContent(ix) > tmp_max){
          iBinMax = ix;
          tmp_max = hist_Num_Nominal->GetBinContent(ix);
        }
      }
      cout << "@@@@ iBinMax = " << iBinMax << " : ["<<hist_Num_Nominal->GetXaxis()->GetBinLowEdge(iBinMax)<<", "<<hist_Num_Nominal->GetXaxis()->GetBinUpEdge(iBinMax)<<"]" << endl;

      y_Nominal = hist_Num_Nominal->GetBinContent(iBinMax);

      lg->AddEntry(hist_Num_ThisSet, "Nominal", "l");
    }
    else{
      hist_Num_ThisSet->SetLineColor(kBlack);

      double y_PDFError = hist_Num_ThisSet->GetBinContent(iBinMax);

      double Acc_RelErr = fabs(y_PDFError-y_Nominal)/y_Nominal;

      bool IsOkay = true;
      if(i==38) IsOkay = false;
      if(y_PDFError<0) IsOkay = false;
      if( Acc_RelErr > 1.0 ) IsOkay = false;
      if(IsOkay){
      cout << i << "\t" << y_PDFError << "\t" << fabs(y_PDFError-y_Nominal)/y_Nominal << "\t" << this_Den << "\t" << N_Negative/(N_Negative+N_Positive) << "\t" << fabs(Y_Negative/(fabs(Y_Negative)+fabs(Y_Positive))) << endl;
        err += Acc_RelErr*Acc_RelErr;
      }

    }

      //cout << i << "\t" << y_PDFError << "\t" << fabs(y_PDFError-y_Nominal)/y_Nominal << "\t" << this_Den << "\t" << N_Negative/(N_Negative+N_Positive) << "\t" << fabs(Y_Negative/(fabs(Y_Negative)+fabs(Y_Positive))) << endl;

    if(i==1){
      lg->AddEntry(hist_Num_ThisSet, "Error sets", "l");
    }

    c_Num->cd();
    hist_Num_ThisSet->Draw("histsame");

  }

  err = sqrt(err);
  cout << "--> Err = " << err << endl;

  system("mkdir -p "+ENV_PLOT_PATH+"/"+dataset+"/"+outdir);

  c_Den->cd();
  hist_DenDistrib->Draw("hist");
  c_Den->SaveAs(ENV_PLOT_PATH+"/"+dataset+"/"+outdir+"/"+mass+"_"+channel+"_"+region+"_"+"Den.pdf");
  c_Den->Close();

  c_Num->cd();
  hist_Num_Nominal->Draw("histsame");

  TLatex latex_info;
  latex_info.SetNDC();
  latex_info.SetTextSize(0.035);
  latex_info.DrawLatex(0.20, 0.85, "m(W_{R}) = "+TString::Itoa(mWR,10)+" GeV, m(N) = "+TString::Itoa(mN,10)+" GeV");
  lg->Draw();

  c_Num->SaveAs(ENV_PLOT_PATH+"/"+dataset+"/"+outdir+"/"+mass+"_"+channel+"_"+region+"_"+"Num.pdf");
  c_Num->Close();


}

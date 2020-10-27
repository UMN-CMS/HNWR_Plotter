#include "canvas_margin.h"
#include "mylib.h"
#include "LRSMSignalInfo.h"

double GetErrors(int mWR, int mN, int i_Year, int i_Channel, int i_Region);

void run(int i_Year, int i_Channel, int i_Region){

  setTDRStyle();
  gStyle->SetOptStat(0);

  TH1::SetDefaultSumw2(true);
  TH1::AddDirectory(kFALSE);

  TString WORKING_DIR = getenv("PLOTTER_WORKING_DIR");
  TString dataset = getenv("CATANVERSION");
  TString ENV_PLOT_PATH = getenv("PLOT_PATH");

  LRSMSignalInfo lrsminfo;
  lrsminfo.GetMassMaps();
  int N_TOTAL(0);
  for(map< double, vector<double> >::iterator it=lrsminfo.maps_WR_to_N.begin(); it!=lrsminfo.maps_WR_to_N.end(); it++){

    double m_WR = it->first;
    vector<double> this_m_Ns = it->second;

    for(int it_N=0; it_N<this_m_Ns.size(); it_N++){

      double m_N = this_m_Ns.at(it_N);
      
      double error = GetErrors(m_WR, m_N, i_Year, i_Channel, i_Region);
      cout << m_WR << "\t" << m_N << "\t" << error << endl;

    }

  }


}


double GetErrors(int mWR, int mN, int i_Year, int i_Channel, int i_Region){

  TString outdir = "200818_PDFTest";

  int DataYear = i_Year;
  TString mass = "WR"+TString::Itoa(mWR,10)+"_N"+TString::Itoa(mN,10);

  TString dataset = getenv("CATANVERSION");
  TString ENV_PLOT_PATH = getenv("PLOT_PATH");

  TString channel = (i_Channel==0) ? "Electron" : "Muon";
  TString region = (i_Region==0) ? "Resolved" : "Boosted";

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

  TH1D *hist_DenDistrib = new TH1D("hist_DenDistrib", "", int(3./0.01), 0., 3.);
  TH1D *hist_Num_Nominal = NULL;

  double y_Nominal;

  int iBinMax;

  double out(0.);

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

      hist_Num_Nominal = (TH1D *)hist_Num_ThisSet->Clone();

      double tmp_max(-9999.);
      for(int ix=1; ix<=hist_Num_Nominal->GetXaxis()->GetNbins(); ix++){
        if(hist_Num_Nominal->GetBinContent(ix) > tmp_max){
          iBinMax = ix;
          tmp_max = hist_Num_Nominal->GetBinContent(ix);
        }
      }
      //cout << "@@@@ iBinMax = " << iBinMax << " : ["<<hist_Num_Nominal->GetXaxis()->GetBinLowEdge(iBinMax)<<", "<<hist_Num_Nominal->GetXaxis()->GetBinUpEdge(iBinMax)<<"]" << endl;

      y_Nominal = hist_Num_Nominal->GetBinContent(iBinMax);

    }
    else{

      double y_PDFError = hist_Num_ThisSet->GetBinContent(iBinMax);

      double Acc_RelErr = fabs(y_PDFError-y_Nominal)/y_Nominal;

      bool IsOkay = true;
      if(i==38) IsOkay = false;
      if(y_PDFError<0) IsOkay = false;
      if( Acc_RelErr > 1.0 ) IsOkay = false;
      if(IsOkay) out += Acc_RelErr*Acc_RelErr;

    }

  }

  return sqrt(out);

}

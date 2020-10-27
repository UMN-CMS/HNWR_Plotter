#include "mylib.h"
#include "canvas_margin.h"
#include "SignalSystematics.h"

void run(int MWR, int MN, TString PDFName=""){

  setTDRStyle();
  gStyle->SetOptStat(0);

  SignalSystematics m;

  TString mass = "WR"+TString::Itoa(MWR,10)+"_N"+TString::Itoa(MN,10);

  TString basedir = "";
  basedir = "/data6/Users/jskim/HNWR_Plotter/rootfiles/Run2Legacy_v4__Default/Regions/2016/Signal/";

  //basedir = "RunSyst__Signal__RunXsecSyst__RunNewPDF__"+PDFName+"__/";

  TString filepath = basedir+"HNWRAnalyzer_WRtoNLtoLLJJ_"+mass+".root";
  TFile *file = new TFile(filepath);

  m.file = file;
  m.region = "HNWR_SingleMuon_Boosted_SR";
  m.UseCustomRebin = true;
  m.DataYear = 2016;
  m.n_rebin = 40;

  
  TH1D *hist_sig_SignalFlavour = (TH1D *)file->Get("SignalFlavour");

  m.ChannelFrac = 1./hist_sig_SignalFlavour->GetEntries();
  if(m.region.Contains("Electron")) m.ChannelFrac *= hist_sig_SignalFlavour->GetBinContent(2);
  else if(m.region.Contains("Muon")) m.ChannelFrac *= hist_sig_SignalFlavour->GetBinContent(3);
  else{
    cout << "WTF?? channel = " << m.region << endl;
    return;
  }

  m.DoDebug = true;
  m.DrawPlot = true;
  m.histPrefix = PDFName+"_";
  m.outputdir = "/data6/Users/jskim/HNWR_Plotter/output/Run2Legacy_v4__Default/temp/200122_QuickSignalSyst/";

  m.isReplica = PDFName=="NNPDF23_lo_as_0130_qed" || PDFName=="NNPDF31_nnlo_as_0118_mc" || PDFName=="PDF4LHC15_nnlo_mc";

  //m.isReplica = false;

  TH1D *hist_sig = (TH1D *)file->Get(m.region+"/WRCand_Mass_"+m.region);

  //hist_sig->Rebin(40);
  hist_sig = RebinWRMass(hist_sig, m.region, m.DataYear);

  m.hist_Central = hist_sig;
  m.Run();

}


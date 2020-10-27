#include "mylib.h"
#include "canvas_margin.h"
#include "SignalSystematics.h"

void run(int MWR, int MN, TString PDFName=""){

  setTDRStyle();
  gStyle->SetOptStat(0);

  SignalSystematics m;

  TString mass = "WR"+TString::Itoa(MWR,10)+"_N"+TString::Itoa(MN,10);

  TString basedir = "";
  basedir = "/data6/Users/jskim/HNWR_Plotter/rootfiles/Run2Legacy_v3__Default/Regions/2017/Signal/";

  //basedir = "RunSyst__Signal__RunXsecSyst__RunNewPDF__"+PDFName+"__/";

  TString filepath = basedir+"HNWRAnalyzer_WRtoNLtoLLJJ_"+mass+".root";
  TFile *file = new TFile(filepath);

  m.file = file;
  m.region = "HNWR_SingleMuon_Resolved_SR";
  m.UseCustomRebin = true;

  
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
  m.outputdir = "/data6/Users/jskim/HNWR_Plotter/output/Run2Legacy_v3__Default/temp/190918_QuickSignalSyst/";

  //m.isReplica = true;

  TH1D *hist_sig = (TH1D *)file->Get(m.region+"/WRCand_Mass_"+m.region);
  m.hist_Central = hist_sig;
  m.Run();

}


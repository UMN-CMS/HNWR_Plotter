#include "canvas_margin.h"
#include "mylib.h"

void Draw_CompareDYPtReweight(int xxx=0){

  int n_rebin = 20;

  TString filename_prefix = "HNWRAnalyzer_SkimTree_LRSMHighPt_";

  //gErrorIgnoreLevel = kFatal;

  setTDRStyle();

  TString Year = "2016";
  TString TotalLumi = "35.92 fb^{-1} (13 TeV)";
  if(xxx==1){
    Year = "2017";
    TotalLumi = "41.53 fb^{-1} (13 TeV)";
  }
  if(xxx==2){
    Year = "2018";
    TotalLumi = "59.74 fb^{-1} (13 TeV)";
  }

  TLatex latex_CMSPriliminary, latex_Lumi;
  latex_CMSPriliminary.SetNDC();
  latex_Lumi.SetNDC();
  latex_CMSPriliminary.SetTextSize(0.035);

  latex_Lumi.SetTextSize(0.035);
  latex_Lumi.SetTextFont(42);

  gStyle->SetOptStat(0);

  TH1::SetDefaultSumw2(true);
  TH1::AddDirectory(kFALSE);

  TString WORKING_DIR = getenv("PLOTTER_WORKING_DIR");
  TString dataset = getenv("CATANVERSION");
  TString ENV_PLOT_PATH = getenv("PLOT_PATH");

  TString base_filepath = WORKING_DIR+"/rootfiles/"+dataset+"/Regions/"+Year+"/";
  TString base_plotpath = ENV_PLOT_PATH+"/"+dataset+"/DYPtReweightComparison/"+Year+"/";

  if( !gSystem->mkdir(base_plotpath, kTRUE) ){
    cout
    << "###################################################" << endl
    << "Directoy " << base_plotpath << " is created" << endl
    << "###################################################" << endl
    << endl;
  }

  TString samplename_DY10to50 = "DYJets10to50_MG";
  TString samplename_DY50 = "DYJets_MG_HT";
  if(Year=="2016"){
  }
  else if(Year=="2017"){
    samplename_DY10to50 = "DYJets10to50_MG";
    samplename_DY50 = "DYJets_MG_JetBinned";
  }
  else if(Year=="2018"){
    samplename_DY10to50 = "DYJets10to50_MG";
    samplename_DY50 = "DYJets_MG_HT";
  }

  TFile *file_Before = new TFile(base_filepath+"/"+filename_prefix+samplename_DY50+".root");
  TFile *file_After = new TFile(base_filepath+"/"+filename_prefix+samplename_DY50+"_Reweighted.root");

  vector<TString> regions = {
    "Resolved_SR",
    "Boosted_SR",
  };

  for(int it_fl=0; it_fl<2; it_fl++){

    TString leptonFlavour = "Electron";
    if(it_fl==1){
      leptonFlavour = "Muon";
    }

    for(int it_region=0; it_region<regions.size(); it_region++){

      TString region = regions.at(it_region);
      TString dirname = "HNWR_Single"+leptonFlavour+"_"+region;
      TString var = "WRCand_Mass";

      TH1D *hist_Before = (TH1D *)file_Before->Get(dirname+"/"+var+"_"+dirname);
      TH1D *hist_After = (TH1D *)file_After->Get(dirname+"/"+var+"_"+dirname);

      hist_Before->Rebin(n_rebin);
      hist_After->Rebin(n_rebin);

      hist_Before->GetXaxis()->SetRangeUser(0., 2600.);
      hist_After->GetXaxis()->SetRangeUser(0., 2600.);

      hist_Before->SetLineColor(kBlack);
      hist_After->SetLineColor(kRed);

      hist_Before->SetLineWidth(2);
      hist_After->SetLineWidth(2);

      hist_Before->GetYaxis()->SetRangeUser(0., 1.2*max( GetMaximum(hist_Before, 0.), GetMaximum(hist_After, 0.) ) );

      //==== canvas
      TCanvas *c_comp = new TCanvas("c_comp", "", 800, 800);
      TPad *c1_up;
      TPad *c1_down;
      c1_up = new TPad("c1", "", 0, 0.25, 1, 1);
      c1_down = new TPad("c1_down", "", 0, 0, 1, 0.25);
      canvas_margin(c_comp, c1_up, c1_down);

      c1_up->Draw();
      c1_down->Draw();

      //==== up

      c1_up->cd(); 

      hist_Before->Draw("histe1");
      hist_After->Draw("histsamee1");

      TLegend *lg = new TLegend(0.6, 0.8, 0.9, 0.9);
      lg->AddEntry(hist_Before, "Before reweighting", "l");
      lg->AddEntry(hist_After, "After reweighting", "l");

      hist_Before->GetYaxis()->SetTitle("Events / "+TString::Itoa(10*n_rebin,10)+" GeV");

      //==== down

      c1_down->cd();
      TH1D *hist_Ratio = (TH1D *)hist_After->Clone();
      hist_Ratio->SetLineColor(kBlue);
      TString xtitle = "m_{lljj} (GeV)";
      if(region.Contains("Boosted")) xtitle = "m_{lJ} (GeV)";
      hist_Ratio->GetXaxis()->SetTitle(xtitle);
      hist_Ratio->GetYaxis()->SetTitle("Ratio");
      hist_Ratio->Divide(hist_Before);
      hist_Ratio->GetYaxis()->SetRangeUser(0.85, 1.15);
      hist_Ratio->Draw("l");

      hist_axis(hist_Before, hist_Ratio);

      //==== y=1 line
      double x_1[2], y_1[2];
      x_1[0] = 5000;  y_1[0] = 1;
      x_1[1] = -5000;  y_1[1] = 1;
      TGraph *g1 = new TGraph(2, x_1, y_1);
      g1->Draw("same");

      c_comp->cd();

      lg->Draw();

      latex_CMSPriliminary.DrawLatex(0.15, 0.96, "#font[62]{CMS} #font[42]{#it{#scale[0.8]{Simulation}}}");
      latex_Lumi.DrawLatex(0.72, 0.96, TotalLumi);

      c_comp->SaveAs(base_plotpath+"/"+region+"_"+leptonFlavour+"_"+var+".pdf");
      c_comp->SaveAs(base_plotpath+"/"+region+"_"+leptonFlavour+"_"+var+".png");
      c_comp->Close();
    }



  }

}

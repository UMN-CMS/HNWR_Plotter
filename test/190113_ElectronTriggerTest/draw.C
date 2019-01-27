#include "canvas_margin.h"

void draw(){

  gStyle->SetOptStat(0);

  vector<TString> samples = {
"FullSIM_WRtoNLtoLLJJ_WR1000_N100_13TeV_TuneCUETP8M1",
"FullSIM_WRtoNLtoLLJJ_WR1000_N500_13TeV_TuneCUETP8M1",
"FullSIM_WRtoNLtoLLJJ_WR4000_N100_13TeV_TuneCUETP8M1",
"FullSIM_WRtoNLtoLLJJ_WR4000_N500_13TeV_TuneCUETP8M1",
"FullSIM_WRtoNLtoLLJJ_WR4000_N1000_13TeV_TuneCUETP8M1",
  };
  vector<Color_t> colors = {
    kRed,
    kBlue,
    kOrange,
    kGreen,
    kBlack,
  };
  vector<TString> samplealiases = {
    "WR=1000, N=100",
    "WR=1000, N=500",
    "WR=4000, N=100",
    "WR=4000, N=500",
    "WR=4000, N=1000",
  };

  vector<TString> cut_aliases = {
    "NoCut",
    "LeadElPt60",
    "LeadElHEEP",

    "Ele35",
    "Ele35 && SubEl",

    "Pho200",
    "Pho200 && SubEl",

    "Ele35||Pho200",
    "Ele35||Pho200 && SubEl",

    "Ele23Ele12",
    "Ele23Ele12 && SubEl",

    "DoubleEle33",
    "DoubleEle33 && SubEl",

    "DoublePho70",
    "DoublePho70 && SubEl",

    "DoubleEle33||DoublePho70",
    "DoubleEle33||DoublePho70 && SubEl",

  };

  vector<TString> cut_histnames = {
    "NoCut",
    "AtLeastOnePt60Electron",
    "LeadingElectronPassHEEP",

    "Case1_TriggerFirst__PassTriggers",
    "Case1_TriggerFirst__SecondElectron",

    "Case1_TriggerFirst__PassTriggers",
    "Case1_TriggerFirst__SecondElectron",

    "Case1_TriggerFirst__PassTriggers",
    "Case1_TriggerFirst__SecondElectron",

    "Case1_TriggerFirst__PassTriggers",
    "Case1_TriggerFirst__SecondElectron",

    "Case1_TriggerFirst__PassTriggers",
    "Case1_TriggerFirst__SecondElectron",

    "Case1_TriggerFirst__PassTriggers",
    "Case1_TriggerFirst__SecondElectron",

    "Case1_TriggerFirst__PassTriggers",
    "Case1_TriggerFirst__SecondElectron",


  };

  vector<int> cut_binindexes = {
    1,
    1,
    1,

    1,1,2,2,3,3,4,4,5,5,6,6,7,7,
  };

  cout << "cut_aliases.size() = " << cut_aliases.size() << endl;
  cout << "cut_histnames.size() = " << cut_histnames.size() << endl;
  cout << "cut_binindexes.size() = " << cut_binindexes.size() << endl;

  const int n_cut = cut_aliases.size();

  TCanvas *c1 = new TCanvas("c1", "", 600, 1000);
  canvas_margin(c1);
  c1->SetBottomMargin( 0.30 );
  c1->cd();

  TLegend *lg = new TLegend(0.3, 0.80, 0.9, 0.93);
  lg->SetNColumns(2);
  lg->SetBorderSize(0);
  lg->SetFillStyle(0);

  //==== formatting
  TH1D *hist_cutflow = new TH1D("hist_cutflow", "", n_cut, 0., 1.*n_cut);
  hist_axis(hist_cutflow);
  hist_cutflow->GetYaxis()->SetRangeUser(0., 1.2);
  for(unsigned int it_cut=0; it_cut<cut_aliases.size(); it_cut++){
    TString cut_alias = cut_aliases.at(it_cut);
    hist_cutflow->Draw("hist");
    hist_cutflow->GetXaxis()->SetBinLabel(it_cut+1, cut_alias);
    //hist_cutflow->GetXaxis()->ChangeLabel(it_cut+1, -1, -1, -1, -1, -1, -1);
    cout << "\t" << cut_alias;
  }
  cout << endl;
  hist_cutflow->GetXaxis()->LabelsOption("v");

  for(unsigned int it_sample=0; it_sample<samples.size(); it_sample++){

    TString sample = samples.at(it_sample);
    TString samplealias = samplealiases.at(it_sample);
    Color_t color = colors.at(it_sample);
    TFile *file = new TFile("rootfiles/HNWRSignalStudy_"+sample+".root");

    TH1D *hist_cutflow = new TH1D("hist_cutflow", "", n_cut, 0., 1.*n_cut);

    for(unsigned int it_cut=0; it_cut<cut_aliases.size(); it_cut++){
      double this_yield = 0.;

      TString cut_histname = cut_histnames.at(it_cut);
      int cut_binindex = cut_binindexes.at(it_cut);

      TH1D *hist_this_cut = (TH1D *)file->Get(cut_histname);
      if(hist_this_cut){
        this_yield = hist_this_cut->GetBinContent(cut_binindex);
      }
      hist_cutflow->SetBinContent(it_cut+1, this_yield);
    }

    hist_cutflow->SetLineColor(color);
    lg->AddEntry(hist_cutflow, samplealias, "l");

    //==== # of ee signal
    TH1D *hist_den = (TH1D *)file->Get("gen_SignalLeptonChannel");
    double n_den = hist_den->GetBinContent(1);

    hist_cutflow->Scale( 1./n_den );
    hist_cutflow->SetBinContent(1,1);
    hist_cutflow->Draw("histsame");

    cout << sample;
    for(int j=1; j<=hist_cutflow->GetXaxis()->GetNbins(); j++){
      cout << "\t" << hist_cutflow->GetBinContent(j);
    }
    cout << endl;

  }

  lg->Draw();
  c1->SaveAs("test.pdf");
  c1->Close();

}



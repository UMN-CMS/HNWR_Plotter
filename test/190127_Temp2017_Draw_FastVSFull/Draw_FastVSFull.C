#include "canvas_margin.h"
#include "mylib.h"
#include "LRSMSignalInfo.h"

void Draw_FastVSFull(){

  TString Year = "2017";

  gStyle->SetOptStat(0);

  TH1::SetDefaultSumw2(true);
  TH1::AddDirectory(kFALSE);

  TString WORKING_DIR = getenv("PLOTTER_WORKING_DIR");
  TString dataset = getenv("CATANVERSION");
  TString ENV_PLOT_PATH = getenv("PLOT_PATH");

  TString base_filepath = WORKING_DIR+"/rootfiles/"+dataset+"/HNWRSignalStudy/"+Year+"/";
  TString base_plotpath = ENV_PLOT_PATH+"/"+dataset+"/FastVSFull/"+Year+"/";

  if( !gSystem->mkdir(base_plotpath, kTRUE) ){
    cout
    << "###################################################" << endl
    << "Directoy " << base_plotpath << " is created" << endl
    << "###################################################" << endl
    << endl;
  }

  vector<TString> channels = {
    //"EE",
    "MuMu"
  };

  vector<TString> samples = {
"WR1000_N100",
"WR1000_N900",
"WR4000_N100",
"WR4000_N1000",
"WR4000_N3900",
  };
  vector<Color_t> colors = {
    kRed,
    kBlue,
    kGreen,
    kGray,
    kBlack,
  };
  vector<TString> samplealiases = {
    "WR=1000, N=100",
    "WR=1000, N=900",
    "WR=4000, N=100",
    "WR=4000, N=1000",
    "WR=4000, N=3900",
  };

  vector<TString> vars = {
    "fatjet_matched_gen_N__LSF",
    "fatjet_matched_gen_N__Pt",
    "PassUMNTag__lep_mathced_gen_priLep__Pt",
    "PassUMNTag__lep_mathced_gen_secLep__Pt",
    "fatjet_matched_gen_N__dR_lep_mathced_gen_secLep",

    "lep_mathced_gen_secLep__dR_gen_jets",
    "lep_mathced_gen_priLep__dPhi_LSFFatJet",
    "dR_AK4_secLep",
    "MaxdR_AK4_secLep",
    "MindR_AK4_secLep",

    "FatJet_LSF_Size",
    "Jet_Size",
    "LeptonVetoJet_Size",
    "FatJet_Size",
    "FatJet_LSF_Away2p0_Size", "FatJet_LSF_Away2p5_Size", "FatJet_LSF_Away2p7_Size",

  };
  vector<TString> aliases = {
    "LSF of matched fatjet",
    "p_{T} of matched fatjet",
    "p_{T} of matched primary lepton",
    "p_{T} of matched secondary lepton",
    "#DeltaR(fatjet, secLep)",

    "#DeltaR(matched AK4, matched secLep)",
    "#Delta#phi(matcehd AK8, matched priLep)",
    "#DeltaR(AK4, matched secLep)",
    "Max #DeltaR(AK4, matched secLep)",
    "Min #DeltaR(AK4, matched secLep)",

    "# of merged jet",
    "# of AK4 jet",
    "# of Leptonveto AK4 jet",
    "# of AK8 jet",
    "# of Away merged jet", "# of Away merged jet", "# of Away merged jet",
  };
  vector<int> rebins = {
    5,
    50,
    50,
    50,
    1,

    1,
    1,
    1,
    1,
    1,

    1,
    1,
    1,
    1,
    1,1,1,
  };
  vector<double> x_mins = {
    0.,
    0.,
    0.,
    0.,
    0.,

    0.,
    0.,
    0.,
    0.,
    0.,

    0.,
    0.,
    0.,
    0.,
    0.,0.,0.,
  };
  vector<double> x_maxs = {
    1.,
    3500.,
    2500.,
    1500.,
    6.,

    6.,
    4.,
    6.,
    6.,
    1.,

    4.,
    10.,
    10.,
    10.,
    4.,4.,4.,
  };

  for(unsigned int it_ch=0; it_ch<channels.size(); it_ch++){

    TString channel = channels.at(it_ch);

    cout << "channel = " << channel << endl;

    TString this_plotpath = base_plotpath+channel+"/";
    gSystem->mkdir(this_plotpath,kTRUE);

    for(unsigned it_var=0; it_var<vars.size(); it_var++){

      TString var = vars.at(it_var);
      TString alias = aliases.at(it_var);
      int rebin = rebins.at(it_var);
      double x_min = x_mins.at(it_var);
      double x_max = x_maxs.at(it_var);

      cout << "  var = " << var << endl;

      TCanvas *c1 = new TCanvas("c1", "", 600, 600);
      canvas_margin(c1);
      c1->Draw();
      TH1D *hist_dummy = NULL;

      TLegend *lg = new TLegend(0.45, 0.70, 0.9, 0.93);
      lg->SetBorderSize(0);
      lg->SetFillStyle(0);

      double y_max = -1;
      for(unsigned int it_sample=0; it_sample<samples.size(); it_sample++){

        TString sample = samples.at(it_sample);
        TString samplealias = samplealiases.at(it_sample);
        Color_t color = colors.at(it_sample);

        //cout << "    sample = " << sample << endl;

        TFile *file_Fast = new TFile(base_filepath+"/HNWRSignalStudy_WR_"+channel+"JJ_"+sample+".root");
        TH1D *hist_NoCut_Fast = (TH1D *)file_Fast->Get(channel+"/NoCut_"+channel);
        TH1D *hist_Fast = (TH1D *)file_Fast->Get(channel+"/GenStudy__"+var+"_"+channel);
        hist_Fast->Rebin(rebin);
        hist_Fast->Scale(1./hist_NoCut_Fast->GetEntries());
        hist_Fast->SetLineColor(color);
        hist_Fast->SetLineWidth(3);

        if(!hist_dummy){
          hist_dummy = (TH1D *)hist_Fast->Clone();
          hist_axis(hist_dummy);
          hist_dummy->Draw("axis");
          hist_dummy->GetXaxis()->SetRangeUser(x_min, x_max);
          hist_dummy->GetYaxis()->SetRangeUser(0., 1.);
        }

        hist_Fast->SetLineStyle(1);

        hist_Fast->Draw("histsame");

        TH1D *hist_for_legend = (TH1D *)hist_Fast->Clone();
        hist_for_legend->SetFillColor(color);
        lg->AddEntry(hist_for_legend, samplealias, "f");

        y_max = max( y_max, GetMaximum(hist_Fast,0) );

      } // END sample loop

      if(var.Contains("FatJet_LSF") && var.Contains("Size")) y_max = 1.0;
      hist_dummy->GetYaxis()->SetRangeUser(0., 1.2*y_max);
      hist_dummy->GetXaxis()->SetTitle(alias);
      hist_dummy->GetYaxis()->SetTitle("Normalized yield");

      lg->Draw();

      c1->SaveAs(this_plotpath+"/"+var+".pdf");
      c1->SaveAs(this_plotpath+"/"+var+".png");
      c1->Close();

    }

  }

}

#include "canvas_margin.h"
#include "mylib.h"
#include "LRSMSignalInfo.h"

void Draw_PYTHIAVSMADGRAPH(){

  TString Year = "2016";

  gStyle->SetOptStat(0);

  TH1::SetDefaultSumw2(true);
  TH1::AddDirectory(kFALSE);

  TString WORKING_DIR = getenv("PLOTTER_WORKING_DIR");
  TString dataset = getenv("CATANVERSION");
  TString ENV_PLOT_PATH = getenv("PLOT_PATH");

  TString base_filepath = WORKING_DIR+"/rootfiles/"+dataset+"/HNWRSignalStudy/"+Year+"/";
  TString base_plotpath = ENV_PLOT_PATH+"/"+dataset+"/PYTHIAVSMADGRAPH/"+Year+"/";

  if( !gSystem->mkdir(base_plotpath, kTRUE) ){
    cout
    << "###################################################" << endl
    << "Directoy " << base_plotpath << " is created" << endl
    << "###################################################" << endl
    << endl;
  }

  vector<TString> samples = {
"WRtoNLtoLLJJ_WR1000_N100",
"WRtoNLtoLLJJ_WR1000_N500",
"WRtoNLtoLLJJ_WR4000_N100",
"WRtoNLtoLLJJ_WR4000_N500",
"WRtoNLtoLLJJ_WR4000_N1000",
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
    "WR=1000, N=500",
    "WR=4000, N=100",
    "WR=4000, N=500",
    "WR=4000, N=1000",
  };

  vector<TString> vars = {
    "M_gen_WR",
    "M_gen_N",
    "M_gen_priLepAndN",
    "M_gen_WRStar",

    "dR_gen_WRStar",
    "dR_gen_WRStar_gen_secLep",

    "Pt_gen_priLep",
    "Pt_gen_secLep",
    "Pt_gen_N",
    "Pt_gen_WRStar",
    "Pt_gen_LeadJet",
    "Pt_gen_SubleadJet",
    "Pt_gen_priLepAndN",

    "Eta_gen_priLep",
    "Eta_gen_secLep",
    "Eta_gen_N",
    "Eta_gen_WRStar",
    "Eta_gen_LeadJet",
    "Eta_gen_SubleadJet",
    "Eta_gen_priLepAndN",
  };
  vector<TString> aliases = {
    "m(W_{R}) (GeV)",
    "m(N) (GeV)",
    "m(W_{R}) (GeV)",
    "m(dijet) (GeV)",

    "#DeltaR of the two jets",
    "#DeltaR(dijet,l_{sec})",

    "p_{T} of l_{pri} (GeV)",
    "p_{T} of l_{sec} (GeV)", 
    "p_{T} of N (GeV)",
    "p_{T} of dijet (GeV)",
    "p_{T} of j_{Lead} (GeV)",
    "p_{T} of j_{Sublead} (GeV)",
    "p_{T} of W_{R} (GeV)",

    "#eta of l_{pri}",
    "#eta of l_{sec}",
    "#eta of N",
    "#eta of dijet",
    "#eta of j_{Lead} (GeV)",
    "#eta of j_{Sublead} (GeV)",
    "#eta of W_{R}",
  };
  vector<int> rebins = {
    10,
    1,
    10,
    10,

    1,
    1,

    50,
    50,
    50,
    50,
    20,
    20,
    50,

    5,
    5,
    5,
    5,
    5,
    5,
    5,
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

    -6.,
    -6.,
    -6.,
    -6.,
    -6.,
    -6.,
    -6.,
  };
  vector<double> x_maxs = {
    4000,
    4000,
    4000,
    4000,

    6.,
    6.,

    2500.,
    1500.,
    3000.,
    2500.,
    500.,
    500.,
    1000.,

    6.,
    6.,
    6.,
    6.,
    6.,
    6.,
    6.,
  };

  TString this_plotpath = base_plotpath;
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

      cout << "    sample = " << sample << endl;

      //==== this is PYTHIA
      TFile *file_Full = new TFile(base_filepath+"/HNWRSignalStudy_FullSIM_"+sample+"_13TeV_TuneCUETP8M1.root");
      TH1D *hist_NoCut_Full = (TH1D *)file_Full->Get("NoCut");
      TH1D *hist_Full = (TH1D *)file_Full->Get(var);
      hist_Full->Rebin(rebin);
      hist_Full->Scale(1./hist_NoCut_Full->GetEntries());
      hist_Full->SetLineColor(color);
      hist_Full->SetLineWidth(3);

      //==== this is MG
      TFile *file_FullMG = new TFile(base_filepath+"/HNWRSignalStudy_FullSIM_"+sample+"_MG.root");
      TH1D *hist_NoCut_FullMG = (TH1D *)file_FullMG->Get("NoCut");
      TH1D *hist_FullMG = (TH1D *)file_FullMG->Get(var);
      hist_FullMG->Rebin(rebin);
      hist_FullMG->Scale(1./hist_NoCut_FullMG->GetEntries());
      hist_FullMG->SetLineColor(color);
      hist_FullMG->SetLineWidth(3);

      if(!hist_dummy){
        hist_dummy = (TH1D *)hist_Full->Clone();
        hist_axis(hist_dummy);
        hist_dummy->Draw("axis");
        hist_dummy->GetXaxis()->SetRangeUser(x_min, x_max);
        hist_dummy->GetYaxis()->SetRangeUser(0., 1.);

      }

      hist_Full->SetLineStyle(1);
      hist_FullMG->SetLineStyle(3);

      hist_Full->Draw("histsame");
      hist_FullMG->Draw("histsame");

      if(var=="fatjet_matched_gen_N__LSF"){
        int int_Start = hist_Full->FindBin(0.7);
        //cout << "    --> LSF Eff = " << hist_Full->Integral(int_Start,999) << "\t" << hist_FullMG->Integral(int_Start,999) << endl;
        cout << hist_Full->Integral(int_Start,999) << "\t" << hist_FullMG->Integral(int_Start,999) << endl;
      }

      TH1D *hist_for_legend = (TH1D *)hist_Full->Clone();
      hist_for_legend->SetFillColor(color);
      lg->AddEntry(hist_for_legend, samplealias, "f");

      y_max = max( y_max, GetMaximum(hist_Full,0) );
      y_max = max( y_max, GetMaximum(hist_FullMG,0) );

    } // END sample loop

    if(var.Contains("FatJet_LSF") && var.Contains("Size")) y_max = 1.0/1.2;
    hist_dummy->GetYaxis()->SetRangeUser(0., 1.2*y_max);
    if(var.Contains("PassUMNTag__lep_mathced_gen")){
      hist_dummy->GetYaxis()->SetRangeUser(0., 0.16);
    }
    hist_dummy->GetXaxis()->SetTitle(alias);
    hist_dummy->GetYaxis()->SetTitle("Normalized yield");

    lg->Draw();

    //==== Save

    if(var=="M_gen_WR" || var=="M_gen_priLepAndN"){

      hist_dummy->GetXaxis()->SetRangeUser(500., 1500);
      hist_dummy->GetYaxis()->SetRangeUser(0., 0.3);
      c1->SaveAs(this_plotpath+"/"+var+"_500_1500.pdf");

      hist_dummy->GetXaxis()->SetRangeUser(3500., 4500);
      hist_dummy->GetYaxis()->SetRangeUser(0., 0.08);
      c1->SaveAs(this_plotpath+"/"+var+"_3500_4500.pdf");
    }
    else if(var=="M_gen_N"){
      hist_dummy->GetXaxis()->SetRangeUser(99., 101.);
      hist_dummy->GetYaxis()->SetRangeUser(0., 1.1);
      c1->SaveAs(this_plotpath+"/"+var+"_99_101.pdf");

      hist_dummy->GetXaxis()->SetRangeUser(499., 501.);
      hist_dummy->GetYaxis()->SetRangeUser(0., 1.1);
      c1->SaveAs(this_plotpath+"/"+var+"_499_501.pdf");

      hist_dummy->GetXaxis()->SetRangeUser(999., 1001);
      hist_dummy->GetYaxis()->SetRangeUser(0., 1.1);
      c1->SaveAs(this_plotpath+"/"+var+"_999_1001.pdf");
    }
    else if(var=="M_gen_WRStar"){
      hist_dummy->GetXaxis()->SetRangeUser(0., 1000);
      c1->SaveAs(this_plotpath+"/"+var+"_0_1000.pdf");

      hist_dummy->GetYaxis()->SetRangeUser(0., 0.35);
      hist_dummy->GetXaxis()->SetRangeUser(0., 100);
      c1->SaveAs(this_plotpath+"/"+var+"_0_100.pdf");
    }
    else if(var=="dR_gen_WRStar"){
      c1->SaveAs(this_plotpath+"/"+var+".pdf");
    }
    else if(var=="dR_gen_WRStar_gen_secLep"){
      c1->SaveAs(this_plotpath+"/"+var+".pdf");
    }
    else if(var.Contains("Pt_")){
      c1->SaveAs(this_plotpath+"/"+var+".pdf");
    }
    else if(var.Contains("Eta_")){
      hist_dummy->GetYaxis()->SetRangeUser(0., 1.6*y_max);
      c1->SaveAs(this_plotpath+"/"+var+".pdf");
    }
    else{

    }


    c1->Close();

  }

}

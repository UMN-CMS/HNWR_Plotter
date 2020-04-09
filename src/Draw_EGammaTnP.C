#include "canvas_margin.h"
#include "mylib.h"

void Draw_EGammaTnP(int int_year=2016){

  TString Year = TString::Itoa(int_year,10);
  TString TotalLumi = "35.92 fb^{-1} (13 TeV)";
  TString TriggerList = "HLT_Ele27_WPTight_Gsf_v || HLT_Photon175_v";
  if(int_year==2017){
    Year = "2017";
    TotalLumi = "41.53 fb^{-1} (13 TeV)";
    TriggerList = "HLT_Ele35_WPTight_Gsf_v || HLT_Photon200_v";
  }
  if(int_year==2018){
    Year = "2018";
    TotalLumi = "59.74 fb^{-1} (13 TeV)";
    TriggerList = "HLT_Ele32_WPTight_Gsf_v || HLT_Photon200_v";
  }
  TLatex latex_CMSPriliminary, latex_Lumi, latex_Trigger;
  latex_CMSPriliminary.SetNDC();
  latex_Lumi.SetNDC();
  latex_Trigger.SetNDC();
  latex_CMSPriliminary.SetTextSize(0.035);
  latex_Lumi.SetTextSize(0.035);
  latex_Lumi.SetTextFont(42);
  latex_Trigger.SetTextSize(0.030);

  setTDRStyle();

  TString WORKING_DIR = getenv("PLOTTER_WORKING_DIR");
  TString dataset = getenv("CATANVERSION");
  TString ENV_PLOT_PATH = getenv("PLOT_PATH");

/*
  vector<TString> pt_legends = {
    "35 #leq p_{T} #leq 50 GeV",
    "50 #leq p_{T} #leq 100 GeV",
    "100 #leq p_{T} #leq 200 GeV",
    "200 #leq p_{T} #leq 500 GeV",
  };
*/

  TString base_filepath = WORKING_DIR+"/rootfiles/"+dataset+"/EGammaTnP/"+Year+"/";
  TString base_plotpath = ENV_PLOT_PATH+"/"+dataset+"/EGammaTnP/"+Year+"/";
  gSystem->mkdir(base_plotpath, kTRUE);

  vector<Color_t> colors = {kBlack, kGray+1, kRed +1, kRed-2, kAzure+2, kAzure-1};

  //==== y=1 line
  double x_1[2], y_1[2];
  x_1[0] = 5000;  y_1[0] = 1;
  x_1[1] = -5000;  y_1[1] = 1;
  TGraph *g1 = new TGraph(2, x_1, y_1);
  g1->SetLineWidth(3);
  g1->SetLineStyle(3);

  //==== file

  TFile *file = new TFile(base_filepath+"/egammaEffi.txt_EGM2D.root");

  //==== 2D eff hists
  //==== x : eta
  //==== y : pt

  TH2F *hist_DATA = (TH2F *)file->Get("EGamma_EffData2D");
  TH2F *hist_MC = (TH2F *)file->Get("EGamma_EffMC2D");

  //==== x-axis = pT
  //==== legends : each eta bins

  cout << "@@@@ Drawing x-axis = pT plot " << endl;

  const int N_ptbins = 4;
  double ptbins[N_ptbins+1] = {50., 60., 100, 200., 500.};

  double pt_center[N_ptbins], pt_lerr[N_ptbins], pt_rerr[N_ptbins];
  for(int i=0; i<N_ptbins; i++){
    pt_center[i] = ( ptbins[i]+ptbins[i+1] ) / 2.;
    pt_lerr[i] = pt_center[i] - ptbins[i];
    pt_rerr[i] = ptbins[i+1] - pt_center[i];
  }

  vector<TString> eta_legends = {
    "0.000 #leq #||{#eta} #leq 1.444",
    "1.444 #leq #||{#eta} #leq 1.566",
    "1.566 #leq #||{#eta} #leq 2.500",
  };

  TCanvas *c_pt = new TCanvas("c_pt", "", 800, 800);
  c_pt->Draw();
  c_pt->cd();

  TPad *c_up_pt;
  TPad *c_down_pt;
  c_up_pt = new TPad("c_pt", "", 0, 0.25, 1, 1);
  c_down_pt = new TPad("c_down_pt", "", 0, 0, 1, 0.25);

  c_up_pt->SetLogx();
  c_down_pt->SetLogx();

  canvas_margin(c_pt, c_up_pt, c_down_pt);

  c_up_pt->Draw();
  c_down_pt->Draw();

  c_up_pt->cd();
  TH1D *hist_dummy_up_pt = new TH1D("hist_dummy_up_pt", "", N_ptbins, ptbins);
  hist_dummy_up_pt->Draw("axis");
  c_down_pt->cd();
  TH1D *hist_dummy_down_pt = new TH1D("hist_dummy_down_pt", "", N_ptbins, ptbins);
  hist_dummy_down_pt->Draw("axis");
  g1->Draw("same");
  hist_axis(hist_dummy_up_pt, hist_dummy_down_pt);

  hist_dummy_up_pt->GetYaxis()->SetTitle("Efficiency");
  hist_dummy_up_pt->GetYaxis()->SetRangeUser(0.2, 1.2);
  hist_dummy_up_pt->GetYaxis()->SetLabelSize(0.035);

  hist_dummy_down_pt->GetYaxis()->SetTitle("SF");
  hist_dummy_down_pt->GetYaxis()->SetRangeUser(0.7, 1.3);
  hist_dummy_down_pt->GetXaxis()->SetMoreLogLabels();
  hist_dummy_down_pt->GetXaxis()->SetNoExponent();
  hist_dummy_down_pt->GetXaxis()->SetTitle("p_{T} (GeV)");
  hist_dummy_down_pt->GetXaxis()->SetLabelSize(0.08);
  hist_dummy_down_pt->GetYaxis()->SetLabelSize(0.09);

  TLegend *lg_eta = new TLegend(0.2, 0.3, 0.6, 0.45);
  lg_eta->SetFillStyle(0);
  lg_eta->SetBorderSize(0);

  for(unsigned int it_lg=0; it_lg<eta_legends.size(); it_lg++){

    if(it_lg==1) continue;

    TString this_legend = eta_legends.at(it_lg);

    int this_xbin = 4 + it_lg;
    cout << "[Bin validation; Legend] " << this_legend << " : " << hist_DATA->GetXaxis()->GetBinLowEdge(this_xbin) << ", " << hist_DATA->GetXaxis()->GetBinUpEdge(this_xbin) << endl;

    double this_eff_DATA[N_ptbins], this_efferr_DATA[N_ptbins];
    double this_eff_MC[N_ptbins];
    double this_SF[N_ptbins], this_SFerr[N_ptbins];

    for(int it_bin=1; it_bin<=hist_DATA->GetYaxis()->GetNbins(); it_bin++){

      if(it_lg==0){
        cout << "[Bin validation; x-axis] " << hist_DATA->GetYaxis()->GetBinLowEdge(it_bin) << ", " << hist_DATA->GetYaxis()->GetBinUpEdge(it_bin) << endl;
      }

      double eff_DATA = hist_DATA->GetBinContent( this_xbin, it_bin );
      double efferr_DATA = hist_DATA->GetBinError( this_xbin, it_bin );

      double eff_MC = hist_MC->GetBinContent( this_xbin, it_bin );
      double efferr_MC = hist_MC->GetBinError( this_xbin, it_bin );

      this_eff_DATA[it_bin-1] = eff_DATA;
      this_efferr_DATA[it_bin-1] = efferr_DATA;
      cout << efferr_DATA << endl;
      this_eff_MC[it_bin-1] = eff_MC;

      this_SF[it_bin-1] = eff_DATA/eff_MC;
      this_SFerr[it_bin-1] = efferr_DATA/eff_MC;

    }

    TGraphAsymmErrors *gr_eff_DATA = new TGraphAsymmErrors(N_ptbins, pt_center, this_eff_DATA, pt_lerr, pt_rerr, this_efferr_DATA, this_efferr_DATA);
    TGraphAsymmErrors *gr_eff_MC = new TGraphAsymmErrors(N_ptbins, pt_center, this_eff_MC, pt_lerr, pt_rerr, 0, 0);
    TGraphAsymmErrors *gr_SF = new TGraphAsymmErrors(N_ptbins, pt_center, this_SF, pt_lerr, pt_rerr, this_SFerr, this_SFerr);

    gr_eff_DATA->SetLineColor(colors.at(it_lg));
    gr_eff_DATA->SetMarkerColor(colors.at(it_lg));
    gr_eff_DATA->SetLineWidth(2);

    gr_eff_MC->SetLineColor(colors.at(it_lg));
    gr_eff_MC->SetMarkerColor(colors.at(it_lg));
    gr_eff_MC->SetMarkerSize(0);
    gr_eff_MC->SetLineWidth(2);
    gr_eff_MC->SetLineStyle(3);

    c_up_pt->cd();
    gr_eff_DATA->Draw("psame");
    gr_eff_MC->Draw("psame");

    c_down_pt->cd();
    gr_SF->SetLineColor(colors.at(it_lg));
    gr_SF->SetMarkerColor(colors.at(it_lg));
    gr_SF->SetLineWidth(2);
    gr_SF->Draw("psame");

    lg_eta->AddEntry(gr_eff_DATA, this_legend, "l");

  }

  c_pt->cd();
  lg_eta->Draw();
  latex_CMSPriliminary.DrawLatex(0.15, 0.96, "#font[62]{CMS} #font[42]{#it{#scale[0.8]{Preliminary}}}");
  latex_Lumi.DrawLatex(0.72, 0.96, TotalLumi);
  latex_Trigger.DrawLatex(0.03, 0.03, "#splitline{HEEP}{"+TriggerList+"}");

  c_pt->SaveAs(base_plotpath+"/Effs_vs_pt.pdf");
  c_pt->Close();

}

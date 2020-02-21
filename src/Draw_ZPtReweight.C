#include "canvas_margin.h"
#include "mylib.h"

void Draw_ZPtReweight(int xxx=0){

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

  setTDRStyle();

  TString WORKING_DIR = getenv("PLOTTER_WORKING_DIR");
  TString dataset = getenv("CATANVERSION");
  TString ENV_PLOT_PATH = getenv("PLOT_PATH");

  TString base_filepath = WORKING_DIR+"/rootfiles/"+dataset+"/OfficialZPtReweight/";
  TString base_plotpath = ENV_PLOT_PATH+"/"+dataset+"/OfficialZPtReweight/";
  gSystem->mkdir(base_plotpath, kTRUE);

  //==== file

  TFile *file = new TFile(base_filepath+"/Zpt_weights_"+Year+".root");

  //==== 2D eff hists
  //==== x : mass
  //==== y : pt

  TH2D *hist_Reweight = (TH2D *)file->Get("zptmass_weights");

  //==== Draw 2D

  //c1->SetTopMargin( 0.05 );
  //c1->SetBottomMargin( 0.13 );
  //c1->SetRightMargin( 0.05 );
  //c1->SetLeftMargin( 0.16 );

  TCanvas *c_2D = new TCanvas("c_2D", "", 1300, 1000);
  canvas_margin(c_2D);
  c_2D->SetLeftMargin( 0.14 );
  c_2D->SetRightMargin( 0.125 );
  c_2D->cd();
  hist_axis(hist_Reweight);

  hist_Reweight->GetYaxis()->SetLabelSize(0.03);
  //hist_Reweight->GetYaxis()->SetTitleSize(0.06);
  hist_Reweight->GetYaxis()->SetTitleOffset(1.05);
  //hist_Reweight->GetXaxis()->SetLabelSize(0.03);
  //hist_Reweight->GetXaxis()->SetTitleSize(0.05);

  hist_Reweight->GetXaxis()->SetTitle("Z boson mass (GeV)");
  hist_Reweight->GetYaxis()->SetTitle("Z boson p_{T} (GeV)");

  hist_Reweight->GetXaxis()->SetRangeUser(20.,250.);
  hist_Reweight->GetYaxis()->SetRangeUser(0.,250.);

  hist_Reweight->Draw("colz");
  c_2D->SaveAs(base_plotpath+"/Reweights_"+Year+"_2D.pdf");
  c_2D->SaveAs(base_plotpath+"/Reweights_"+Year+"_2D.png");
  c_2D->Close();

  //==== Get mass binnings

  const TArrayD *MassBinArray = hist_Reweight->GetXaxis()->GetXbins();
  const int n_MassBin = MassBinArray->GetSize()-1;
  double MassBins[n_MassBin+1];
  cout << "@@@@ Prining mass bin" << endl;
  cout << "@@@@ n_MassBin = " << n_MassBin << endl;
  for(int i=0; i<n_MassBin+1; i++){
    MassBins[i] = MassBinArray->At(i);
  }
  for(int i=0; i<n_MassBin+1; i++){
    cout << MassBins[i] << ", ";
  }
  cout << endl;

  //==== Get Pt binnings

  const TArrayD *PtBinArray = hist_Reweight->GetYaxis()->GetXbins();
  const int n_PtBin = PtBinArray->GetSize()-1;
  double PtBins[n_PtBin+1];
  cout << "@@@@ Prining Pt bin" << endl;
  cout << "@@@@ n_PtBin = " << n_PtBin << endl;
  for(int i=0; i<n_PtBin+1; i++){
    PtBins[i] = PtBinArray->At(i);
  }
  for(int i=0; i<n_PtBin+1; i++){
    cout << PtBins[i] << ", ";
  }
  cout << endl;

  TCanvas *c1 = new TCanvas("c1", "", 800, 800);
  canvas_margin(c1);
  c1->Draw();
  c1->cd();
  //c1->SetLogx();

  TH1D *hist_dummy = new TH1D("hist_dummy", "", n_MassBin, MassBins);
  hist_axis(hist_dummy);
  hist_dummy->Draw("axis");
  hist_dummy->GetYaxis()->SetRangeUser(0.7, 1.3);
  hist_dummy->GetXaxis()->SetTitle("Z boson p_{T} (GeV)");
  hist_dummy->GetYaxis()->SetTitle("Reweight");

  c1->cd();

  double y_max = -1;
  double y_min = 9999;
  vector<Color_t> colors = {
    kRed-1, kRed, kRed+1, kOrange-1, kOrange, kOrange+1, kYellow-1, kYellow, kYellow+1, kGreen-1, kGreen, kGreen+1, kBlue-1, kBlue, kBlue+1, kViolet-1, kViolet, kViolet+1, kGray, kBlack,
    kBlack,kBlack,kBlack,kBlack,kBlack,kBlack,kBlack,kBlack,kBlack,kBlack,kBlack,kBlack,kBlack,kBlack,kBlack,kBlack,kBlack,kBlack,kBlack,kBlack,kBlack,kBlack,kBlack,kBlack,kBlack,kBlack,kBlack,
    kBlack,kBlack,kBlack,kBlack,kBlack,kBlack,kBlack,kBlack,kBlack,kBlack,kBlack,kBlack,kBlack,kBlack,kBlack,kBlack,kBlack,kBlack,kBlack,kBlack,kBlack,kBlack,kBlack,kBlack,kBlack,kBlack,kBlack,
    kBlack,kBlack,kBlack,kBlack,kBlack,kBlack,kBlack,kBlack,kBlack,kBlack,kBlack,kBlack,kBlack,kBlack,kBlack,kBlack,kBlack,kBlack,kBlack,kBlack,kBlack,kBlack,kBlack,kBlack,kBlack,kBlack,kBlack,
  };
  TLegend *lg = new TLegend(0.45, 0.7, 0.90, 0.92);
  lg->SetBorderSize(0);
  lg->SetFillStyle(0);
  lg->SetNColumns(3);
  for(int i_massbin=0; i_massbin<n_MassBin; i_massbin++){

    if(i_massbin>=colors.size()) break;

    double mass_l = MassBins[i_massbin];
    double mass_r = MassBins[i_massbin+1];

    cout << "@@@@ mass [" << mass_l << ", " << mass_r << "]" << endl;

    TH1D *hist_this = new TH1D("hist_"+TString::Itoa(i_massbin,10), "", n_PtBin, PtBins);
    for(int i_ptbin=0; i_ptbin<n_PtBin; i_ptbin++){
      double this_value = hist_Reweight->GetBinContent(i_massbin+1, i_ptbin+1);
      hist_this->SetBinContent(i_ptbin+1, this_value);
      hist_this->SetBinError(i_ptbin+1, 0.);
    }
    hist_this->SetLineWidth(2);

    Color_t this_color = colors.at(int(i_massbin/3));
    Style_t this_style = 1+2*(i_massbin%3);
    hist_this->SetLineColor(this_color);
    hist_this->SetLineStyle(this_style);

    std::stringstream alias;
    alias << "[" << mass_l << ", " << mass_r << "] GeV";
    lg->AddEntry(hist_this, alias.str().c_str(), "l");

    hist_this->Draw("histsame");

    y_max = max( y_max, GetMaximum(hist_this) );
    y_min = min( y_min, GetMinimum(hist_this) );
  }

  c1->cd();
  lg->Draw();
  hist_dummy->GetYaxis()->SetRangeUser(y_min-0.2, y_max+0.2);
  //hist_dummy->GetXaxis()->SetRangeUser(20, 250);

  c1->SaveAs(base_plotpath+"/Reweights_"+Year+".pdf");
  c1->SaveAs(base_plotpath+"/Reweights_"+Year+".png");
  c1->Close();

}

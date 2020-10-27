#include "canvas_margin.h"

void run_WR(){

  setTDRStyle();

  TString form_0 = "2./3. + (1-x)*(1-x)*1./3.";
  TString form_1 = "2.*x*(1-x)*1./3.";
  TString form_2 = "x*x*1./3.";

  TF1 *f_0 = new TF1("f_0",form_0,0,1);
  TF1 *f_1 = new TF1("f_1",form_1,0,1);
  TF1 *f_2 = new TF1("f_2",form_2,0,1);
  TF1 *f_all = new TF1("f_all",form_0+"+"+form_1+"+"+form_2,0,1);

  f_0->SetLineColor(kRed);
  f_1->SetLineColor(kOrange);
  f_2->SetLineColor(kGreen);
  f_all->SetLineColor(kBlack);

  TCanvas *c1 = new TCanvas("c1", "", 600, 600);
  canvas_margin(c1);
  c1->cd();

  TH1D *hist_dummy = new TH1D("hist_dummy", "", 100, 0., 1.);
  hist_axis(hist_dummy);
  hist_dummy->Draw();

  f_0->Draw("same");
  f_1->Draw("same");
  f_2->Draw("same");
  f_all->Draw("same");

  c1->SaveAs("WR__Prob.pdf");

  //==== expected # bjet

  double beff = 0.6;

  TCanvas *c_bjet = new TCanvas("c_bjet", "", 600, 600);
  canvas_margin(c_bjet);
  c_bjet->cd();
  hist_dummy->Draw();
  TH1D *hist_bjet = new TH1D("hist_bjet", "", 4, 0., 4.);
  hist_bjet->SetBinContent( 1, f_0->Eval(beff) );
  hist_bjet->SetBinContent( 2, f_1->Eval(beff) );
  hist_bjet->SetBinContent( 3, f_2->Eval(beff) );
  hist_bjet->Draw("hist");
  c_bjet->SaveAs("WR__bjet.pdf");


}

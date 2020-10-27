#include "canvas_margin.h"

void run_ZP(){

  setTDRStyle();

  TString form_0 = "4./9. + (1-x)*(1-x)*4./9. + pow( (1-x), 4)*1./9.";
  TString form_1 = "2.*x*(1-x)*4./9. + 4.*x*pow( (1-x), 3 )*1./9.";
  TString form_2 = "x*x*4./9. + 6.*x*x*(1-x)*(1-x)*1./9.";
  TString form_3 = "4.*x*x*x*(1-x)*1./9.";
  TString form_4 = "x*x*x*x*1./9.";

  TF1 *f_0 = new TF1("f_0",form_0,0,1);
  TF1 *f_1 = new TF1("f_1",form_1,0,1);
  TF1 *f_2 = new TF1("f_2",form_2,0,1);
  TF1 *f_3 = new TF1("f_3",form_3,0,1);
  TF1 *f_4 = new TF1("f_4",form_4,0,1);
  TF1 *f_all = new TF1("f_all",form_0+"+"+form_1+"+"+form_2+"+"+form_3+"+"+form_4,0,1);

  f_0->SetLineColor(kRed);
  f_1->SetLineColor(kOrange);
  f_2->SetLineColor(kGreen);
  f_3->SetLineColor(kBlue);
  f_4->SetLineColor(kViolet);
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
  f_3->Draw("same");
  f_4->Draw("same");
  f_all->Draw("same");

  c1->SaveAs("ZP__Prob.pdf");

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
  hist_bjet->SetBinContent( 4, f_3->Eval(beff) );
  hist_bjet->SetBinContent( 5, f_4->Eval(beff) );
  hist_bjet->Draw("hist");
  c_bjet->SaveAs("ZP__bjet.pdf");


}

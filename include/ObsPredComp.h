#include "canvas_margin.h"
#include "mylib.h"

class ObsPredComp{
public:

  TCanvas *c_comp;
  TPad *c1_up;
  TPad *c1_down;

  TH1D *hist_Obs;
  TH1D *hist_Pred;
  THStack *stack_Pred;

  TString alias_Obs;
  TString alias_Pred;

  void SetXRange(double min, double max){
    UseCustomXRange = true;
    x_min = min;
    x_max = max;
  }
  bool UseCustomXRange;
  double x_min, x_max;
  TString x_title;
  bool Logy;

  TString RatioTitle;

  TString TotalLumi;

  ObsPredComp(){

    c_comp = NULL;
    c1_up = NULL;
    c1_down = NULL;

    hist_Obs = NULL;
    hist_Pred = NULL;
    stack_Pred = NULL;

    UseCustomXRange = false;
    x_min = -9999;
    x_max = -9999;
    Logy = false;

    RatioTitle = "#frac{Obs.}{Pred.}";

  }
  ~ObsPredComp(){
    c_comp->Close();
  }

  void Save(TString outputpath){
    c_comp->SaveAs(outputpath+".pdf");
    //c_comp->SaveAs(outputpath+".png");
    c_comp->Close();
  }

  void Draw(){

    //==== canvas
    c_comp = new TCanvas("c_comp", "", 800, 800);
    c1_up = new TPad("c1", "", 0, 0.25, 1, 1);
    c1_down = new TPad("c1_down", "", 0, 0, 1, 0.25);
    canvas_margin(c_comp, c1_up, c1_down);

    c1_up->Draw();
    c1_down->Draw();

    //==== up

    c1_up->cd();
    if(Logy) c1_up->SetLogy();

    //==== empty
    TH1D *hist_empty_up = NULL;
    bool UseStack = false;
    if(hist_Pred){
      hist_empty_up = (TH1D *)hist_Pred->Clone();
    }
    else if(stack_Pred){
      UseStack = true;
      hist_empty_up = (TH1D *)stack_Pred->GetStack()->Last()->Clone();
      hist_Pred = (TH1D *)stack_Pred->GetStack()->Last()->Clone();
    }
    hist_empty_up->Draw("axis");

    //==== Pred
    if(UseStack){
      stack_Pred->Draw("histsame");
      hist_Pred->SetFillColor(kOrange);
      hist_Pred->SetLineColor(kOrange);
    }
    else{
      hist_Pred->SetFillColor(kOrange);
      hist_Pred->SetLineColor(kOrange);
      hist_Pred->Draw("histsame");
    }

    TH1D *hist_Pred_err = (TH1D *)hist_Pred->Clone();
    hist_Pred_err->SetMarkerColor(0);
    hist_Pred_err->SetMarkerSize(0);
    hist_Pred_err->SetFillStyle(3013);
    hist_Pred_err->SetFillColor(kBlack);
    hist_Pred_err->SetLineColor(0);
    hist_Pred_err->Draw("sameE2");

    if(Logy){
      hist_empty_up->GetYaxis()->SetRangeUser(1., 10.*max( GetMaximum(hist_Pred), GetMaximum(hist_Obs) ) );
    }
    else{
      hist_empty_up->GetYaxis()->SetRangeUser(0., 1.2*max( GetMaximum(hist_Pred), GetMaximum(hist_Obs) ) );
    }

    if(UseCustomXRange){
      hist_empty_up->GetXaxis()->SetRangeUser(x_min,x_max);
    }

    //==== Obs
    hist_Obs->SetMarkerStyle(20);
    hist_Obs->SetMarkerSize(1.2);

    TGraphAsymmErrors *gr_Obs = new TGraphAsymmErrors(hist_Obs);
    for(int i=0; i<gr_Obs->GetN(); ++i){
      gr_Obs->SetPointEXlow(i, 0);
      gr_Obs->SetPointEXhigh(i, 0);
    }
    gr_Obs->SetLineWidth(2.0);
    gr_Obs->SetMarkerSize(0.);
    gr_Obs->SetMarkerColor(kBlack);
    gr_Obs->SetLineColor(kBlack);
    hist_Obs->Draw("phistsame");
    gr_Obs->Draw("p0same");
    hist_empty_up->Draw("axissame");

    TLegend *lg_up = new TLegend(0.6, 0.7, 0.9, 0.9);

    lg_up->AddEntry(hist_Obs, alias_Obs, "pe");
    lg_up->AddEntry(hist_Pred, alias_Pred, "f");
    lg_up->Draw();

    //==== down

    c1_down->cd();

    TH1D *tmp_ratio_point = (TH1D *)hist_Obs->Clone();
    tmp_ratio_point->Divide(hist_Pred);
    TGraphAsymmErrors *gr_ratio_point = new TGraphAsymmErrors(tmp_ratio_point);
    gr_ratio_point->SetLineWidth(2.0);
    gr_ratio_point->SetMarkerSize(0.);
    gr_ratio_point->SetLineColor(kBlack);

    TH1D *hist_empty_bottom = (TH1D *)hist_Obs->Clone();
    hist_empty_bottom->GetYaxis()->SetRangeUser(0.,2.0);
    hist_empty_bottom->SetNdivisions(504,"Y");
    hist_empty_bottom->GetYaxis()->SetRangeUser(0.5,1.5);
    hist_empty_bottom->GetXaxis()->SetTitle(x_title);
    hist_empty_bottom->GetYaxis()->SetTitle(RatioTitle);
    hist_empty_bottom->SetFillColor(0);
    hist_empty_bottom->SetFillStyle(0);
    hist_empty_bottom->SetMarkerSize(0);
    hist_empty_bottom->SetMarkerStyle(0);
    hist_empty_bottom->SetLineColor(kWhite);
    hist_empty_bottom->Draw("axis");
    hist_axis(hist_empty_up, hist_empty_bottom);

    TH1D *hist_ratio = (TH1D *)hist_Obs->Clone();
    TH1D *hist_ratio_err = (TH1D *)hist_Obs->Clone();
    for(int i=1; i<=hist_ratio->GetXaxis()->GetNbins(); i++){
      //==== FIXME for zero? how?
      if(hist_Pred->GetBinContent(i)>0){

        //==== ratio point
        //==== BinContent = Data/Bkgd
        //==== BinError = DataError/Bkgd
        hist_ratio->SetBinContent( i, hist_ratio->GetBinContent(i) / hist_Pred->GetBinContent(i) );
        hist_ratio->SetBinError ( i, hist_ratio->GetBinError(i) / hist_Pred->GetBinContent(i) );
        gr_ratio_point->SetPointEXlow(i-1, 0);
        gr_ratio_point->SetPointEXhigh(i-1, 0);

        hist_ratio_err->SetBinContent( i, 1.);
        hist_ratio_err->SetBinError( i, hist_Pred->GetBinError(i) / hist_Pred->GetBinContent(i) );


      }
      else{
        hist_ratio->SetBinContent( i, 0 );
        hist_ratio->SetBinError ( i, 0 );
        gr_ratio_point->SetPoint(i-1, 0, 0);
        gr_ratio_point->SetPointEYlow(i-1, 0);
        gr_ratio_point->SetPointEXlow(i-1, 0);
        gr_ratio_point->SetPointEYhigh(i-1, 0);
        gr_ratio_point->SetPointEXhigh(i-1, 0);
        hist_ratio_err->SetBinContent( i, 1.);
        hist_ratio_err->SetBinError( i, 0.);
      }
    }

    hist_ratio_err->SetFillColor(kGray);
    hist_ratio_err->SetMarkerSize(0);
    hist_ratio_err->SetMarkerStyle(0);
    hist_ratio_err->SetLineColor(kWhite);
    hist_ratio_err->Draw("E2same");

    hist_ratio->Draw("p9histsame");
    gr_ratio_point->Draw("p0same");
    hist_empty_bottom->Draw("axissame");

    c_comp->cd();
    TLatex latex_CMSPriliminary, latex_Lumi;
    latex_CMSPriliminary.SetNDC();
    latex_Lumi.SetNDC();
    latex_CMSPriliminary.SetTextSize(0.035);
    latex_CMSPriliminary.DrawLatex(0.15, 0.96, "#font[62]{CMS} #font[42]{#it{#scale[0.8]{Preliminary}}}");

    latex_Lumi.SetTextSize(0.035);
    latex_Lumi.SetTextFont(42);
    latex_Lumi.DrawLatex(0.72, 0.96, TotalLumi);

  }

};

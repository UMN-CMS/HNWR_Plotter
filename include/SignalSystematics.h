#ifndef SignalSystematics_h
#define SignalSystematics_h

#include "canvas_margin.h"
#include "mylib.h"

class SignalSystematics{

public:

  TFile *file;

  int n_rebin;
  bool UseCustomRebin;
  bool DoDebug;
  TString region;

  //==== Central Histogram

  TH1D *hist_Central;

  //==== Denominator values

  double *DenValues_Scale;
  double *DenValues_PDFError;
  double *DenValues_AlphaS;

  //==== histograms

  TH1D *hist_Central_Num;
  TH1D *hist_ScaleUp;
  TH1D *hist_ScaleDn;

  TH1D *hist_PDFError;
  TH1D *hist_PDFErrorUp;
  TH1D *hist_PDFErrorDn;

  TH1D *hist_AlphaS;
  TH1D *hist_AlphaSUp;
  TH1D *hist_AlphaSDn;

  //==== output plot
  bool DrawPlot;
  TString outputdir;

  SignalSystematics(){

    file = NULL;

    n_rebin = 20;
    UseCustomRebin = true;
    DoDebug = false;
    region = "";

    hist_Central = NULL;

    DenValues_Scale = new double[7];
    DenValues_PDFError = new double[101];
    DenValues_AlphaS = new double[2];

    hist_Central_Num = NULL;
    hist_ScaleUp = NULL;
    hist_ScaleDn = NULL;

    hist_PDFError = NULL;
    hist_PDFErrorUp = NULL;
    hist_PDFErrorDn = NULL;

    hist_AlphaS = NULL;
    hist_AlphaSUp = NULL;
    hist_AlphaSDn = NULL;

    DrawPlot = false;
    outputdir = "";

  }

  void Run(){

    gStyle->SetOptStat(0);

    if(DoDebug){
      cout << "[SignalSystematics] Getting denominator values" << endl;
    }

    //==== TFile

    //TFile *file = new TFile(filepath);

    //==== Den vars

    TDirectory *dir_Den = (TDirectory *)file->Get("XsecSyst_Den");

    //==== Scale
    int ScaleIDs[7] = {
      1001, // 1) R=1.0 F = 1.0
      1006, // 2) R=2.0 F = 1.0
      1011, // 3) R=0.5 F = 1.0
      1016, // 4) R=1.0 F = 2.0
      1021, // 5) R=2.0 F = 2.0
      //1026, // 6) R=0.5 F = 2.0
      1031, // 7) R=1.0 F = 0.5
      //1036, // 8) R=2.0 F = 0.5
      1041  // 9) R=0.5 F = 0.5
    };
    Color_t colors[7] = {
      kBlack,
      kRed,
      kGreen,
      kBlue,
      kMagenta,
      kCyan,
      kSpring,
    };
    for(int i=0; i<7; i++) ScaleIDs[i] = ScaleIDs[i]-1001;

    if(DoDebug) cout << "[SignalSystematics] @@@@ Scale " << endl;
    for(int i=0; i<7; i++){
      // PDFWeights_Scale_23

      TString histname = "PDFWeights_Scale_"+TString::Itoa(ScaleIDs[i],10);
      TH1D *hist = (TH1D *)dir_Den->Get(histname+"_XsecSyst_Den");
      if(DoDebug) cout << "[SignalSystematics] i = " << i << " : " << hist->GetBinContent(1) << endl;

      DenValues_Scale[i] = hist->GetBinContent(1);

    }

    //==== PDF error

    if(DoDebug) cout << "[SignalSystematics] @@@@ PDF Error" << endl;
    TH1D *hist_DenValue_PDFErrorNominal = (TH1D *)dir_Den->Get("PDFWeights_Error_0_XsecSyst_Den");
    double DenValue_PDFErrorNominal = hist_DenValue_PDFErrorNominal->GetBinContent(1);
    double tmp = 0.;
    for(int i=0; i<=100; i++){
      // PDFWeights_Error_20

      TString histname = "PDFWeights_Error_"+TString::Itoa(i,10);
      TH1D *hist = (TH1D *)dir_Den->Get(histname+"_XsecSyst_Den");
      if(DoDebug) cout << "[SignalSystematics] i = " << i << " : " << hist->GetBinContent(1) << endl;

      double this_diff = hist->GetBinContent(1)-DenValue_PDFErrorNominal;
      tmp += this_diff*this_diff;

      DenValues_PDFError[i] = hist->GetBinContent(1);

    }
    if(DoDebug) cout << "[SignalSystematics] ----> Error = " << sqrt(tmp) << endl;

    //==== AlphaS

    if(DoDebug) cout << "[SignalSystematics] @@@@ AlphaS" << endl;
    for(int i=0; i<2; i++){
      // PDFWeights_AlphaS_20

      TString histname = "PDFWeights_AlphaS_"+TString::Itoa(i,10);
      TH1D *hist = (TH1D *)dir_Den->Get(histname+"_XsecSyst_Den");
      if(DoDebug) cout << "[SignalSystematics] i = " << i << " : " << hist->GetBinContent(1) << endl;
      DenValues_AlphaS[i] = hist->GetBinContent(1);

    }
    double AlphaSError_Den = fabs(DenValues_AlphaS[0]-DenValues_AlphaS[1])/2.;
    if(DoDebug) cout << "[SignalSystematics] ----> Error = " << AlphaSError_Den << endl;


    //===========================================================================================




    //==== Num

    if(DoDebug){
      cout << "[SignalSystematics] Getting eff values" << endl;
    }

    TDirectory *dir_Num = (TDirectory *)file->Get("XsecSyst_Num_"+region);

    //============
    //==== Scale
    //============

    if(DoDebug) cout << "[SignalSystematics] @@@@ Scale " << endl;

    TCanvas *c_Scale = new TCanvas("c_Scale", "", 600, 600);
    canvas_margin(c_Scale);
    c_Scale->cd();

    //==== central
    hist_Central_Num = (TH1D *)dir_Num->Get("PDFWeights_Scale_0_XsecSyst_Num_"+region);
    if(UseCustomRebin) hist_Central_Num = RebinWRMass(hist_Central_Num, region);
    else               hist_Central_Num->Rebin(n_rebin);
    hist_Central_Num->SetLineWidth(2);
    hist_Central_Num->Scale(1./DenValues_Scale[0]);
    //==== FIXME ee : mm = 1 : 1
    hist_Central_Num->Scale(2.);

    //==== dummy
    TH1D *hist_dummy = (TH1D *)hist_Central_Num->Clone();
    hist_dummy->Draw("axis");
    hist_axis(hist_dummy);

    hist_Central_Num->SetLineColor(kBlack);
    hist_Central_Num->Draw("histsame");

    hist_ScaleUp = (TH1D *)hist_Central_Num->Clone();
    hist_ScaleDn = (TH1D *)hist_Central_Num->Clone();
    const int n_xbin = hist_Central_Num->GetXaxis()->GetNbins();

    double y_max = GetMaximum(hist_Central_Num);
    map< int, vector<double> > ScalesToBinValues;
    for(int i=1; i<7; i++){

      TString histname = "PDFWeights_Scale_"+TString::Itoa(ScaleIDs[i],10)+"_XsecSyst_Num_"+region;
      TH1D *hist = (TH1D *)dir_Num->Get(histname);
      if(UseCustomRebin) hist = RebinWRMass(hist, region);
      else               hist->Rebin(n_rebin);
      hist->SetLineWidth(2);
      hist->Scale(1./DenValues_Scale[i]);

      //==== FIXME ee : mm = 1 : 1
      hist->Scale(2.);
      if(DoDebug) cout << "[SignalSystematics] hist->Integral() = " << hist->Integral() << endl;

      y_max = max( y_max, GetMaximum(hist));

      hist->SetLineColor(colors[i]);
      hist->SetLineWidth(3);
      hist->SetLineStyle(3);
      hist->Draw("histsame");

      vector<double> values;
      for(int x=1; x<=hist->GetXaxis()->GetNbins(); x++){
        values.push_back( hist->GetBinContent(x) );
      }
      ScalesToBinValues[i] = values;

    }
    for(int x=1; x<=hist_Central_Num->GetXaxis()->GetNbins(); x++){

      //==== x : bincontent
      //==== y : vector element index

      int y = x-1;

      double bin_central = hist_Central_Num->GetBinContent(x);

      double binmax = bin_central;
      double binmin = bin_central;
      for(int i=1; i<7; i++){
        binmax = max( binmax, ScalesToBinValues[i].at(y) );
        binmin = min( binmin, ScalesToBinValues[i].at(y) );
      }

      hist_ScaleUp->SetBinContent(x, binmax);
      hist_ScaleDn->SetBinContent(x, binmin);

    }

    if(region.Contains("Resolved")) hist_dummy->GetXaxis()->SetTitle("m_{lljj} (GeV)");
    else hist_dummy->GetXaxis()->SetTitle("m_{lJ} (GeV)");

    hist_dummy->GetYaxis()->SetTitle("Efficiency / 200 GeV");
    hist_dummy->GetYaxis()->SetRangeUser(0., 1.2*y_max);
    hist_dummy->Draw("histsameaxis");

    hist_ScaleUp->SetLineWidth(2);
    hist_ScaleUp->SetLineStyle(1);
    hist_ScaleUp->SetLineColor(kGray);

    hist_ScaleDn->SetLineWidth(2);
    hist_ScaleDn->SetLineStyle(1);
    hist_ScaleDn->SetLineColor(kGray);

    hist_ScaleUp->Draw("histsame");
    hist_ScaleDn->Draw("histsame");

    if(DrawPlot) c_Scale->SaveAs(outputdir+"/Scale_"+region+".pdf");
    c_Scale->Close();

    //================
    //==== PDF Error
    //================

    if(DoDebug) cout << "[SignalSystematics] @@@@ PDF Error" << endl;

    TCanvas *c_PDFError = new TCanvas("c_PDFError", "", 600, 600);
    canvas_margin(c_PDFError);
    c_PDFError->cd();

    hist_PDFError = (TH1D *)hist_Central_Num->Clone();
    hist_PDFErrorUp = (TH1D *)hist_Central_Num->Clone();
    hist_PDFErrorDn = (TH1D *)hist_Central_Num->Clone();
    map< int, vector<double> > PDFErrorSetToBinValues;

    for(int i=0; i<=100; i++){

      TString histname = "PDFWeights_Error_"+TString::Itoa(i,10)+"_XsecSyst_Num_"+region;
      TH1D *hist = (TH1D *)dir_Num->Get(histname);
      if(UseCustomRebin) hist = RebinWRMass(hist, region);
      else               hist->Rebin(n_rebin);
      hist->SetLineWidth(2);
      hist->Scale(1./DenValues_PDFError[i]);

      vector<double> values;
      for(int x=1; x<=hist->GetXaxis()->GetNbins(); x++){
        values.push_back( hist->GetBinContent(x) );
      }

      PDFErrorSetToBinValues[i] = values;

    } // END Loop PDFError set

    for(int x=1; x<=hist_Central_Num->GetXaxis()->GetNbins(); x++){

      //==== x : bincontent
      //==== y : vector element index

      int y = x-1;
      double bin_central = PDFErrorSetToBinValues[0].at(y);
      double diff = 0;

      for(int j=0; j<=100; j++){
        double this_diff = PDFErrorSetToBinValues[j].at(y)-bin_central;
        diff += this_diff*this_diff;
      }
      //cout << bin_central << "\t" << sqrt(diff) << endl;
      hist_PDFError->SetBinContent(x, 2.*bin_central);
      hist_PDFError->SetBinError(x, 2.*sqrt(diff));

      hist_PDFErrorUp->SetBinContent(x, 2.* (bin_central+sqrt(diff)) );
      hist_PDFErrorDn->SetBinContent(x, 2.* max(0., (bin_central-sqrt(diff)) ) );

      //cout << "bin " << x << " : " <<hist_PDFError->GetBinContent(x) << endl;


    }

    hist_axis(hist_PDFError);
    hist_PDFError->SetLineColor(kBlack);
    hist_PDFError->SetLineWidth(1);
    hist_PDFError->GetYaxis()->SetRangeUser(0., 1.2*y_max);
    hist_PDFError->Draw("histsamee1");

    hist_PDFErrorUp->SetLineStyle(3);
    hist_PDFErrorUp->SetLineColor(kRed);

    hist_PDFErrorDn->SetLineStyle(3);
    hist_PDFErrorDn->SetLineColor(kBlue);

    hist_PDFErrorUp->Draw("histsame");
    hist_PDFErrorDn->Draw("histsame");

    if(DrawPlot) c_PDFError->SaveAs(outputdir+"/PDFError_"+region+".pdf");
    c_PDFError->Close();

    //==== AlphaS

    TCanvas *c_AlphaS = new TCanvas("c_AlphaS", "", 600, 600);
    canvas_margin(c_AlphaS);
    c_AlphaS->cd();

    hist_AlphaS = (TH1D *)hist_Central_Num->Clone();
    hist_AlphaSUp = (TH1D *)hist_Central_Num->Clone();
    hist_AlphaSDn = (TH1D *)hist_Central_Num->Clone();

    map< int, vector<double> > AlphaSToBinValues;

    for(int i=0; i<2; i++){

      TString histname = "PDFWeights_AlphaS_"+TString::Itoa(i,10)+"_XsecSyst_Num_"+region;
      TH1D *hist = (TH1D *)dir_Num->Get(histname);
      if(UseCustomRebin) hist = RebinWRMass(hist, region);
      else               hist->Rebin(n_rebin);
      hist->SetLineWidth(2);
      hist->Scale(1./DenValues_AlphaS[i]);

      vector<double> values;
      for(int x=1; x<=hist->GetXaxis()->GetNbins(); x++){
        values.push_back( hist->GetBinContent(x) );
      }

      AlphaSToBinValues[i] = values;

    } // END Loop AlphaS

    for(int x=1; x<=hist_Central_Num->GetXaxis()->GetNbins(); x++){

      //==== x : bincontent
      //==== y : vector element index

      int y = x-1;
      //==== already doubled
      double bin_central = hist_Central_Num->GetBinContent(x);

      //==== not yet doubles
      double alphaSUp = 2.*AlphaSToBinValues[0].at(y)-bin_central;
      double alphaSDn = 2.*AlphaSToBinValues[1].at(y)-bin_central;

      double this_err = fabs( (alphaSUp-alphaSDn)/2. );

      hist_AlphaS->SetBinContent(x, bin_central);
      hist_AlphaS->SetBinError(x, this_err);

      hist_AlphaSUp->SetBinContent(x, bin_central+this_err );
      hist_AlphaSDn->SetBinContent(x, max(0., bin_central-this_err) );

      if(DoDebug) cout << "[SignalSystematics] bin " << x << " : " << bin_central << "\t" << this_err << endl;

    }

    hist_axis(hist_AlphaS);
    hist_AlphaS->SetLineColor(kBlack);
    hist_AlphaS->SetLineWidth(2);
    hist_AlphaS->GetYaxis()->SetRangeUser(0., 1.2*y_max);
    hist_AlphaS->Draw("histsamee1");

    hist_AlphaSUp->SetLineStyle(3);
    hist_AlphaSUp->SetLineColor(kRed);

    hist_AlphaSDn->SetLineStyle(3);
    hist_AlphaSDn->SetLineColor(kBlue);

    hist_AlphaSUp->Draw("histsame");
    hist_AlphaSDn->Draw("histsame");

    if(DrawPlot) c_AlphaS->SaveAs(outputdir+"/AlphaS_"+region+".pdf");
    c_AlphaS->Close();

    //==== Now make output

    for(int i=1; i<=hist_Central->GetXaxis()->GetNbins(); i++){
      double central_value = hist_Central->GetBinContent(i);

      double central_efF_value = hist_Central_Num->GetBinContent(i);

      //==== sometimes central_efF_value=1E-20.. and gives wrong result
      if(central_efF_value>1E-7){

        hist_ScaleUp->SetBinContent( i, central_value * hist_ScaleUp->GetBinContent(i) / central_efF_value );
        hist_ScaleDn->SetBinContent( i, central_value * hist_ScaleDn->GetBinContent(i) / central_efF_value );

        hist_PDFErrorUp->SetBinContent( i, central_value * hist_PDFErrorUp->GetBinContent(i) / central_efF_value );      
        hist_PDFErrorDn->SetBinContent( i, central_value * hist_PDFErrorDn->GetBinContent(i) / central_efF_value );

        hist_AlphaSUp->SetBinContent( i, central_value * hist_AlphaSUp->GetBinContent(i) / central_efF_value );      
        hist_AlphaSDn->SetBinContent( i, central_value * hist_AlphaSDn->GetBinContent(i) / central_efF_value );

      }
      else{

        hist_ScaleUp->SetBinContent( i, 0. );
        hist_ScaleDn->SetBinContent( i, 0. );

        hist_PDFErrorUp->SetBinContent( i, 0. );
        hist_PDFErrorDn->SetBinContent( i, 0. );

        hist_AlphaSUp->SetBinContent( i, 0. );
        hist_AlphaSDn->SetBinContent( i, 0. );

      }

    }

/*
    //==========
    //==== All
    //==========

    TCanvas *c_All = new TCanvas("c_All", "", 600, 600);
    canvas_margin(c_All);
    c_All->cd();

    hist_Central_Num->Draw("hist");
    hist_axis(hist_Central_Num);
    hist_Central_Num->SetLineWidth(3);
    hist_Central_Num->SetLineColor(kBlack);

    hist_ScaleUp->SetLineColor(kRed);
    hist_ScaleDn->SetLineColor(kRed);
    hist_AlphaS->SetLineColor(kGreen);

    hist_PDFError->SetMarkerColor(0);
    hist_PDFError->SetMarkerSize(0);
    hist_PDFError->SetFillStyle(3013);
    hist_PDFError->SetFillColor(kBlack);
    hist_PDFError->SetLineColor(0);

    hist_ScaleUp->Draw("histsame");
    hist_ScaleDn->Draw("histsame");
    hist_PDFError->Draw("sameE2");
    //hist_AlphaS->Draw("e2same");

    if(DrawPlot) c_All->SaveAs(outputdir+"/All_"+region+".pdf");
    c_All->Close();
*/

  }

};

#endif

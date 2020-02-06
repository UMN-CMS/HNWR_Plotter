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
  bool isReplica;
  TString region;

  double ChannelFrac;

  //==== Central Histogram

  TH1D *hist_Central;

  //==== Denominator values

  double *DenValues_Scale;
  double *DenValues_PDFError;
  double *DenValues_AlphaS;

  double xsec_ScaleSyst;
  double xsec_PDFErrorSyst;
  double xsec_AlphaSSyst;

  //==== PDF Error set

  int NPDFErrorSet;

  //==== histograms

  TH1D *hist_Central_Num;
  TH1D *hist_ScaleUp;
  TH1D *hist_ScaleDn;
  TH1D *hist_ScaleIntegral;

  TH1D *hist_PDFError;
  TH1D *hist_PDFErrorUp;
  TH1D *hist_PDFErrorDn;

  TH1D *hist_AlphaS;
  TH1D *hist_AlphaSUp;
  TH1D *hist_AlphaSDn;

  //==== output plot
  bool DrawPlot;
  TString outputdir;
  TString histPrefix;

  SignalSystematics(int nPDFerrset=101){

    file = NULL;

    n_rebin = 20;
    UseCustomRebin = true;
    DoDebug = false;
    isReplica = false;
    region = "";

    hist_Central = NULL;

    NPDFErrorSet = nPDFerrset;
    const int cnPDFerrset = nPDFerrset;

    DenValues_Scale = new double[7];
    DenValues_PDFError = new double[cnPDFerrset];
    DenValues_AlphaS = new double[2];

    xsec_ScaleSyst = -999;
    xsec_PDFErrorSyst = -999;
    xsec_AlphaSSyst = -999;

    hist_Central_Num = NULL;
    hist_ScaleUp = NULL;
    hist_ScaleDn = NULL;
    hist_ScaleIntegral = NULL;

    hist_PDFError = NULL;
    hist_PDFErrorUp = NULL;
    hist_PDFErrorDn = NULL;

    hist_AlphaS = NULL;
    hist_AlphaSUp = NULL;
    hist_AlphaSDn = NULL;

    DrawPlot = false;
    outputdir = "";
    histPrefix = "";

  }

  void Run(){

    gStyle->SetOptStat(0);

    if(DoDebug){
      cout << "[SignalSystematics] Getting denominator values" << endl;
    }

    //===========================================================================================
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

      xsec_ScaleSyst = max( xsec_ScaleSyst, fabs( DenValues_Scale[i] - DenValues_Scale[0] ) );

    }

    //==== PDF error

    if(DoDebug) cout << "[SignalSystematics] @@@@ PDF Error" << endl;
    TH1D *hist_DenValue_PDFErrorNominal = (TH1D *)dir_Den->Get("PDFWeights_Error_0_XsecSyst_Den");
    double DenValue_PDFErrorNominal = hist_DenValue_PDFErrorNominal->GetBinContent(1);
    if(isReplica){
      DenValue_PDFErrorNominal = 0.;
      for(int i=1; i<NPDFErrorSet; i++){
        TString histname = "PDFWeights_Error_"+TString::Itoa(i,10);
        TH1D *hist = (TH1D *)dir_Den->Get(histname+"_XsecSyst_Den");
        DenValue_PDFErrorNominal += hist->GetBinContent(1);
      }
      DenValue_PDFErrorNominal = DenValue_PDFErrorNominal/NPDFErrorSet;
      if(DoDebug) cout << "[SignalSystematics] @@@@ isReplica, average = " << DenValue_PDFErrorNominal << endl;
    }

    double tmp = 0.;
    for(int i=0; i<NPDFErrorSet; i++){
      // PDFWeights_Error_20

      if(isReplica && i==0) continue;

      TString histname = "PDFWeights_Error_"+TString::Itoa(i,10);
      TH1D *hist = (TH1D *)dir_Den->Get(histname+"_XsecSyst_Den");

      double this_diff = hist->GetBinContent(1)-DenValue_PDFErrorNominal;
      tmp += this_diff*this_diff;

      DenValues_PDFError[i] = hist->GetBinContent(1);

      if(DoDebug) cout << "[SignalSystematics] i = " << i << " : " << hist->GetBinContent(1) << "\ttmp2 = " << tmp << endl;

    }
    if(isReplica) tmp = tmp/(NPDFErrorSet-1);
    if(DoDebug) cout << "[SignalSystematics] ----> Error = " << sqrt(tmp) << endl;
    xsec_PDFErrorSyst = sqrt(tmp);

    //==== AlphaS

    if(DoDebug) cout << "[SignalSystematics] @@@@ AlphaS" << endl;
    for(int i=0; i<2; i++){
      // PDFWeights_AlphaS_20

      TString histname = "PDFWeights_AlphaS_"+TString::Itoa(i,10);
      TH1D *hist = (TH1D *)dir_Den->Get(histname+"_XsecSyst_Den");
      if(DoDebug) cout << "[SignalSystematics] i = " << i << " : " << hist->GetBinContent(1) << endl;
      DenValues_AlphaS[i] = hist->GetBinContent(1);

    }
    xsec_AlphaSSyst = fabs(DenValues_AlphaS[0]-DenValues_AlphaS[1])/2.;
    if(DoDebug) cout << "[SignalSystematics] ----> Error = " << xsec_AlphaSSyst << endl;


    //===========================================================================================
    //==== Num

    if(DoDebug){
      cout << "[SignalSystematics] Getting eff values" << endl;
    }

    TDirectory *dir_Num = (TDirectory *)file->Get("XsecSyst_Num_"+region);

    //==== central

    hist_Central_Num = (TH1D *)dir_Num->Get("PDFWeights_Scale_0_XsecSyst_Num_"+region);
    if(UseCustomRebin) hist_Central_Num = RebinWRMass(hist_Central_Num, region);
    else               hist_Central_Num->Rebin(n_rebin);
    hist_Central_Num->SetLineWidth(2);
    hist_Central_Num->Scale(1./DenValues_Scale[0]);
    hist_Central_Num->Scale(1./ChannelFrac);

    //============
    //==== Scale
    //============

    if(DoDebug) cout << "[SignalSystematics] @@@@ Scale " << endl;

    TCanvas *c_Scale = new TCanvas("c_Scale", "", 600, 600);
    canvas_margin(c_Scale);
    c_Scale->cd();

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

    double integral_Central = hist_Central_Num->Integral();
    double integral_Scale_Max(-1), integral_Scale_Min(99999999);

    for(int i=1; i<7; i++){

      TString histname = "PDFWeights_Scale_"+TString::Itoa(ScaleIDs[i],10)+"_XsecSyst_Num_"+region;
      TH1D *hist = (TH1D *)dir_Num->Get(histname);
      if(UseCustomRebin) hist = RebinWRMass(hist, region);
      else               hist->Rebin(n_rebin);
      hist->SetLineWidth(2);
      hist->Scale(1./DenValues_Scale[i]);
      hist->Scale(1./ChannelFrac);
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

      integral_Scale_Max = max( integral_Scale_Max, hist->Integral() );
      integral_Scale_Min = min( integral_Scale_Min, hist->Integral() );

    }
    double integral_Scale_Up = fabs(integral_Central-integral_Scale_Max)/integral_Central;
    double integral_Scale_Dn = fabs(integral_Central-integral_Scale_Min)/integral_Central;
    if(DoDebug){
      cout << "@@@@ Scale Integral : " << integral_Central << "\t" << integral_Scale_Max << "\t" << integral_Scale_Min << endl;
      cout << "@@@@ Scale Up = " << integral_Scale_Up << endl;
      cout << "@@@@ Scale Dn = " << integral_Scale_Dn << endl;
      cout << "@@@@ Scale integral syst = " << max(integral_Scale_Up,integral_Scale_Dn) << endl;
    }
    hist_ScaleIntegral = new TH1D("hist_ScaleIntegral", "", 1, 0., 1.);
    hist_ScaleIntegral->SetBinContent(1, max(integral_Scale_Up,integral_Scale_Dn));
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

    if(DrawPlot) c_Scale->SaveAs(outputdir+"/"+histPrefix+"Scale_"+region+".pdf");
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

    for(int i=0; i<NPDFErrorSet; i++){

      TString histname = "PDFWeights_Error_"+TString::Itoa(i,10)+"_XsecSyst_Num_"+region;
      TH1D *hist = (TH1D *)dir_Num->Get(histname);
      if(UseCustomRebin) hist = RebinWRMass(hist, region);
      else               hist->Rebin(n_rebin);
      hist->SetLineWidth(2);
      hist->Scale(1./DenValues_PDFError[i]);
      //==== we will calculate diff, so 1./ChannelFrac later

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

      if(isReplica){
        bin_central = 0;
        for(int j=1; j<NPDFErrorSet; j++){
          bin_central += PDFErrorSetToBinValues[j].at(y);
        }
        bin_central = bin_central/NPDFErrorSet;
      }

      if(DoDebug) cout << "bin "<< x << " : central = " << bin_central << endl;

      //if(DoDebug) cout << "bin "<< x << " : central = " << bin_central << endl;

      for(int j=0; j<NPDFErrorSet; j++){
        if(isReplica && j==0) continue;
        double this_diff = PDFErrorSetToBinValues[j].at(y)-bin_central;
        if(DoDebug) cout << "  PDFSet " << j << " : " << PDFErrorSetToBinValues[j].at(y) << " -> diff = " << this_diff << " -> curretn sum diff2 = " << diff << endl;
        diff += this_diff*this_diff;
      }
      if(isReplica) diff = diff/(NPDFErrorSet-1);
      hist_PDFError->SetBinContent(x, bin_central/ChannelFrac);
      hist_PDFError->SetBinError(x, sqrt(diff)/ChannelFrac);

      hist_PDFErrorUp->SetBinContent(x, (bin_central+sqrt(diff))/ChannelFrac );
      hist_PDFErrorDn->SetBinContent(x, max(0.000001, (bin_central-sqrt(diff))/ChannelFrac ) );

      if(DoDebug) cout << "[SignalSystematics] bin " << x << " : " <<hist_PDFError->GetBinContent(x) << ", error = " << hist_PDFError->GetBinError(x) << endl;


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

    if(DrawPlot) c_PDFError->SaveAs(outputdir+"/"+histPrefix+"PDFError_"+region+".pdf");
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
      hist->Scale(1./ChannelFrac);

      vector<double> values;
      for(int x=1; x<=hist->GetXaxis()->GetNbins(); x++){
        values.push_back( hist->GetBinContent(x) );
      }

      AlphaSToBinValues[i] = values;

    } // END Loop AlphaS

    if(DoDebug) cout << "[SignalSystematics] @@@@ PDF AlphaS" << endl;
    for(int x=1; x<=hist_Central_Num->GetXaxis()->GetNbins(); x++){

      //==== x : bincontent
      //==== y : vector element index

      int y = x-1;

      double bin_central = hist_Central_Num->GetBinContent(x);

      double alphaSUp = AlphaSToBinValues[0].at(y)-bin_central;
      double alphaSDn = AlphaSToBinValues[1].at(y)-bin_central;

      double this_err = fabs( (alphaSUp-alphaSDn)/2. );

      hist_AlphaS->SetBinContent(x, bin_central);
      hist_AlphaS->SetBinError(x, this_err);

      hist_AlphaSUp->SetBinContent(x, bin_central+this_err );
      hist_AlphaSDn->SetBinContent(x, max(0.000001, bin_central-this_err) );

      if(DoDebug) cout << "[SignalSystematics] bin " << x << " : " << AlphaSToBinValues[0].at(y) << "\t" << AlphaSToBinValues[1].at(y) << " --> " << bin_central << "\t" << this_err << endl;

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

    if(DrawPlot) c_AlphaS->SaveAs(outputdir+"/"+histPrefix+"AlphaS_"+region+".pdf");
    c_AlphaS->Close();

    //==== Now make output

    //==== Lets modify <=0 bincontent to 0.000001
    for(int i=1; i<=hist_Central->GetXaxis()->GetNbins(); i++){
      if(hist_Central->GetBinContent(i)<0) hist_Central->SetBinContent(i, 0.000001);
    }

    for(int i=1; i<=hist_Central->GetXaxis()->GetNbins(); i++){
      double central_value = hist_Central->GetBinContent(i);

      double central_efF_value = hist_Central_Num->GetBinContent(i);

      //==== sometimes central_efF_value=1E-20.. and gives wrong result
      if(central_efF_value>1E-7){

        hist_ScaleUp->SetBinContent( i, central_value * hist_ScaleUp->GetBinContent(i) / central_efF_value );
        hist_ScaleDn->SetBinContent( i, central_value * hist_ScaleDn->GetBinContent(i) / central_efF_value );

        double this_PDFError = hist_PDFError->GetBinError(i)/hist_PDFError->GetBinContent(i);
        hist_PDFErrorUp->SetBinContent( i, central_value * (1+this_PDFError) );
        hist_PDFErrorDn->SetBinContent( i, central_value * max(0.000001, (1-this_PDFError)) ); //TODO DEBUGGING

        if(DoDebug){
          double x_l = hist_Central->GetXaxis()->GetBinLowEdge(i);
          double x_r = hist_Central->GetXaxis()->GetBinUpEdge(i);
          //printf("[%f,%f] : %f\t%f\t%f\t%f\n",x_l,x_r,central_value, hist_PDFErrorUp->GetBinContent(i), hist_PDFErrorDn->GetBinContent(i), central_efF_value);
          printf("[%f,%f] : %f\t%f\n",x_l,x_r,central_value,this_PDFError);
        }


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

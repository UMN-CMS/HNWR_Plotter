#include "canvas_margin.h"
#include "LRSMSignalInfo.h"
#include "SignalSystematics.h"

void Draw_PDFError(int Year, TString PDFName){

  //TString PDFName = "NNPDF31_nnlo_as_0118_mc_hessian_pdfas";
  //bool isReplica = false;

  bool isReplica = PDFName=="NNPDF23_lo_as_0130_qed" || PDFName=="NNPDF31_nnlo_as_0118_mc" || PDFName=="PDF4LHC15_nnlo_mc";

  //gErrorIgnoreLevel = kFatal;

  bool Usekfactor = true;

  double signal_scale = 0.1;

  TString TotalLumi = "";
  if(Year==2016){
    TotalLumi = "35.92 fb^{-1} (13 TeV)";
  }
  else if(Year==2017){
    TotalLumi = "41.53 fb^{-1} (13 TeV)";
  }
  else if(Year==2018){
    TotalLumi = "59.74 fb^{-1} (13 TeV)";
  }

  TLatex latex_CMSPriliminary;
  latex_CMSPriliminary.SetNDC();
  latex_CMSPriliminary.SetTextSize(0.035);

  TLatex latex_Lumi;
  latex_Lumi.SetNDC();
  latex_Lumi.SetTextSize(0.035);
  latex_Lumi.SetTextFont(42);

  setTDRStyle();
  gStyle->SetOptStat(0);
  gStyle->SetPalette(kBeach);

  TH1::SetDefaultSumw2(true);
  TH1::AddDirectory(kFALSE);

  TString WORKING_DIR = getenv("PLOTTER_WORKING_DIR");
  TString dataset = getenv("CATANVERSION");
  TString ENV_PLOT_PATH = getenv("PLOT_PATH");

  TString base_filepath = "/data6/Users/jskim/SKFlatOutput/Run2Legacy_v4/HNWRAnalyzer/"+TString::Itoa(Year,10)+"/RunSyst__Signal__RunXsecSyst__RunNewPDF__"+PDFName+"__/";
  if(PDFName=="NNPDF31_nnlo_hessian_pdfas"){
    base_filepath = "/data6/Users/jskim/HNWR_Plotter/rootfiles/Run2Legacy_v4__Default/Regions/2016/Signal/";
  }
  TString base_plotpath = ENV_PLOT_PATH+"/"+dataset+"/PDFErrorPlot/"+TString::Itoa(Year,10)+"/"+PDFName+"/";
  gSystem->mkdir(base_plotpath, kTRUE);

  vector<TString> regions = {
    //"Resolved",
    "Boosted",
  };
  vector<TString> channels = {
    //"Electron",
    "Muon",
  };

  LRSMSignalInfo tmp_lrsminfo;
  tmp_lrsminfo.GetMassMaps();

  for(unsigned int it_channel=0; it_channel<channels.size(); it_channel++){

    TString channel = channels.at(it_channel);

    cout << "@@@@ channel = " << channel << endl;

    for(unsigned int it_region=0; it_region<regions.size(); it_region++){

      TString region = regions.at(it_region);

      //==== 1D : Error vs N, for each WR

      for(map< double, vector<double> >::iterator it=tmp_lrsminfo.maps_WR_to_N.begin(); it!=tmp_lrsminfo.maps_WR_to_N.end(); it++){

        double m_WR = it->first;
        vector<double> this_m_Ns = it->second;

        const int n_N = this_m_Ns.size();

        TCanvas *c_vs_N = new TCanvas("c_vs_N", "", 600, 600);
        canvas_margin(c_vs_N);
        c_vs_N->cd();

        double mN_max = this_m_Ns.at( this_m_Ns.size()-1 );
        TH1D *hist_dummy_vs_N = new TH1D("hist_dummy_vs_N", "", int((mN_max-0.)/100), 0., mN_max);
        hist_axis(hist_dummy_vs_N);
        hist_dummy_vs_N->GetYaxis()->SetTitle("Uncertainty");
        hist_dummy_vs_N->GetXaxis()->SetTitle("m_{N} (GeV)");
        hist_dummy_vs_N->Draw("hist");

        TLatex str_m_WR;
        str_m_WR.SetNDC();
        str_m_WR.SetTextSize(0.035);
        str_m_WR.DrawLatex(0.65, 0.85, "m_{WR} = "+TString::Itoa(m_WR,10)+" GeV");

        const int NX = this_m_Ns.size();
        double x[NX], y_Scale[NX], y_PDFError[NX], y_AlphaS[NX];
        for(int ix=0; ix<NX; ix++){

          double m_N = this_m_Ns.at(ix);


          TString mass = "WR"+TString::Itoa(m_WR,10)+"_N"+TString::Itoa(m_N,10);

          TString filepath = base_filepath+"HNWRAnalyzer_WRtoNLtoLLJJ_"+mass+".root";
          TFile *file = new TFile(filepath);

          SignalSystematics m;
          m.file = file;
          m.isReplica = isReplica;
          m.region = "HNWR_Single"+channel+"_"+region+"_SR";
          m.UseCustomRebin = true;

          TH1D *hist_sig_SignalFlavour = (TH1D *)file->Get("SignalFlavour");

          m.ChannelFrac = 1./hist_sig_SignalFlavour->GetEntries();
          if(m.region.Contains("Electron")) m.ChannelFrac *= hist_sig_SignalFlavour->GetBinContent(2);
          else if(m.region.Contains("Muon")) m.ChannelFrac *= hist_sig_SignalFlavour->GetBinContent(3);
          else{
            cout << "WTF?? channel = " << m.region << endl;
            return;
          }

          m.DoDebug = false;
          m.DrawPlot = false;

          TH1D *hist_sig = (TH1D *)file->Get(m.region+"/WRCand_Mass_"+m.region);
          m.hist_Central = hist_sig;
          m.Run();

          x[ix] = m_N;
          y_Scale[ix] = m.xsec_ScaleSyst;
          y_PDFError[ix] = m.xsec_PDFErrorSyst;
          y_AlphaS[ix] = m.xsec_AlphaSSyst;

          file->Close();

        } // END Loop over N

        TGraph *gr_Scale = new TGraph(NX, x, y_Scale);
        TGraph *gr_PDFError = new TGraph(NX, x, y_PDFError);
        TGraph *gr_AlphaS = new TGraph(NX, x, y_AlphaS);

        gr_Scale->SetLineColor(kBlack);
        gr_PDFError->SetLineColor(kBlue);
        gr_AlphaS->SetLineColor(kRed);

        gr_Scale->SetLineWidth(3);
        gr_PDFError->SetLineWidth(3);
        gr_AlphaS->SetLineWidth(3);

        gr_Scale->Draw("lsame");
        gr_PDFError->Draw("lsame");
        gr_AlphaS->Draw("lsame");

        TLegend *lg = new TLegend(0.25, 0.7, 0.5, 0.9);
        lg->AddEntry(gr_Scale, "Scale", "l");
        lg->AddEntry(gr_PDFError, "PDF error", "l");
        lg->AddEntry(gr_AlphaS, "#alpha_{S}", "l");
        lg->Draw();

        c_vs_N->SaveAs(base_plotpath+"/"+region+"_"+channel+"_Err_vs_N_WR"+TString::Itoa(m_WR,10)+".pdf");
        c_vs_N->Close();

      } // END Loop over WR


      //==== 1D : Error vs WR, for each N

      vector<double> test_Ns = {100, 200, 400, 600, 800, 1000};

      for(int z=0; z<test_Ns.size(); z++){

        double m_N = test_Ns.at(z);

        vector<double> this_WRs;
        for(map< double, vector<double> >::iterator it=tmp_lrsminfo.maps_N_to_WR.begin(); it!=tmp_lrsminfo.maps_N_to_WR.end(); it++){

          double this_m_N = it->first;
          vector<double> this_m_WRs = it->second;

          for(int it_N=0; it_N<this_m_WRs.size(); it_N++){

            double m_WR = this_m_WRs.at(it_N);

            bool ToDraw = false;
            if(m_N>0) ToDraw = (this_m_N==m_N);
            else ToDraw = (2*this_m_N==m_WR);

            if(ToDraw) this_WRs.push_back( m_WR );

          }

        }

        TCanvas *c_vs_WR = new TCanvas("c_vs_WR", "", 600, 600);
        canvas_margin(c_vs_WR);
        c_vs_WR->cd();

        TH1D *hist_dummy_vs_WR = new TH1D("hist_dummy_vs_WR", "", 7000/100, 0., 7000.);
        hist_axis(hist_dummy_vs_WR);
        hist_dummy_vs_WR->GetYaxis()->SetTitle("Uncertainty");
        hist_dummy_vs_WR->GetXaxis()->SetTitle("m_{W_{R}} (GeV)");
        hist_dummy_vs_WR->Draw("hist");
        
        TLatex str_m_WR;
        str_m_WR.SetNDC();
        str_m_WR.SetTextSize(0.035);
        str_m_WR.DrawLatex(0.65, 0.85, "m_{N} = "+TString::Itoa(m_N,10)+" GeV");
        
        const int NX = this_WRs.size();
        double x[NX], y_Scale[NX], y_PDFError[NX], y_AlphaS[NX];

        for(unsigned int ix=0; ix<this_WRs.size(); ix++){

          double m_WR = this_WRs.at(ix);
          TString mass = "WR"+TString::Itoa(m_WR,10)+"_N"+TString::Itoa(m_N,10);

          TString filepath = base_filepath+"HNWRAnalyzer_WRtoNLtoLLJJ_"+mass+".root";
          TFile *file = new TFile(filepath);
        
          SignalSystematics m;
          m.file = file;
          m.isReplica = isReplica;
          m.region = "HNWR_Single"+channel+"_"+region+"_SR";
          m.UseCustomRebin = true;

          TH1D *hist_sig_SignalFlavour = (TH1D *)file->Get("SignalFlavour");

          m.ChannelFrac = 1./hist_sig_SignalFlavour->GetEntries();
          if(m.region.Contains("Electron")) m.ChannelFrac *= hist_sig_SignalFlavour->GetBinContent(2);
          else if(m.region.Contains("Muon")) m.ChannelFrac *= hist_sig_SignalFlavour->GetBinContent(3);
          else{
            cout << "WTF?? channel = " << m.region << endl;
            return;
          }
          
          m.DoDebug = false;
          m.DrawPlot = false;
          
          TH1D *hist_sig = (TH1D *)file->Get(m.region+"/WRCand_Mass_"+m.region);
          m.hist_Central = hist_sig;
          m.Run();
          
          x[ix] = m_WR;
          y_Scale[ix] = m.xsec_ScaleSyst;
          y_PDFError[ix] = m.xsec_PDFErrorSyst;
          y_AlphaS[ix] = m.xsec_AlphaSSyst;
            
          file->Close();

        } // END WR loop

        TGraph *gr_Scale = new TGraph(NX, x, y_Scale);
        TGraph *gr_PDFError = new TGraph(NX, x, y_PDFError);
        TGraph *gr_AlphaS = new TGraph(NX, x, y_AlphaS);
          
        gr_Scale->SetLineColor(kBlack);
        gr_PDFError->SetLineColor(kBlue);
        gr_AlphaS->SetLineColor(kRed);
          
        gr_Scale->SetLineWidth(3);
        gr_PDFError->SetLineWidth(3);
        gr_AlphaS->SetLineWidth(3);

        gr_Scale->Draw("lsame");
        gr_PDFError->Draw("lsame");
        gr_AlphaS->Draw("lsame");

        TLegend *lg = new TLegend(0.25, 0.7, 0.5, 0.9);
        lg->AddEntry(gr_Scale, "Scale", "l");
        lg->AddEntry(gr_PDFError, "PDF error", "l");
        lg->AddEntry(gr_AlphaS, "#alpha_{S}", "l");
        lg->Draw();

        c_vs_WR->SaveAs(base_plotpath+"/"+region+"_"+channel+"_Err_vs_WR_N"+TString::Itoa(m_N,10)+".pdf");
        c_vs_WR->Close();


      } // END N loop




    } // END Loop over regions

  } // END Loop channels


}


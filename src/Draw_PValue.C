#include "canvas_margin.h"
#include "LRSMSignalInfo.h"
#include "mylib.h"

void Draw_PValue(int Year, TString dirname=""){

  TString inputfile = "";
  TString TotalLumi = "";
  TString str_Year = TString::Itoa(Year,10);

  inputfile = dirname;
  if(Year==2016){
    TotalLumi = "35.92 fb^{-1} (13 TeV)";
  }
  else if(Year==2017){
    TotalLumi = "41.53 fb^{-1} (13 TeV)";
  }
  else if(Year==2018){
    TotalLumi = "59.74 fb^{-1} (13 TeV)";
  }
  else if(Year==-1){
    TotalLumi = "137 fb^{-1} (13 TeV)";
    str_Year = "YearCombined";
  }

  TLatex latex_CMSPriliminary;
  latex_CMSPriliminary.SetNDC();
  latex_CMSPriliminary.SetTextSize(0.035);

  TLatex latex_Lumi;
  latex_Lumi.SetNDC();
  latex_Lumi.SetTextSize(0.035);
  latex_Lumi.SetTextFont(42);

  TLatex latex_ch;
  latex_ch.SetNDC();
  latex_ch.SetTextSize(0.040);

  setTDRStyle();
  gStyle->SetOptStat(0);
  gStyle->SetPalette(55);

  TH1::SetDefaultSumw2(true);
  TH1::AddDirectory(kFALSE);

  TString WORKING_DIR = getenv("PLOTTER_WORKING_DIR");
  TString dataset = getenv("CATANVERSION");
  TString ENV_PLOT_PATH = getenv("PLOT_PATH");

  TString filepath_result = WORKING_DIR+"/rootfiles/"+dataset+"/PValue/"+str_Year+"/"+inputfile+".txt";
  cout << "@@@@ filepath_result = " << filepath_result << endl;
  TString plotpath = ENV_PLOT_PATH+"/"+dataset+"/PValue/"+str_Year+"/"+inputfile;

  gSystem->mkdir(plotpath, kTRUE);

  vector<TString> regions = {
    "Combined",
    "Resolved",
    "Boosted",
  };
  vector<Color_t> colors = {
    kBlack,
    kGreen,
    kBlue,
  };

  vector<TString> channels = {
    "EE",
    "MuMu",
  };

  //==== y=x line

  double x_0[2], y_0[2];
  x_0[0] = 0;  y_0[0] = 0;
  x_0[1] = 10000;  y_0[1] = 10000;

  vector<double> pvalueForSigs = {
    0.159,
    0.0228,
    0.00135,
    3.17E-5,
    2.87E-7,
  };
  vector<TGraph *> gr_pvalue;
  for(unsigned int i=0; i<pvalueForSigs.size(); i++){

    y_0[0] = pvalueForSigs.at(i);
    y_0[1] = pvalueForSigs.at(i);

    TGraph *g_tmp = new TGraph(2, x_0, y_0);
    g_tmp->SetName("gr_pvalue_"+TString::Itoa(i+1,10));
    g_tmp->SetLineColor(kBlack);
    g_tmp->SetLineWidth(3);
    g_tmp->SetLineStyle(3);
    gr_pvalue.push_back(g_tmp);

  }

  //=============
  //==== Result
  //=============

  LRSMSignalInfo tmp_lrsminfo;
  tmp_lrsminfo.GetMassMaps();

  for(unsigned int it_channel=0; it_channel<channels.size(); it_channel++){

    TString channel = channels.at(it_channel);

    cout << "@@@@ channel = " << channel << endl;

    TLegend *lg = new TLegend(0.15, 0.62, 0.55, 0.94);
    lg->SetBorderSize(0);
    lg->SetFillStyle(0);

    //==== Our results

    vector<LRSMSignalInfo> results;
    for(map< double, vector<double> >::iterator it=tmp_lrsminfo.maps_WR_to_N.begin(); it!=tmp_lrsminfo.maps_WR_to_N.end(); it++){

      double m_WR = it->first;
      //cout << "Working on m_WR = " << m_WR << endl;
      vector<double> this_m_Ns = it->second;

      for(int it_N=0; it_N<this_m_Ns.size(); it_N++){

        double m_N = this_m_Ns.at(it_N);
        //cout << "  m_N = " << m_N << endl;


        LRSMSignalInfo lrsminfo;
        lrsminfo.prod_channel="SchWR";
        lrsminfo.generator="aMCNLO";
        lrsminfo.lep_channel = channel;
        lrsminfo.mass_WR = m_WR;
        lrsminfo.mass_N = m_N;
        lrsminfo.SetNames();

        string line_limit;
        ifstream in_limit(filepath_result);
        bool limit_found = false;
        //cout << "  Looking for (channel, m_WR, m_N)" << "("<<channel<<", "<<m_WR<<", "<<m_N<<")"<<endl;
        while(getline(in_limit,line_limit)){
          std::istringstream is( line_limit );

          TString ch_, region_;
          double mwr, mn, xsec;

          is >> ch_;
          is >> region_;
          is >> mwr;
          is >> mn;

          if(ch_==channel && int(m_WR)==int(mwr) && int(m_N)==int(mn)){

            LimitResult m;

            m.region = region_;

            is >> m.limit_exp;

            lrsminfo.LimitResults.push_back( m );

            //==== debuggin lines
            //LimitResult n=lrsminfo.LimitResults.at(lrsminfo.LimitResults.size()-1);
            //cout << n.limit_exp << "\t" << n.limit_exp_1sdUp << "\t" << n.limit_exp_1sdDn << "\t" << n.limit_exp_2sdUp << "\t" << n.limit_exp_2sdDn << endl;
            //lrsminfo.Print();

          }

        }

        results.push_back( lrsminfo );

      }

    }

    //==== 1D : Limit vs N, for each WR

    for(map< double, vector<double> >::iterator it=tmp_lrsminfo.maps_WR_to_N.begin(); it!=tmp_lrsminfo.maps_WR_to_N.end(); it++){

      double m_WR = it->first;
      vector<double> this_m_Ns = it->second;

      for(unsigned int it_region=0; it_region<regions.size(); it_region++){

        TString region = regions.at(it_region);

        const int n_N = this_m_Ns.size();
        double x_N[n_N], y_Sig[n_N];

        for(int it_N=0; it_N<this_m_Ns.size(); it_N++){

          double m_N = this_m_Ns.at(it_N);

          LRSMSignalInfo this_lrsm;
          bool found = false;
          for(unsigned it_result=0; it_result<results.size(); it_result++){
            if(results.at(it_result).mass_WR == m_WR && results.at(it_result).mass_N == m_N){
              this_lrsm = results.at(it_result);
              found = true;
              break;
            }
          }
          if(!found){
            cout << "[1D : Limit vs N, for each WR] no result[s] for WR = " << m_WR << ", N = " << m_N << endl;
          }

          LimitResult this_result;
          found = false;
          vector<LimitResult> this_results = this_lrsm.LimitResults;
          for(unsigned int z=0; z<this_results.size(); z++){
            if(this_results.at(z).region == region){
              this_result = this_results.at(z);
              found = true;
            }
          }
          if(!found){
            cout << "[1D : Limit vs N, for each WR] no result for WR = " << m_WR << ", N = " << m_N << endl;
          }

          x_N[it_N] = m_N;
          y_Sig[it_N] = this_result.limit_exp;

        } // END Loop over N

        TGraphAsymmErrors *gr_Sig = new TGraphAsymmErrors(n_N,x_N,y_Sig,0,0,0,0);
        gr_Sig->SetLineColor(kBlack);
        gr_Sig->SetLineWidth(3);
        //gr_Sig->SetLineStyle(2);

        TLegend *lg = new TLegend(0.2, 0.2, 0.7, 0.35);
        lg->SetBorderSize(0);
        lg->SetFillStyle(0);
        //lg->AddEntry( gr_exp, "Sig", "l");

        TCanvas *c_1D_vsN = new TCanvas("c1", "", 800, 800);
        canvas_margin(c_1D_vsN);
        c_1D_vsN->cd();
        c_1D_vsN->SetLogy();

        TH1D *hist_dummy = new TH1D("hist_dummy", "", 7000, 0., 7000.);
        hist_dummy->Draw("hist");
        hist_axis(hist_dummy);
        hist_dummy->GetXaxis()->SetRangeUser(this_m_Ns.at(0), this_m_Ns.at(this_m_Ns.size()-1));
        hist_dummy->GetXaxis()->SetRangeUser(0., 6800.);
        hist_dummy->GetXaxis()->SetTitle("m_{N} (GeV)");
        hist_dummy->GetYaxis()->SetTitle("p-value");
        hist_dummy->GetYaxis()->SetRangeUser(1E-5, 2); 

        gr_Sig->Draw("lsame");

        TLatex str_m_WR;
        str_m_WR.SetNDC();
        str_m_WR.SetTextSize(0.035);
        str_m_WR.DrawLatex(0.55, 0.85, "m_{WR} = "+TString::Itoa(m_WR,10)+" GeV");
        if(channel=="EE")        latex_ch.DrawLatex(0.55, 0.80, region+" ee channel");
        else if(channel=="MuMu") latex_ch.DrawLatex(0.55, 0.80, region+" #mu#mu channel");

        lg->Draw();

        latex_CMSPriliminary.DrawLatex(0.15, 0.96, "#font[62]{CMS} #font[42]{#it{#scale[0.8]{Preliminary}}}");
        latex_Lumi.DrawLatex(0.73, 0.96, TotalLumi);

        for(unsigned int i=0; i<gr_pvalue.size(); i++){
          gr_pvalue.at(i)->Draw("lsame");

          TLatex tl;
          tl.SetTextSize(0.037);
          tl.DrawLatex(6000, pvalueForSigs.at(i)*0.5, TString::Itoa(i+1,10)+"#sigma");

        }

        c_1D_vsN->SaveAs(plotpath+"/1D_"+channel+"_"+region+"_WR"+TString::Itoa(m_WR,10)+"_pvalue_vs_N.pdf");
        c_1D_vsN->Close();

      } // END Loop over regions

    } // END Loop over WR

    //==== 1D : Limit vs WR, for each N
    //==== negative N means N=WR/2

    vector<double> test_Ns = {-500, 100, 200, 400, 600, 800};

    for(int z=0; z<test_Ns.size(); z++){

       double testN = test_Ns.at(z);

      //==== 1D : Limit vs WR, for each N = WR/2

      //==== Check how many mN=mWR/2 point exist
      vector<LRSMSignalInfo> lrsminfo_Half;
      for(map< double, vector<double> >::iterator it=tmp_lrsminfo.maps_N_to_WR.begin(); it!=tmp_lrsminfo.maps_N_to_WR.end(); it++){

        double m_N = it->first;
        vector<double> this_m_WRs = it->second;

        for(int it_N=0; it_N<this_m_WRs.size(); it_N++){

          double m_WR = this_m_WRs.at(it_N);

          bool ToDraw = false;
          if(testN>0) ToDraw = (m_N==testN);
          else ToDraw = (2*m_N==m_WR);

          if(ToDraw){
            for(unsigned it_result=0; it_result<results.size(); it_result++){
              if(results.at(it_result).mass_WR == m_WR && results.at(it_result).mass_N == m_N){
                lrsminfo_Half.push_back( results.at(it_result) );
                break;
              }
            }

          }

        }

      }

      for(unsigned int it_region=0; it_region<regions.size(); it_region++){

        TString region = regions.at(it_region);
        const int n_WR = lrsminfo_Half.size();

        double x_WR[n_WR], y_Sig[n_WR];

        for(unsigned int it_N=0; it_N<lrsminfo_Half.size(); it_N++){

          LRSMSignalInfo this_lrsm = lrsminfo_Half.at(it_N);

          LimitResult this_result;
          vector<LimitResult> this_results = this_lrsm.LimitResults;
          for(unsigned int z=0; z<this_results.size(); z++){
            if(this_results.at(z).region == region){
              this_result = this_results.at(z);
            }
          }


          x_WR[it_N] = this_lrsm.mass_WR;
          y_Sig[it_N] = this_result.limit_exp;

        }

        TGraphAsymmErrors *gr_Sig = new TGraphAsymmErrors(n_WR,x_WR,y_Sig,0,0,0,0);
        gr_Sig->SetLineColor(kBlack);
        gr_Sig->SetLineWidth(3);
        //gr_Sig->SetLineStyle(2);

        TLegend *lg = new TLegend(0.2, 0.2, 0.7, 0.35);
        lg->SetBorderSize(0);
        lg->SetFillStyle(0);
        //lg->AddEntry( gr_exp, "Expected limit", "l");

        TCanvas *c_1D_vsN = new TCanvas("c1", "", 800, 800);
        canvas_margin(c_1D_vsN);
        c_1D_vsN->cd();
        c_1D_vsN->SetLogy();

        TH1D *hist_dummy = new TH1D("hist_dummy", "", 7000, 0., 7000.);
        hist_dummy->Draw("hist");
        hist_axis(hist_dummy);
        hist_dummy->GetXaxis()->SetRangeUser(800,6800);
        hist_dummy->GetXaxis()->SetTitle("m_{W_{R}} (GeV)");
        hist_dummy->GetYaxis()->SetTitle("p-value");
        hist_dummy->GetYaxis()->SetRangeUser(1E-5, 2); 

        gr_Sig->Draw("lsame");

        hist_dummy->Draw("axissame");

        TLatex str_m_WR;
        str_m_WR.SetNDC();
        str_m_WR.SetTextSize(0.035);

        TString NMassString = "";
        if(testN>0){
          NMassString = "m_{N} = "+TString::Itoa(testN,10)+" GeV";
        }
        else{
          NMassString = "m_{N} = m_{WR}/2";
        }

        str_m_WR.DrawLatex(0.55, 0.85, NMassString);
        if(channel=="EE")        latex_ch.DrawLatex(0.55, 0.80, region+" ee channel");
        else if(channel=="MuMu") latex_ch.DrawLatex(0.55, 0.80, region+" #mu#mu channel");

        lg->Draw();

        latex_CMSPriliminary.DrawLatex(0.15, 0.96, "#font[62]{CMS} #font[42]{#it{#scale[0.8]{Preliminary}}}");
        latex_Lumi.DrawLatex(0.73, 0.96, TotalLumi);


        TString outname = "";
        if(testN>0){
          outname = "1D_"+channel+"_"+region+"_N"+TString::Itoa(testN,10)+"_pvalue_vs_WR";
        }
        else{
          outname = "1D_"+channel+"_"+region+"_HalfN_pvalue_vs_WR";
        }

        for(unsigned int i=0; i<gr_pvalue.size(); i++){
          gr_pvalue.at(i)->Draw("lsame");

          TLatex tl;
          tl.SetTextSize(0.037);
          tl.DrawLatex(6000, pvalueForSigs.at(i)*0.5, TString::Itoa(i+1,10)+"#sigma");

        }

        c_1D_vsN->SaveAs(plotpath+"/"+outname+".pdf");
        c_1D_vsN->Close();

      }

    } // END Loop over regions

  } // END Loop channels



}


#include "canvas_margin.h"
#include "LRSMSignalInfo.h"
#include "mylib.h"

void Draw_Limit(int Year, TString dirname=""){

  TString ATLASexpORobs = "exp";

  //==== TODO
  //==== Change result to obs (Both CMS and ATALS previoud result)

  bool DrawObs = false;

  gErrorIgnoreLevel = kFatal;

  bool Usekfactor = true;

  double signal_scale = 0.1;

  TString inputfile = "";
  TString TotalLumi = "";
  TString str_Year = TString::Itoa(Year,10);

  inputfile = dirname;
  if(Year==2016){
    //inputfile = dirname;
    TotalLumi = "35.92 fb^{-1} (13 TeV)";
  }
  else if(Year==2017){
    //inputfile = "";
    TotalLumi = "41.53 fb^{-1} (13 TeV)";
  }
  else if(Year==2018){
    //inputfile = "";
    TotalLumi = "59.74 fb^{-1} (13 TeV)";
  }
  else if(Year==-1){
    //inputfile = "";
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

  bool UseAsymptotic = true;

  TString Method = "FullCLs";
  if(UseAsymptotic) Method = "Asymptotic";

  setTDRStyle();
  gStyle->SetOptStat(0);
  gStyle->SetPalette(55);

  TH1::SetDefaultSumw2(true);
  TH1::AddDirectory(kFALSE);

  TString WORKING_DIR = getenv("PLOTTER_WORKING_DIR");
  TString dataset = getenv("CATANVERSION");
  TString ENV_PLOT_PATH = getenv("PLOT_PATH");

  TString filepath_result = WORKING_DIR+"/rootfiles/"+dataset+"/Limit/"+Method+"/"+str_Year+"/"+inputfile+".txt";
  cout << "@@@@ filepath_result = " << filepath_result << endl;
  TString plotpath = ENV_PLOT_PATH+"/"+dataset+"/Limit/"+Method+"/"+str_Year+"/"+inputfile;
  if(!Usekfactor) plotpath = ENV_PLOT_PATH+"/"+dataset+"/Limit/"+Method+"/"+str_Year+"/"+inputfile+"_Nokfactor";

  gSystem->mkdir(plotpath, kTRUE);

  TString filename_thoery = WORKING_DIR+"/data/"+dataset+"/xsec_190705_GenXsecAN_eeANDmm.txt";

  vector<TString> regions = {
    "Combined",
    "Resolved",
    "Boosted",
  };
  vector<TString> aliases = {
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
  TGraph *g0 = new TGraph(2, x_0, y_0);
  g0->SetLineStyle(3);
  g0->SetLineColor(kGray);

  //=============
  //==== Result
  //=============

  LRSMSignalInfo tmp_lrsminfo;
  tmp_lrsminfo.GetMassMaps();

  for(unsigned int it_channel=0; it_channel<channels.size(); it_channel++){

    TString channel = channels.at(it_channel);

    cout << "@@@@ channel = " << channel << endl;

    TLegend *lg = new TLegend(0.15, 0.62, 0.6, 0.94);
    lg->SetBorderSize(0);
    lg->SetFillStyle(0);

    //==== ATLAS (1809.11105) : https://www.hepdata.net/record/ins1696330
    string atlas_line;
    vector<double> atlas_mZp, atlas_mN;
    ifstream atlas_in(WORKING_DIR+"/data/"+dataset+"/Limit_ATLAS13TeV_"+channel+"_"+ATLASexpORobs+".txt");
    while(getline(atlas_in,atlas_line)){
      std::istringstream is( atlas_line );

      double a,b;
      is >> a;
      is >> b;

      atlas_mZp.push_back(a*1000.);
      atlas_mN.push_back(b*1000.);
    }

    const int N_atlas = atlas_mZp.size();
    double mZp_atlas[N_atlas+2], mN_atlas[N_atlas+2];
    for(int i_atlas = 0; i_atlas < N_atlas; i_atlas++){
      mZp_atlas[i_atlas] = atlas_mZp.at(i_atlas);
      mN_atlas[i_atlas] = atlas_mN.at(i_atlas);
    }

    mZp_atlas[N_atlas] = 600.;
    mN_atlas[N_atlas] = 50.;
    mZp_atlas[N_atlas+1] = atlas_mZp.at(0);
    mN_atlas[N_atlas+1] = atlas_mN.at(0);

    TGraph *gr_atlas = new TGraph(N_atlas + 2, mZp_atlas, mN_atlas);
    gr_atlas-> SetLineStyle(8);
    gr_atlas->SetLineWidth(3);
    gr_atlas->SetLineColor(kGray);

    //==== ATLAS (1904.12679) : https://www.hepdata.net/record/ins1731814
    string atlas_boosted_line;
    vector<double> atlas_boosted_mZp, atlas_boosted_mN;
    ifstream atlas_boosted_in(WORKING_DIR+"/data/"+dataset+"/Limit_ATLAS13TeVBoosted_"+channel+"_"+ATLASexpORobs+".txt");
    while(getline(atlas_boosted_in,atlas_boosted_line)){
      std::istringstream is( atlas_boosted_line );

      double a,b;
      is >> a;
      is >> b;

      atlas_boosted_mZp.push_back(a);
      atlas_boosted_mN.push_back(b);
    }

    const int N_atlas_boosted = atlas_boosted_mZp.size();
    double mZp_atlas_boosted[N_atlas_boosted+2], mN_atlas_boosted[N_atlas_boosted+2];
    for(int i_atlas_boosted = 0; i_atlas_boosted < N_atlas_boosted; i_atlas_boosted++){
      mZp_atlas_boosted[i_atlas_boosted] = atlas_boosted_mZp.at(i_atlas_boosted);
      mN_atlas_boosted[i_atlas_boosted] = atlas_boosted_mN.at(i_atlas_boosted);
    }

    mZp_atlas_boosted[N_atlas_boosted] = 600.;
    mN_atlas_boosted[N_atlas_boosted] = 50.;
    mZp_atlas_boosted[N_atlas_boosted+1] = atlas_boosted_mZp.at(0);
    mN_atlas_boosted[N_atlas_boosted+1] = atlas_boosted_mN.at(0);

    TGraph *gr_atlas_boosted = new TGraph(N_atlas_boosted + 2, mZp_atlas_boosted, mN_atlas_boosted);
    gr_atlas_boosted->SetLineStyle(3);
    gr_atlas_boosted->SetLineWidth(3);
    gr_atlas_boosted->SetLineColor(kGray);

    //==== EXO17011

    string EXO17011_line;
    vector<double> EXO17011_mZp, EXO17011_mN;
    ifstream EXO17011_in(WORKING_DIR+"/data/"+dataset+"/Limit_EXO17011_"+channel+"_"+ATLASexpORobs+".txt");
    while(getline(EXO17011_in,EXO17011_line)){
      std::istringstream is( EXO17011_line );

      double a,b;
      is >> a;
      is >> b;

      EXO17011_mZp.push_back(a);
      EXO17011_mN.push_back(b);
    }

    const int N_EXO17011 = EXO17011_mZp.size();
    double mZp_EXO17011[N_EXO17011], mN_EXO17011[N_EXO17011];
    for(int i_EXO17011 = 0; i_EXO17011 < N_EXO17011; i_EXO17011++){
      mZp_EXO17011[i_EXO17011] = EXO17011_mZp.at(i_EXO17011);
      mN_EXO17011[i_EXO17011] = EXO17011_mN.at(i_EXO17011);
    }

    TGraph *gr_EXO17011 = new TGraph(N_EXO17011, mZp_EXO17011, mN_EXO17011);
    gr_EXO17011-> SetLineStyle(8);
    gr_EXO17011->SetLineWidth(3);
    gr_EXO17011->SetLineColor(kMagenta);

    //==== Our results

    vector<LRSMSignalInfo> results;
    for(map< double, vector<double> >::iterator it=tmp_lrsminfo.maps_WR_to_N.begin(); it!=tmp_lrsminfo.maps_WR_to_N.end(); it++){

      double m_WR = it->first;
      vector<double> this_m_Ns = it->second;

      for(int it_N=0; it_N<this_m_Ns.size(); it_N++){

        double m_N = this_m_Ns.at(it_N);

        LRSMSignalInfo lrsminfo;
        lrsminfo.prod_channel="SchWR";
        lrsminfo.generator="aMCNLO";
        lrsminfo.lep_channel = channel;
        lrsminfo.mass_WR = m_WR;
        lrsminfo.mass_N = m_N;
        lrsminfo.SetNames();

        string line_xsec;
        ifstream in_xsec(filename_thoery);
        bool theory_xsec_found = false;
        while(getline(in_xsec,line_xsec)){
          std::istringstream is( line_xsec );

          double mwr, mn, xsec, xsec_err;

          is >> mwr;
          is >> mn;
          is >> xsec;
          is >> xsec_err;

          //==== pb -> fb
          xsec = 1000.*xsec;
          //==== upper limit is xsec(ee+mm)

          if(Usekfactor){

            double this_kfactor = GetKFactor(int(mwr), int(mn));
            xsec *= this_kfactor;
          }
          if(int(m_WR)==int(mwr) && int(m_N)==int(mn)){
            theory_xsec_found = true;
            lrsminfo.xsec = xsec;
          }

        }

        string line_limit;
        ifstream in_limit(filepath_result);
        bool limit_found = false;
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
            is >> m.limit_exp_1sdUp;
            is >> m.limit_exp_1sdDn;
            is >> m.limit_exp_2sdUp;
            is >> m.limit_exp_2sdDn;
            is >> m.limit_obs;

            m.limit_exp *= signal_scale;
            m.limit_exp_1sdUp *= signal_scale;
            m.limit_exp_1sdDn *= signal_scale;
            m.limit_exp_2sdUp *= signal_scale;
            m.limit_exp_2sdDn *= signal_scale;
            m.limit_obs *= signal_scale;

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

    TCanvas *c_2D = new TCanvas("c_2D", "", 800, 600);
    canvas_margin(c_2D);
    c_2D->SetBottomMargin( 0.125 );
    c_2D->SetRightMargin( 0.16 );
    c_2D->SetLeftMargin( 0.13 );
    c_2D->cd();
    c_2D->SetLogz();

    //==== 2D binning
    //==== 1) WR
    int target_WR_max = 6000;
    int target_WR_min = 0;
    int target_WR_d_bin = 200;
    double bin_WR_max = target_WR_max+target_WR_d_bin/2;
    double bin_WR_min = target_WR_min-target_WR_d_bin/2;
    int bin_WR_n = (bin_WR_max-bin_WR_min)/target_WR_d_bin;
    //==== 2) N
    int target_N_max = 6000;
    int target_N_min = 0;
    int target_N_d_bin = 200;
    double bin_N_max = target_N_max+target_N_d_bin/2;
    double bin_N_min = target_N_min-target_N_d_bin/2;
    int bin_N_n = (bin_N_max-bin_N_min)/target_N_d_bin;

    TH2D *hist_dummy = new TH2D("hist_dummy", "", 71, -100, 7000, 71, -100, 7000);
    hist_axis(hist_dummy);

    hist_dummy->GetYaxis()->SetLabelSize(0.035);
    hist_dummy->GetXaxis()->SetLabelSize(0.035);
    hist_dummy->GetYaxis()->SetTitleSize(0.06);
    hist_dummy->GetXaxis()->SetTitleSize(0.057);
    hist_dummy->GetYaxis()->SetTitleOffset(1.0);
    hist_dummy->GetXaxis()->SetTitleOffset(0.93);

    hist_dummy->Draw("hist");
    hist_dummy->GetYaxis()->SetTitle("m_{N} (GeV)");
    hist_dummy->GetXaxis()->SetRangeUser(800., 5400.);
    hist_dummy->GetYaxis()->SetRangeUser(100., 5400.);
    hist_dummy->GetZaxis()->SetRangeUser(1E-4, 20);
    hist_dummy->GetXaxis()->SetTitle("m_{W_{R}} (GeV)");

    for(unsigned int it_region=0; it_region<regions.size(); it_region++){

      TString region = regions.at(it_region);

      vector<double> vec_wr, vec_n;
      vector<double> vec_xsec;
      vector<double> vec_limit_exp, vec_limit_exp_1sdUp, vec_limit_exp_1sdDn, vec_limit_exp_2sdUp, vec_limit_exp_2sdDn;
      vector<double> vec_limit_obs;

      for(unsigned int r=0; r<results.size(); r++){

        LRSMSignalInfo m = results.at(r);
        vec_xsec.push_back( m.xsec );

        for(unsigned int l=0; l<m.LimitResults.size(); l++){;

          if(m.LimitResults.at(l).region == region){

            vec_wr.push_back( m.mass_WR );
            vec_n.push_back( m.mass_N );
            vec_limit_exp.push_back( m.LimitResults.at(l).limit_exp );
            vec_limit_exp_1sdUp.push_back( m.LimitResults.at(l).limit_exp_1sdUp );
            vec_limit_exp_1sdDn.push_back( m.LimitResults.at(l).limit_exp_1sdDn );
            vec_limit_exp_2sdUp.push_back( m.LimitResults.at(l).limit_exp_2sdUp );
            vec_limit_exp_2sdDn.push_back( m.LimitResults.at(l).limit_exp_2sdDn );
            vec_limit_obs.push_back( m.LimitResults.at(l).limit_obs );

          }
        
        }
      }


      const int n_mass = vec_wr.size();
      double arr_wr[n_mass], arr_n[n_mass];
      double arr_xsec[n_mass];
      double arr_limit_exp[n_mass], arr_limit_exp_1sdUp[n_mass], arr_limit_exp_1sdDn[n_mass], arr_limit_exp_2sdUp[n_mass], arr_limit_exp_2sdDn[n_mass];
      double arr_limit_exp_ratio[n_mass], arr_limit_exp_1sdUp_ratio[n_mass], arr_limit_exp_1sdDn_ratio[n_mass], arr_limit_exp_2sdUp_ratio[n_mass], arr_limit_exp_2sdDn_ratio[n_mass];

      double arr_limit_obs[n_mass];
      double arr_limit_obs_ratio[n_mass];

      for(unsigned int r=0;r<n_mass;r++){

        arr_wr[r] = vec_wr.at(r);
        arr_n[r] = vec_n.at(r);
        arr_xsec[r] = vec_xsec.at(r);
        arr_limit_exp[r]       = vec_limit_exp.at(r);
        arr_limit_exp_1sdUp[r] = vec_limit_exp_1sdUp.at(r);
        arr_limit_exp_1sdDn[r] = vec_limit_exp_1sdDn.at(r);
        arr_limit_exp_2sdUp[r] = vec_limit_exp_2sdUp.at(r);
        arr_limit_exp_2sdDn[r] = vec_limit_exp_2sdDn.at(r);
        arr_limit_obs[r]       = vec_limit_obs.at(r);

        arr_limit_exp_ratio[r]       = arr_xsec[r]/vec_limit_exp.at(r);
        arr_limit_exp_1sdUp_ratio[r] = arr_xsec[r]/vec_limit_exp_1sdUp.at(r);
        arr_limit_exp_1sdDn_ratio[r] = arr_xsec[r]/vec_limit_exp_1sdDn.at(r);
        arr_limit_exp_2sdUp_ratio[r] = arr_xsec[r]/vec_limit_exp_2sdUp.at(r);
        arr_limit_exp_2sdDn_ratio[r] = arr_xsec[r]/vec_limit_exp_2sdDn.at(r);
        arr_limit_obs_ratio[r]       = arr_xsec[r]/vec_limit_obs.at(r);


      }

      TGraph2D *gr2d_xsec = new TGraph2D(n_mass, arr_wr, arr_n, arr_xsec);

      TGraph2D *gr2d_limit_exp = new TGraph2D(n_mass, arr_wr, arr_n, arr_limit_exp);
      TGraph2D *gr2d_limit_exp_1sdUp = new TGraph2D(n_mass, arr_wr, arr_n, arr_limit_exp_1sdUp);
      TGraph2D *gr2d_limit_exp_1sdDn = new TGraph2D(n_mass, arr_wr, arr_n, arr_limit_exp_1sdDn);
      TGraph2D *gr2d_limit_exp_2sdUp = new TGraph2D(n_mass, arr_wr, arr_n, arr_limit_exp_2sdUp);
      TGraph2D *gr2d_limit_exp_2sdDn = new TGraph2D(n_mass, arr_wr, arr_n, arr_limit_exp_2sdDn);
      TGraph2D *gr2d_limit_obs = new TGraph2D(n_mass, arr_wr, arr_n, arr_limit_obs);

      TGraph2D *gr2d_limit_exp_ratio = new TGraph2D(n_mass, arr_wr, arr_n, arr_limit_exp_ratio);
      TGraph2D *gr2d_limit_exp_1sdUp_ratio = new TGraph2D(n_mass, arr_wr, arr_n, arr_limit_exp_1sdUp_ratio);
      TGraph2D *gr2d_limit_exp_1sdDn_ratio = new TGraph2D(n_mass, arr_wr, arr_n, arr_limit_exp_1sdDn_ratio);
      TGraph2D *gr2d_limit_exp_2sdUp_ratio = new TGraph2D(n_mass, arr_wr, arr_n, arr_limit_exp_2sdUp_ratio);
      TGraph2D *gr2d_limit_exp_2sdDn_ratio = new TGraph2D(n_mass, arr_wr, arr_n, arr_limit_exp_2sdDn_ratio);
      TGraph2D *gr2d_limit_obs_ratio = new TGraph2D(n_mass, arr_wr, arr_n, arr_limit_obs_ratio);

      //==== Now fill 2D histogram based on TGraph2D's using Interpolate function

      TH2D *hist2d_limit_exp_ratio = new TH2D("hist2d_limit_exp_ratio", "", bin_WR_n, bin_WR_min, bin_WR_max, bin_N_n, bin_N_min, bin_N_max);
      TH2D *hist2d_limit_exp_1sdUp_ratio = new TH2D("hist2d_limit_exp_1sdUp_ratio", "", bin_WR_n, bin_WR_min, bin_WR_max, bin_N_n, bin_N_min, bin_N_max);
      TH2D *hist2d_limit_exp_1sdDn_ratio = new TH2D("hist2d_limit_exp_1sdDn_ratio", "", bin_WR_n, bin_WR_min, bin_WR_max, bin_N_n, bin_N_min, bin_N_max);
      TH2D *hist2d_limit_exp = new TH2D("hist2d_limit_exp", "", bin_WR_n, bin_WR_min, bin_WR_max, bin_N_n, bin_N_min, bin_N_max);

      TH2D *hist2d_limit_obs_ratio = new TH2D("hist2d_limit_obs_ratio", "", bin_WR_n, bin_WR_min, bin_WR_max, bin_N_n, bin_N_min, bin_N_max);
      TH2D *hist2d_limit_obs = new TH2D("hist2d_limit_obs", "", bin_WR_n, bin_WR_min, bin_WR_max, bin_N_n, bin_N_min, bin_N_max);

      for(int it_x=1; it_x<=hist2d_limit_exp_ratio->GetXaxis()->GetNbins(); it_x++){
        double x_center = hist2d_limit_exp_ratio->GetXaxis()->GetBinCenter(it_x);

        //if(x_center<400) continue;

        for(int it_y=1; it_y<=hist2d_limit_exp_ratio->GetYaxis()->GetNbins(); it_y++){
          double y_center = hist2d_limit_exp_ratio->GetYaxis()->GetBinCenter(it_y);

          //if(y_center<100) continue;

          if(y_center>=x_center) continue;

          //cout << x_center << "\t" << y_center << endl;
          double this_xsec = gr2d_xsec->Interpolate(x_center, y_center);
          double this_exp = gr2d_limit_exp->Interpolate(x_center, y_center);
          double this_exp_1sdUp = gr2d_limit_exp_1sdUp->Interpolate(x_center, y_center);
          double this_exp_1sdDn = gr2d_limit_exp_1sdDn->Interpolate(x_center, y_center);
          double this_obs = gr2d_limit_obs->Interpolate(x_center, y_center);
          if(this_xsec<=0||this_exp<=0) continue;

          double this_exp_ratio = this_xsec/this_exp;
          double this_exp_1sdUp_ratio = this_xsec/this_exp_1sdUp;
          double this_exp_1sdDn_ratio = this_xsec/this_exp_1sdDn;

          double this_obs_ratio = this_xsec/this_obs;

          //cout << region << "\t" << x_center << "\t" << y_center << "\t" << this_xsec << "\t" << this_exp << "\t" << this_exp_ratio << endl;

          if(region=="Combined"){
            //cout << x_center << "\t" << y_center << "\t" << this_xsec << "\t" << this_exp << "\t" << this_exp_ratio << endl;
          }

          hist2d_limit_exp_ratio->SetBinContent(it_x, it_y, this_exp_ratio);
          hist2d_limit_exp_1sdUp_ratio->SetBinContent(it_x, it_y, this_exp_1sdUp_ratio);
          hist2d_limit_exp_1sdDn_ratio->SetBinContent(it_x, it_y, this_exp_1sdDn_ratio);
          hist2d_limit_exp->SetBinContent(it_x, it_y, this_exp);

          hist2d_limit_obs_ratio->SetBinContent(it_x, it_y, this_obs_ratio);
          hist2d_limit_obs->SetBinContent(it_x, it_y, this_obs);
        }
      }

      //==== Draw "colz" for "Combined (Resolved+Boosted)" results
      if(region=="Combined"){

        //==== TODO change this to obs
        TH2D *hist2d_limit_exp_ratio_clone = (TH2D *)hist2d_limit_exp_ratio->Clone();
        for(int x=1; x<=hist2d_limit_exp_ratio_clone->GetXaxis()->GetNbins(); x++){
          for(int y=1; y<hist2d_limit_exp_ratio_clone->GetYaxis()->GetNbins(); y++){
            double v = hist2d_limit_exp_ratio_clone->GetBinContent(x,y);
            if(v>0){
              v = 1./v;
              if(v>20) v = 20;
              if(v<1E-4) v=1E-4;
              hist2d_limit_exp_ratio_clone->SetBinContent(x,y,v);
            }
            double x_l = hist2d_limit_exp_ratio_clone->GetXaxis()->GetBinLowEdge(x);
            double x_r = hist2d_limit_exp_ratio_clone->GetXaxis()->GetBinUpEdge(x);
            double y_l = hist2d_limit_exp_ratio_clone->GetYaxis()->GetBinLowEdge(y);
            double y_r = hist2d_limit_exp_ratio_clone->GetYaxis()->GetBinUpEdge(y);
            cout << "x : ["<<x_l<<","<<x_r<<"], y : ["<<y_l<<","<<y_r<<"] -> v = " << v << endl;
          }
        }

        hist2d_limit_exp_ratio_clone->GetZaxis()->SetRangeUser(1E-4, 20);
        //hist2d_limit_exp_ratio_clone->GetZaxis()->SetTitleSize(0.01);
        hist2d_limit_exp_ratio_clone->GetZaxis()->SetLabelSize(0.03);

        hist2d_limit_exp_ratio_clone->GetZaxis()->SetTitleSize(0.040);
        hist2d_limit_exp_ratio_clone->GetZaxis()->SetTitleOffset(1.30);
        hist2d_limit_exp_ratio_clone->GetZaxis()->SetTitle("95% CL upper limit on cross section/ Theory (g_{R}=g_{L})");
        hist2d_limit_exp_ratio_clone->Draw("colzsame");
        hist2d_limit_exp_ratio_clone->Draw("axissame");

      }

      //==== Draw exclusion curves
      double conts[] = {1.};  
      hist2d_limit_exp_ratio->SetContour(1,conts);
      hist2d_limit_exp_ratio->SetLineWidth(2);
      hist2d_limit_exp_ratio->SetLineStyle(1);
      hist2d_limit_exp_ratio->SetLineColor(colors.at(it_region));
      hist2d_limit_exp_ratio->Draw("cont3same");

      hist2d_limit_exp_1sdUp_ratio->SetContour(1,conts);
      hist2d_limit_exp_1sdUp_ratio->SetLineWidth(2);
      hist2d_limit_exp_1sdUp_ratio->SetLineStyle(3);
      hist2d_limit_exp_1sdUp_ratio->SetLineColor(colors.at(it_region));
      //hist2d_limit_exp_1sdUp_ratio->Draw("cont3same");

      hist2d_limit_exp_1sdDn_ratio->SetContour(1,conts);
      hist2d_limit_exp_1sdDn_ratio->SetLineWidth(2);
      hist2d_limit_exp_1sdDn_ratio->SetLineStyle(3);
      hist2d_limit_exp_1sdDn_ratio->SetLineColor(colors.at(it_region));
      //hist2d_limit_exp_1sdDn_ratio->Draw("cont3same");

      hist2d_limit_obs_ratio->SetContour(1,conts);
      hist2d_limit_obs_ratio->SetLineWidth(2);
      hist2d_limit_obs_ratio->SetLineStyle(1);
      hist2d_limit_obs_ratio->SetLineColor(colors.at(it_region));
      if(DrawObs) hist2d_limit_obs_ratio->Draw("cont2same");

      lg->AddEntry( hist2d_limit_exp_ratio, aliases.at(it_region), "l");

    } // END Loop region

    gr_atlas->Draw("lsame");
    gr_atlas_boosted->Draw("lsame");
    gr_EXO17011->Draw("lsame");

    lg->AddEntry( gr_atlas, "ATLAS 13 TeV (Resolved, 36 fb^{-1})", "l");
    lg->AddEntry( gr_atlas_boosted, "ATLAS 13 TeV (Boosted, 80 fb^{-1})", "l");
    lg->AddEntry( gr_EXO17011, "CMS 13 TeV (Resolved, 36 fb^{-1})", "l");

    //g0->Draw("same");

    lg->Draw();

    hist_dummy->Draw("axissame");

    c_2D->cd();
    latex_CMSPriliminary.DrawLatex(0.13, 0.96, "#font[62]{CMS} #font[42]{#it{#scale[0.8]{Preliminary}}}");
    latex_Lumi.DrawLatex(0.66, 0.96, TotalLumi);

    TLatex latex_ch;
    latex_ch.SetNDC();
    latex_ch.SetTextSize(0.040);
    if(channel=="EE")        latex_ch.DrawLatex(0.20, 0.5, "ee channel");
    else if(channel=="MuMu") latex_ch.DrawLatex(0.20, 0.5, "#mu#mu channel");

    c_2D->SaveAs(plotpath+"/2D_"+channel+".pdf");
    c_2D->SaveAs(plotpath+"/2D_"+channel+".C");
    c_2D->Close();

    //==== 1D : Limit vs N, for each WR

    for(map< double, vector<double> >::iterator it=tmp_lrsminfo.maps_WR_to_N.begin(); it!=tmp_lrsminfo.maps_WR_to_N.end(); it++){

      double m_WR = it->first;
      vector<double> this_m_Ns = it->second;

      vector<TString> regions_2 = { "Resolved", "Boosted", "Combined"};
      TGraphAsymmErrors *gr_exp_Resolved;
      TGraphAsymmErrors *gr_exp_Boosted;

      for(unsigned int it_region=0; it_region<regions_2.size(); it_region++){

        TString region = regions_2.at(it_region);

        const int n_N = this_m_Ns.size();
        double x_N[n_N], y_obs[n_N], y_exp[n_N], y_exp_1sdUp[n_N], y_exp_1sdDn[n_N], y_exp_2sdUp[n_N], y_exp_2sdDn[n_N];
        double y_xsec[n_N], y_xsec_err_Up[n_N], y_xsec_err_Dn[n_N];;

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
          y_exp[it_N] = this_result.limit_exp/2.;

          y_exp_1sdUp[it_N] = (this_result.limit_exp_1sdUp - this_result.limit_exp)/2.;
          y_exp_1sdDn[it_N] = (this_result.limit_exp - this_result.limit_exp_1sdDn)/2.;
          y_exp_2sdUp[it_N] = (this_result.limit_exp_2sdUp - this_result.limit_exp)/2.;
          y_exp_2sdDn[it_N] = (this_result.limit_exp - this_result.limit_exp_2sdDn)/2.;

          y_obs[it_N] = this_result.limit_obs/2.;

          y_xsec[it_N] = this_lrsm.xsec/2.;
          y_xsec_err_Up[it_N] = 0. * this_lrsm.xsec/2.;
          y_xsec_err_Dn[it_N] = 0. * this_lrsm.xsec/2.;

        } // END Loop over N

        TGraphAsymmErrors *gr_exp = new TGraphAsymmErrors(n_N,x_N,y_exp,0,0,0,0);
        gr_exp->SetLineColor(kBlack);
        gr_exp->SetLineWidth(3);
        gr_exp->SetLineStyle(2);
        if(it_region==0){
          gr_exp_Resolved = (TGraphAsymmErrors *)gr_exp->Clone();
        }
        if(it_region==1){
          gr_exp_Boosted = (TGraphAsymmErrors *)gr_exp->Clone();
        }

        TGraphAsymmErrors *gr_exp_1sd = new TGraphAsymmErrors(n_N, x_N, y_exp, 0, 0, y_exp_1sdDn, y_exp_1sdUp);
        gr_exp_1sd->SetFillColor(kGreen+1);
        gr_exp_1sd->SetLineColor(kGreen+1);
        gr_exp_1sd->SetMarkerColor(kGreen+1);

        TGraphAsymmErrors *gr_exp_2sd = new TGraphAsymmErrors(n_N, x_N, y_exp, 0, 0, y_exp_2sdDn, y_exp_2sdUp);
        gr_exp_2sd->SetFillColor(kOrange);
        gr_exp_2sd->SetLineColor(kOrange);
        gr_exp_2sd->SetMarkerColor(kOrange);

        TGraphAsymmErrors *gr_obs = new TGraphAsymmErrors(n_N,x_N,y_obs,0,0,0,0);
        gr_obs->SetLineColor(kBlack);
        gr_obs->SetLineWidth(3);
        gr_obs->SetLineStyle(1);

        TGraphAsymmErrors *gr_xsec = new TGraphAsymmErrors(n_N, x_N, y_xsec, 0, 0, y_xsec_err_Dn, y_xsec_err_Up);
        gr_xsec->SetLineColor(kRed);

        TLegend *lg = new TLegend(0.2, 0.2, 0.7, 0.35);
        lg->SetBorderSize(0);
        lg->SetFillStyle(0);
        lg->AddEntry( gr_exp, "Expected limit", "l");
        lg->AddEntry( gr_exp_1sd, "68% expected", "f");
        lg->AddEntry( gr_exp_2sd, "95% expected", "f");
        lg->AddEntry( gr_xsec, "Theory (g_{R}=g_{L})", "l");

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
        hist_dummy->GetYaxis()->SetTitle("#sigma(pp#rightarrowW_{R})#bf{#it{#Beta}}(W_{R}#rightarroweeq#bar{q'}) (fb)");
        if(channel=="MuMu") hist_dummy->GetYaxis()->SetTitle("#sigma(pp#rightarrowW_{R})#bf{#it{#Beta}}(W_{R}#rightarrow#mu#muq#bar{q'}) (fb)");
        hist_dummy->GetYaxis()->SetRangeUser(1E-4, 1E4); 

        gr_exp_2sd->Draw("3same");
        gr_exp_1sd->Draw("3same");
        gr_exp->Draw("lsame");
        if(DrawObs) gr_obs->Draw("lsame");
        if(it_region==2){
          gr_exp_Resolved->SetLineColor(kRed);
          //gr_exp_Resolved->Draw("lsame");
          gr_exp_Boosted->SetLineColor(kBlue);
          //gr_exp_Boosted->Draw("lsame");
        }

        gr_xsec->Draw("lsame");

        TLatex str_m_WR;
        str_m_WR.SetNDC();
        str_m_WR.SetTextSize(0.035);
        str_m_WR.DrawLatex(0.55, 0.85, "m_{WR} = "+TString::Itoa(m_WR,10)+" GeV");
        if(channel=="EE")        latex_ch.DrawLatex(0.55, 0.80, region+" ee channel");
        else if(channel=="MuMu") latex_ch.DrawLatex(0.55, 0.80, region+" #mu#mu channel");

        lg->Draw();

        latex_CMSPriliminary.DrawLatex(0.15, 0.96, "#font[62]{CMS} #font[42]{#it{#scale[0.8]{Preliminary}}}");
        latex_Lumi.DrawLatex(0.73, 0.96, TotalLumi);

        c_1D_vsN->SaveAs(plotpath+"/1D_"+channel+"_"+region+"_WR"+TString::Itoa(m_WR,10)+"_Limit_vs_N.pdf");
        c_1D_vsN->SaveAs(plotpath+"/1D_"+channel+"_"+region+"_WR"+TString::Itoa(m_WR,10)+"_Limit_vs_N.C");
        c_1D_vsN->Close();

      } // END Loop over regions

    } // END Loop over WR

    //==== 1D : Limit vs WR, for each N
    //==== negative N means N=WR/2

    vector<double> test_Ns = {-500, 100, 200, 400};

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

      vector<TString> regions_2 = { "Resolved", "Boosted", "Combined"};
      TGraphAsymmErrors *gr_exp_Resolved;
      TGraphAsymmErrors *gr_exp_Boosted;

      for(unsigned int it_region=0; it_region<regions_2.size(); it_region++){

        TString region = regions_2.at(it_region);
        const int n_WR = lrsminfo_Half.size();

        double x_WR[n_WR], y_obs[n_WR], y_exp[n_WR], y_exp_1sdUp[n_WR], y_exp_1sdDn[n_WR], y_exp_2sdUp[n_WR], y_exp_2sdDn[n_WR];
        double y_xsec[n_WR], y_xsec_err_Up[n_WR], y_xsec_err_Dn[n_WR];

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
          y_exp[it_N] = this_result.limit_exp/2.;

          y_exp_1sdUp[it_N] = (this_result.limit_exp_1sdUp - this_result.limit_exp)/2.;
          y_exp_1sdDn[it_N] = (this_result.limit_exp - this_result.limit_exp_1sdDn)/2.;
          y_exp_2sdUp[it_N] = (this_result.limit_exp_2sdUp - this_result.limit_exp)/2.;
          y_exp_2sdDn[it_N] = (this_result.limit_exp - this_result.limit_exp_2sdDn)/2.;

          y_obs[it_N] = this_result.limit_obs/2.;

          y_xsec[it_N] = this_lrsm.xsec/2.;
          y_xsec_err_Up[it_N] = 0. * this_lrsm.xsec/2.;
          y_xsec_err_Dn[it_N] = 0. * this_lrsm.xsec/2.;

        }

        TGraphAsymmErrors *gr_exp = new TGraphAsymmErrors(n_WR,x_WR,y_exp,0,0,0,0);
        gr_exp->SetLineColor(kBlack);
        gr_exp->SetLineWidth(3);
        gr_exp->SetLineStyle(2);
        if(it_region==0){
          gr_exp_Resolved = (TGraphAsymmErrors *)gr_exp->Clone();
        }
        if(it_region==1){
          gr_exp_Boosted = (TGraphAsymmErrors *)gr_exp->Clone();
        }

        TGraphAsymmErrors *gr_exp_1sd = new TGraphAsymmErrors(n_WR, x_WR, y_exp, 0, 0, y_exp_1sdDn, y_exp_1sdUp);
        gr_exp_1sd->SetFillColor(kGreen+1);
        gr_exp_1sd->SetLineColor(kGreen+1);
        gr_exp_1sd->SetMarkerColor(kGreen+1);

        TGraphAsymmErrors *gr_exp_2sd = new TGraphAsymmErrors(n_WR, x_WR, y_exp, 0, 0, y_exp_2sdDn, y_exp_2sdUp);
        gr_exp_2sd->SetFillColor(kOrange);
        gr_exp_2sd->SetLineColor(kOrange);
        gr_exp_2sd->SetMarkerColor(kOrange);

        TGraphAsymmErrors *gr_obs = new TGraphAsymmErrors(n_WR,x_WR,y_obs,0,0,0,0);
        gr_obs->SetLineColor(kBlack);
        gr_obs->SetLineWidth(3);
        gr_obs->SetLineStyle(1);

        TGraphAsymmErrors *gr_xsec = new TGraphAsymmErrors(n_WR, x_WR, y_xsec, 0, 0, y_xsec_err_Dn, y_xsec_err_Up);
        gr_xsec->SetLineColor(kRed);

        TLegend *lg = new TLegend(0.2, 0.2, 0.7, 0.35);
        lg->SetBorderSize(0);
        lg->SetFillStyle(0);
        lg->AddEntry( gr_exp, "Expected limit", "l");
        lg->AddEntry( gr_exp_1sd, "68% expected", "f");
        lg->AddEntry( gr_exp_2sd, "95% expected", "f");
        lg->AddEntry( gr_xsec, "Theory (g_{R}=g_{L})", "l");

        TCanvas *c_1D_vsN = new TCanvas("c1", "", 800, 800);
        canvas_margin(c_1D_vsN);
        c_1D_vsN->cd();
        c_1D_vsN->SetLogy();

        TH1D *hist_dummy = new TH1D("hist_dummy", "", 7000, 0., 7000.);
        hist_dummy->Draw("hist");
        hist_axis(hist_dummy);
        hist_dummy->GetXaxis()->SetRangeUser(800,6800);
        hist_dummy->GetXaxis()->SetTitle("m_{W_{R}} (GeV)");
        hist_dummy->GetYaxis()->SetTitle("#sigma(pp#rightarrowW_{R})#bf{#it{#Beta}}(W_{R}#rightarroweeq#bar{q'}) (fb)");
        if(channel=="MuMu") hist_dummy->GetYaxis()->SetTitle("#sigma(pp#rightarrowW_{R})#bf{#it{#Beta}}(W_{R}#rightarrow#mu#muq#bar{q'}) (fb)");
        hist_dummy->GetYaxis()->SetRangeUser(1E-4, 1E4); 

        gr_exp_2sd->Draw("3same");
        gr_exp_1sd->Draw("3same");
        gr_exp->Draw("lsame");
        if(DrawObs) gr_obs->Draw("lsame");
        if(it_region==2){
          gr_exp_Resolved->SetLineColor(kRed);
          //gr_exp_Resolved->Draw("lsame");
          gr_exp_Boosted->SetLineColor(kBlue);
          //gr_exp_Boosted->Draw("lsame");
        }

        gr_xsec->Draw("lsame");

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
          outname = "1D_"+channel+"_"+region+"_N"+TString::Itoa(testN,10)+"_Limit_vs_WR";
        }
        else{
          outname = "1D_"+channel+"_"+region+"_HalfN_Limit_vs_WR";
        }

        c_1D_vsN->SaveAs(plotpath+"/"+outname+".pdf");
        c_1D_vsN->SaveAs(plotpath+"/"+outname+".C");
        c_1D_vsN->Close();

      }

    } // END Loop over regions

  } // END Loop channels


}


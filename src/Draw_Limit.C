#include "canvas_margin.h"
#include "LRSMSignalInfo.h"

void Draw_Limit(){

  TString inputfile = "2018_12_18_171155__ChargeBlind_sorted";


  bool UseAsymptotic = true;

  TString Method = "FullCLs";
  if(UseAsymptotic) Method = "Asymptotic";

  setTDRStyle();
  gStyle->SetOptStat(0);
  gStyle->SetPalette(kBeach);

  TH1::SetDefaultSumw2(true);
  TH1::AddDirectory(kFALSE);

  TString WORKING_DIR = getenv("PLOTTER_WORKING_DIR");
  TString dataset = getenv("CATANVERSION");
  TString ENV_PLOT_PATH = getenv("PLOT_PATH");

  TString filepath_result = WORKING_DIR+"/rootfiles/"+dataset+"/Limit/"+Method+"/"+inputfile+".txt";
  TString plotpath = ENV_PLOT_PATH+"/"+dataset+"/Limit/"+inputfile+"/"+Method+"/";

  gSystem->mkdir(plotpath, kTRUE);

  TString filename_thoery = WORKING_DIR+"/data/"+dataset+"/xsec_181004_Private_MuMu_NLO.txt";

  vector<TString> regions = {
    //"Combined",
    "OneLepton_AwayFatJetWithSFLepton100GeV",
    "TwoLepton_TwoJet_mllgt150",
    "OSandSSCombined",
    //"TwoLepton_TwoJet_mllgt150_OS",
    //"TwoLepton_TwoJet_mllgt150_SS",
  };
  vector<TString> aliases = {
    //"Combined",
    "Boosted-SR",
    "Resolved-SR (Charge blind)",
    "Resolved-SR (OS SS combined)",
    //"Resolved-SR (OS)",
    //"Resolved-SR (SS)",
  };
  vector<Color_t> colors = {
    kBlack,
    kGreen,
    kBlue,
    kRed,
  };

  vector<TString> channels = {
    //"EE",
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

    TLegend *lg = new TLegend(0.15, 0.62, 0.6, 0.94);
    lg->SetBorderSize(0);
    lg->SetFillStyle(0);

    //==== ATLAS
    string atlas_line;
    vector<double> atlas_mZp, atlas_mN;
    ifstream atlas_in(WORKING_DIR+"/data/"+dataset+"/Limit_ATLAT13TeV_"+channel+"_obs.txt");
    while(getline(atlas_in,atlas_line)){
      std::istringstream is( atlas_line );

      //atlas_line                                                                                                                                                                                          
      TString mark;
      double a,b;
      is >> mark;
      is >> a;
      is >> b;

      if(mark.Contains("TRUE")){
        atlas_mZp.push_back(a);
        atlas_mN.push_back(b);
      }
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

    //==== EXO17011

    string EXO17011_line;
    vector<double> EXO17011_mZp, EXO17011_mN;
    ifstream EXO17011_in(WORKING_DIR+"/data/"+dataset+"/Limit_EXO17011_"+channel+"_obs.txt");
    while(getline(EXO17011_in,EXO17011_line)){
      std::istringstream is( EXO17011_line );

      //EXO17011_line                                                                                                                                                                                          
      TString mark;
      double a,b;
      is >> mark;
      is >> a;
      is >> b;

      if(mark.Contains("TRUE")){
        EXO17011_mZp.push_back(a);
        EXO17011_mN.push_back(b);
      }
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

    //==== Results

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

          double mwr, mn, xsec;

          is >> mwr;
          is >> mn;
          is >> xsec;

          //==== pb -> fb
          xsec = 1000.*xsec;

          if(int(m_WR)==int(mwr) && int(m_N)==int(mn)){
            theory_xsec_found - true;
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

            //cout << m.limit_exp << "\t" << m.limit_exp_1sdUp << "\t" << m.limit_exp_1sdDn << "\t" << m.limit_exp_2sdUp << "\t" << m.limit_exp_2sdDn << endl;

            lrsminfo.LimitResults.push_back( m );

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
/*
    c1->SetTopMargin( 0.05 );
    c1->SetBottomMargin( 0.13 );
    c1->SetRightMargin( 0.05 );
    c1->SetLeftMargin( 0.16 );
*/
    c_2D->SetBottomMargin( 0.10 );
    c_2D->SetRightMargin( 0.13 );
    c_2D->SetLeftMargin( 0.11 );
    c_2D->cd();
    c_2D->SetLogz();

    //==== 2D binning
    //==== 1) WR
    int target_WR_max = 6000;
    int target_WR_min = 0;
    int target_WR_d_bin = 300;
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
/*
    hist_dummy->GetYaxis()->SetLabelSize(0.04);
    hist_dummy->GetYaxis()->SetTitleSize(0.06);
    hist_dummy->GetYaxis()->SetTitleOffset(1.10);
    hist_dummy->GetXaxis()->SetLabelSize(0.03);
    hist_dummy->GetXaxis()->SetTitleSize(0.05);
*/

    hist_dummy->GetYaxis()->SetLabelSize(0.035);
    hist_dummy->GetXaxis()->SetLabelSize(0.035);
    hist_dummy->GetYaxis()->SetTitleSize(0.04);
    hist_dummy->GetXaxis()->SetTitleSize(0.04);
    hist_dummy->GetYaxis()->SetTitleOffset(1.25);
    hist_dummy->GetXaxis()->SetTitleOffset(1.0);

    hist_dummy->Draw("hist");
    hist_dummy->GetYaxis()->SetTitle("m_{N} (GeV)");
    hist_dummy->GetXaxis()->SetRangeUser(400., 6000.);
    hist_dummy->GetYaxis()->SetRangeUser(100., 5000.);
    hist_dummy->GetZaxis()->SetRangeUser(1E-2, 20);
    hist_dummy->GetXaxis()->SetTitle("m_{W_{R}} (GeV)");

    for(unsigned int it_region=0; it_region<regions.size(); it_region++){

      TString region = regions.at(it_region);

      vector<double> vec_wr, vec_n;
      vector<double> vec_xsec;
      vector<double> vec_limit_exp, vec_limit_exp_1sdUp, vec_limit_exp_1sdDn, vec_limit_exp_2sdUp, vec_limit_exp_2sdDn;

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

          }
        
        }
      }


      const int n_mass = vec_wr.size();
      double arr_wr[n_mass], arr_n[n_mass];
      double arr_xsec[n_mass];
      double arr_limit_exp[n_mass], arr_limit_exp_1sdUp[n_mass], arr_limit_exp_1sdDn[n_mass], arr_limit_exp_2sdUp[n_mass], arr_limit_exp_2sdDn[n_mass];
      double arr_limit_exp_ratio[n_mass], arr_limit_exp_1sdUp_ratio[n_mass], arr_limit_exp_1sdDn_ratio[n_mass], arr_limit_exp_2sdUp_ratio[n_mass], arr_limit_exp_2sdDn_ratio[n_mass];


      for(unsigned int r=0;r<n_mass;r++){

        arr_wr[r] = vec_wr.at(r);
        arr_n[r] = vec_n.at(r);
        arr_xsec[r] = vec_xsec.at(r);
        arr_limit_exp[r] = vec_limit_exp.at(r);
        arr_limit_exp_1sdUp[r] = vec_limit_exp_1sdUp.at(r);
        arr_limit_exp_1sdDn[r] = vec_limit_exp_1sdDn.at(r);
        arr_limit_exp_2sdUp[r] = vec_limit_exp_2sdUp.at(r);
        arr_limit_exp_2sdDn[r] = vec_limit_exp_2sdDn.at(r);

        arr_limit_exp_ratio[r] = arr_xsec[r]/vec_limit_exp_1sdUp.at(r);
        arr_limit_exp_1sdUp_ratio[r] = arr_xsec[r]/vec_limit_exp_1sdUp.at(r);
        arr_limit_exp_1sdDn_ratio[r] = arr_xsec[r]/vec_limit_exp_1sdDn.at(r);
        arr_limit_exp_2sdUp_ratio[r] = arr_xsec[r]/vec_limit_exp_2sdUp.at(r);
        arr_limit_exp_2sdDn_ratio[r] = arr_xsec[r]/vec_limit_exp_2sdDn.at(r);

      }

      TGraph2D *gr2d_xsec = new TGraph2D(n_mass, arr_wr, arr_n, arr_xsec);

      TGraph2D *gr2d_limit_exp = new TGraph2D(n_mass, arr_wr, arr_n, arr_limit_exp);
      TGraph2D *gr2d_limit_exp_1sdUp = new TGraph2D(n_mass, arr_wr, arr_n, arr_limit_exp_1sdUp);
      TGraph2D *gr2d_limit_exp_1sdDn = new TGraph2D(n_mass, arr_wr, arr_n, arr_limit_exp_1sdDn);
      TGraph2D *gr2d_limit_exp_2sdUp = new TGraph2D(n_mass, arr_wr, arr_n, arr_limit_exp_2sdUp);
      TGraph2D *gr2d_limit_exp_2sdDn = new TGraph2D(n_mass, arr_wr, arr_n, arr_limit_exp_2sdDn);

      TGraph2D *gr2d_limit_exp_ratio = new TGraph2D(n_mass, arr_wr, arr_n, arr_limit_exp_ratio);
      TGraph2D *gr2d_limit_exp_1sdUp_ratio = new TGraph2D(n_mass, arr_wr, arr_n, arr_limit_exp_1sdUp_ratio);
      TGraph2D *gr2d_limit_exp_1sdDn_ratio = new TGraph2D(n_mass, arr_wr, arr_n, arr_limit_exp_1sdDn_ratio);
      TGraph2D *gr2d_limit_exp_2sdUp_ratio = new TGraph2D(n_mass, arr_wr, arr_n, arr_limit_exp_2sdUp_ratio);
      TGraph2D *gr2d_limit_exp_2sdDn_ratio = new TGraph2D(n_mass, arr_wr, arr_n, arr_limit_exp_2sdDn_ratio);

/*
      //==== Method 1) Use GetHistogram()
      TH2D *test = gr2d_limit_exp_ratio->GetHistogram();

      //==== Method 2) Use Interpolate
      TH2D *test = new TH2D("test", "", 700, 0., 7000.,  700, 0., 7000.);;
      for(int it_x=1; it_x<=test->GetXaxis()->GetNbins(); it_x++){
        double x_center = test->GetXaxis()->GetBinCenter(it_x);
        for(int it_y=1; it_y<=test->GetYaxis()->GetNbins(); it_y++){
          double y_center = test->GetYaxis()->GetBinCenter(it_y);
          if(y_center>=x_center) continue;
          //cout << x_center << "\t" << y_center << endl;
          double this_xsec = gr2d_xsec->Interpolate(x_center, y_center);
          double this_exp = gr2d_limit_exp->Interpolate(x_center, y_center);
          if(this_xsec==0||this_exp==0) continue;
          double this_ratio = this_xsec/this_exp;
          //cout << x_center << "\t" << y_center << "\t" << this_xsec << "\t" << this_exp << "\t" << this_ratio << endl;
          test->SetBinContent(it_x, it_y, this_ratio);
        }
      }
*/

      TH2D *hist2d_limit_exp_ratio = new TH2D("hist2d_limit_exp_ratio", "", bin_WR_n, bin_WR_min, bin_WR_max, bin_N_n, bin_N_min, bin_N_max);
      TH2D *hist2d_limit_exp = new TH2D("hist2d_limit_exp", "", bin_WR_n, bin_WR_min, bin_WR_max, bin_N_n, bin_N_min, bin_N_max);
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
          if(this_xsec<=0||this_exp<=0) continue;
          double this_ratio = this_xsec/this_exp;
          cout << region << "\t" << x_center << "\t" << y_center << "\t" << this_xsec << "\t" << this_exp << "\t" << this_ratio << endl;

          if(region=="Combined"){
            //cout << x_center << "\t" << y_center << "\t" << this_xsec << "\t" << this_exp << "\t" << this_ratio << endl;
          }

          hist2d_limit_exp_ratio->SetBinContent(it_x, it_y, this_ratio);
          hist2d_limit_exp->SetBinContent(it_x, it_y, this_exp);
        }
      }

      if(region=="Combined"){

        hist2d_limit_exp->GetZaxis()->SetRangeUser(1E-2, 20);
        //hist2d_limit_exp->GetZaxis()->SetTitleSize(0.01);
        hist2d_limit_exp->GetZaxis()->SetLabelSize(0.03);

        hist2d_limit_exp->Draw("colzsame");

      }

      double conts[] = {1.};  
      hist2d_limit_exp_ratio->SetContour(1,conts);
      hist2d_limit_exp_ratio->SetLineWidth(2);
      hist2d_limit_exp_ratio->SetLineColor(colors.at(it_region));
      hist2d_limit_exp_ratio->SetFillColor(colors.at(it_region));
      hist2d_limit_exp_ratio->Draw("cont2same");

      lg->AddEntry( hist2d_limit_exp_ratio, aliases.at(it_region), "l");

      //test->Draw("colzsametext");

    } // END Loop region

    gr_atlas->Draw("lsame");
    gr_EXO17011->Draw("lsame");

    lg->AddEntry( gr_atlas, "ATLAS 13 TeV", "l");
    lg->AddEntry( gr_EXO17011, "CMS 13 TeV (2016)", "l");

    //g0->Draw("same");

    lg->Draw();

    hist_dummy->Draw("axissame");

    c_2D->SaveAs(plotpath+"/"+channel+".pdf");
    c_2D->Close();



  } // END Loop channels


}

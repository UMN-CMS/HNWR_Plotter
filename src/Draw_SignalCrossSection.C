#include "canvas_margin.h"

void Draw_SignalCrossSection(){

  gStyle->SetOptStat(0);

  TString WORKING_DIR = getenv("PLOTTER_WORKING_DIR");
  TString catversion = getenv("CATVERSION");
  TString dataset = getenv("CATANVERSION");
  TString ENV_FILE_PATH = getenv("FILE_PATH");
  TString ENV_PLOT_PATH = getenv("PLOT_PATH");

  TString plotpath = ENV_PLOT_PATH+"/"+dataset+"/SignalCrossSection/";

  TString xsectxtfilepath = WORKING_DIR+"/data/"+dataset+"/xsec_181004_Private_MuMu_NLO.txt";

  string line_xsec;
  ifstream in_xsec(xsectxtfilepath);
  vector<double> vec_mWR, vec_mN, vec_xsec;
  while(getline(in_xsec,line_xsec)){
    std::istringstream is( line_xsec );

    double mwr, mn, xsec;

    is >> mwr;
    is >> mn;
    is >> xsec;

    //==== pb -> fb
    xsec = 1000.*xsec;

    vec_mWR.push_back( mwr );
    vec_mN.push_back( mn );
    vec_xsec.push_back( xsec );

  }

  if( !gSystem->mkdir(plotpath, kTRUE) ){
    cout
    << "###################################################" << endl
    << "Directoy " << plotpath << " is created" << endl
    << "###################################################" << endl
    << endl;
  }

  map< double, vector< pair<double, double> > > map__mWR__mN_Xsec;
  vector<Color_t> colors = {
    1, 2, 3, 4, 5, 6, 7, 8, 9, 11, 12, 13, 14,
    //kRed, kOrange, kGreen, kBlue, kViolet, kGray, kBlack, kCyan, kMagenta, k
  };

  for(unsigned int i=0; i<vec_mWR.size(); i++){

    double mWR = vec_mWR.at(i);
    double mN = vec_mN.at(i);
    double Xsec = vec_xsec.at(i);

    cout << mWR << "\t" << mN << "\t" << Xsec << endl;

    map__mWR__mN_Xsec[mWR].push_back( make_pair(mN, Xsec) );

  }

  TCanvas *c1_mN = new TCanvas("c1_mN", "", 600, 600);
  canvas_margin(c1_mN);
  TH1D *dummy_mN = new TH1D("dummy_mN", "", 10000, 0., 10000.);
  hist_axis(dummy_mN);
  dummy_mN->Draw("hist");
  dummy_mN->GetXaxis()->SetRangeUser(90., 7000.);
  dummy_mN->GetYaxis()->SetRangeUser(1e-4, 20000.);
  dummy_mN->GetXaxis()->SetTitle("m_{N} (GeV)");
  dummy_mN->GetYaxis()->SetTitle("#sigma(pp#rightarrow#muN#rightarrow#mu#mujj) (fb)");
  c1_mN->SetLogx();
  c1_mN->SetLogy();

  TLegend *lg = new TLegend(0.7, 0.55, 0.94, 0.93);
  lg->SetBorderSize(0);
  lg->SetFillStyle(0);

  int counter = 0;
  for(map< double, vector< pair<double, double>  > >::iterator it=map__mWR__mN_Xsec.begin(); it!=map__mWR__mN_Xsec.end(); it++){

    double mWR = it->first;
    vector< pair<double, double> > mN_Xsec = it->second;

    sort(mN_Xsec.begin(), mN_Xsec.end());

    cout << "This mWR = " << mWR << endl;
    const int n_this_mN = mN_Xsec.size();
    double x_mN[n_this_mN], y_Xsec[n_this_mN];

    double this_norm_xsec = 1.;

    for(unsigned int i=0; i<mN_Xsec.size(); i++){
      double mN = mN_Xsec.at(i).first;
      double Xsec = mN_Xsec.at(i).second;
      cout << mN << "\t" << Xsec << endl;
      x_mN[i] = mN;
      y_Xsec[i] = Xsec;
    }

    c1_mN->cd();
    TGraph *gr_mN = new TGraph(n_this_mN, x_mN,y_Xsec);
    gr_mN->SetLineWidth(2);
    gr_mN->SetMarkerStyle(20);
    gr_mN->SetMarkerSize(1);
    gr_mN->SetMarkerColor(colors.at(counter));
    gr_mN->SetLineColor(colors.at(counter));
    gr_mN->Draw("lpsame");

    TString alias = "m_{W_{R}} = "+TString::Itoa(mWR,10)+" GeV";

    lg->AddEntry(gr_mN, alias, "lp");

    counter++;

  }

  c1_mN->cd();
  lg->Draw();
  c1_mN->SaveAs(plotpath+"/Xsec.pdf");
  c1_mN->Close();

}

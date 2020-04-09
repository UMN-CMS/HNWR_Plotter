#include "canvas_margin.h"
#include "mylib.h"

void Draw_CompareEGammaTnP(){

  setTDRStyle();

  TString WORKING_DIR = getenv("PLOTTER_WORKING_DIR");
  TString dataset = getenv("CATANVERSION");
  TString ENV_PLOT_PATH = getenv("PLOT_PATH");

  TString Year = "2018";


  //==== POG Tight
  TString filename_POG = "POGTight.root";
  TString filename_Me = "POGTight.root";
  TString Which = "POGTight";
  vector<TString> legends = {
    "10 #leq p_{T} #leq 20 GeV",
    "20 #leq p_{T} #leq 35 GeV",
    "35 #leq p_{T} #leq 50 GeV",
    "50 #leq p_{T} #leq 100 GeV",
    "100 #leq p_{T} #leq 200 GeV",
    "200 #leq p_{T} #leq 500 GeV",
  };
  const int N_etabin = 10;
  double etabins[N_etabin+1] = {-2.5,-2.0,-1.566,-1.4442, -0.8, 0.0, 0.8, 1.4442, 1.566, 2.0, 2.5};


/*
  //==== HEEP
  TString filename_POG = "HEEP.root";
  TString filename_Me = "HEEP.root";
  TString Which = "HEEP";
  vector<TString> legends = {
    "35 #leq p_{T} #leq 50 GeV",
    "50 #leq p_{T} #leq 100 GeV",
    "100 #leq p_{T} #leq 200 GeV",
    "200 #leq p_{T} #leq 500 GeV",
  };
  const int N_etabin = 6;
  double etabins[N_etabin+1] = {-2.5, -1.566, -1.4442, 0.0, 1.4442, 1.566, 2.5};
*/

  TString base_filepath = WORKING_DIR+"/rootfiles/"+dataset+"/EGammaTnPComaprison/"+Year+"/";
  TString base_plotpath = ENV_PLOT_PATH+"/"+dataset+"/EGammaSFComparison/";
  gSystem->mkdir(base_plotpath, kTRUE);

  TCanvas *c1 = new TCanvas("c1", "", 800, 800);
  c1->Draw();
  c1->cd();

  TPad *c1_up;
  TPad *c1_down;
  c1_up = new TPad("c1", "", 0, 0.25, 1, 1);
  c1_down = new TPad("c1_down", "", 0, 0, 1, 0.25);

  canvas_margin(c1, c1_up, c1_down);

  c1_up->Draw();
  c1_down->Draw();

  c1_up->cd();
  TH1D *hist_dummpy_up = new TH1D("hist_dummpy_up", "", N_etabin, etabins);
  hist_dummpy_up->Draw("axis");
  hist_dummpy_up->GetYaxis()->SetRangeUser(0.7, 1.3);
  hist_dummpy_up->GetYaxis()->SetTitle("Scale factor");

  c1_down->cd();
  TH1D *hist_dummpy_down = new TH1D("hist_dummpy_down", "", N_etabin, etabins);
  hist_axis(hist_dummpy_up, hist_dummpy_down);
  hist_dummpy_down->GetXaxis()->SetTitle("SuperCluster #eta");
  hist_dummpy_down->GetYaxis()->SetTitle("POG/Reprod.");
  hist_dummpy_down->GetYaxis()->SetRangeUser(0.7, 1.3);
  hist_dummpy_down->Draw("axis");

  TFile *file_POG = new TFile(base_filepath+"/POG/"+filename_POG);
  TFile *file_Me = new TFile(base_filepath+"/Mine/"+filename_Me);

  TLegend *lg = new TLegend(0.5,0.80,0.95 ,0.92);
  lg->SetBorderSize(0);
  lg->SetFillStyle(0);

  for(unsigned int it_legend=0; it_legend<legends.size(); it_legend++){
    TString grname = "grSF1D_"+TString::Itoa(it_legend,10);
    TGraph *gr_POG = (TGraph *)file_POG->Get(grname);
    TGraph *gr_Me = (TGraph *)file_Me->Get(grname);

    gr_POG->SetLineStyle(3);
    gr_POG->SetLineWidth( gr_Me->GetLineWidth() );
    gr_POG->SetLineColor( gr_Me->GetLineColor() );
    lg->AddEntry(gr_Me, legends.at(it_legend), "l");

    c1_up->cd();
    gr_Me->Draw("psame");
    gr_POG->Draw("psame");

    TGraph *gr_Ratio = (TGraph *)gr_Me->Clone();
    int N_Me = gr_Me->GetN();
    int N_POG = gr_POG->GetN();
    cout << grname << endl;
    for(int i=0; i<N_Me; i++){

      double a_x, a_y, b_x, b_y;

      gr_Me->GetPoint(i, a_x, a_y);

      bool found = false;
      //cout << "Looking for a_x = " << a_x << endl;
      for(int j=0; j<N_POG; j++){
        gr_POG->GetPoint(j, b_x, b_y);
        bool IsSame = fabs(a_x-b_x)<0.001;
        //cout << "  b_x = " << b_x << " -> " << IsSame << endl;
        if(IsSame){
          found = true;
          break;
        }
      }

      double ratio = 1;
      if(!found){
        //cout << a_x << "\t" << b_x << endl;
      }
      else{
        ratio = a_y/b_y;
      }
      //cout << a_x << "\t" << ratio << endl;
      gr_Ratio->SetPoint( i, a_x, ratio );
    }
    c1_down->cd();
    gr_Ratio->Draw("psame");

  }


  c1->cd();
  lg->Draw();

  c1->SaveAs(base_plotpath+"/"+Which+"_"+Year+".pdf");
  c1->Close();


}

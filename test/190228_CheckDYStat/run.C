#include "canvas_margin.h"
#include "mylib.h"

void run(){

  TString Year = "2016";

  TString WORKING_DIR = getenv("PLOTTER_WORKING_DIR");
  TString dataset = getenv("CATANVERSION");
  TString ENV_PLOT_PATH = getenv("PLOT_PATH");

  TString base_filepath = "./";
  TString base_plotpath = ENV_PLOT_PATH+"/"+dataset+"/DYStatCheck/"+Year+"/";

/*
  if( !gSystem->mkdir(base_plotpath, kTRUE) ){
    cout
    << "###################################################" << endl
    << "Directoy " << base_plotpath << " is created" << endl
    << "###################################################" << endl
    << endl;
  }
*/

  TFile *file_Incl = new TFile(base_filepath+"MyPlayGround_WJets_MG.root");

  vector<double> HTbins = {
    0, 70, 100, 200, 400, 600, 800, 1200, 2500, 9999
  };

  TH1D *hist_HT = (TH1D *)file_Incl->Get("HT");

  for(unsigned int it_bin=0; it_bin<HTbins.size()-1; it_bin++){
    double x_l = HTbins.at(it_bin);
    double x_r = HTbins.at(it_bin+1);

    //cout<<"("<<x_l<<", "<<x_r<<")"<<endl;

    double y_thisbin = hist_HT->Integral( hist_HT->FindBin(x_l), hist_HT->FindBin(x_r) );
    cout << x_l<< "\t"<< x_r<< "\t" << y_thisbin << endl;
    


  }


}

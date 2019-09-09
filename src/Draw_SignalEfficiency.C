#include "canvas_margin.h"
#include "LRSMSignalInfo.h"

void Draw_SignalEfficiency(){

  TString Year = "2016";

  bool IsCR = false;

  setTDRStyle();
  gStyle->SetOptStat(0);

  TH1::SetDefaultSumw2(true);
  TH1::AddDirectory(kFALSE);

  TString WORKING_DIR = getenv("PLOTTER_WORKING_DIR");
  TString dataset = getenv("CATANVERSION");
  TString ENV_PLOT_PATH = getenv("PLOT_PATH");

/*
  //==== If you have test directory
  TString whichDir = "181029_TestSDMassEff";
  TString base_filepath = WORKING_DIR+"/rootfiles/"+dataset+"/Regions/Signal/"+whichDir+"/";
  TString base_plotpath = ENV_PLOT_PATH+"/"+dataset+"/SignalEfficiency/"+whichDir+"/";
*/


  //=== If not, use geenral
  TString base_filepath = WORKING_DIR+"/rootfiles/"+dataset+"/Regions/"+Year+"/Signal/";
  TString base_plotpath = ENV_PLOT_PATH+"/"+dataset+"/SignalEfficiency/"+Year+"/";


  if(IsCR){
    base_filepath = WORKING_DIR+"/rootfiles/"+dataset+"/Regions/Signal/";
    base_plotpath = ENV_PLOT_PATH+"/"+dataset+"/SignalEfficiency/CR/";
  }

  LRSMSignalInfo lrsminfo;
  lrsminfo.GetMassMaps();

  vector<TString> Configs = {
   "HNWR",
   //"HNWRWith0p5HoverE",
   //"HNWRdR0p5",
   //"HNWRdR0p6",
   //"HNWRdR0p7",
   
  };

  vector<TString> regions = {
    "Boosted_SR",
    "Resolved_SR",
  };
  vector<Color_t> colors = {
    kBlue,
    kRed,
  };
  vector<TString> region_aliases = {
    "Boosted SR",
    "Resolved SR",
  };

  if(IsCR){
    regions = {
      "OneLepton_AwayFatJetWithOFLepton",
      "TwoLepton_TwoJet_mlllt150",
      "HNWR_EMu_TwoLepton_TwoJet",
    };
    colors = {
      kBlue,
      kRed,
      kGreen,
    };
    region_aliases = {
      "Boosted CR",
      "Resolved SR (revered m(ll))",
      "Resolved SR (e#mu)",
    };
  }


  vector< TString > Suffixs = {

    "SingleElectron",
    "SingleMuon",

  };

  double xxx[1],yyy[1];
  TGraph *forlg = new TGraph(1,xxx,yyy);
  forlg->SetLineColor(kBlack);
  forlg->SetLineWidth(3);
  forlg->SetMarkerStyle(15);
  forlg->SetMarkerColor(kBlack);

  for(unsigned int it_Config=0; it_Config<Configs.size(); it_Config++){

    TString Config = Configs.at(it_Config);

    for(map< double, vector<double> >::iterator it=lrsminfo.maps_WR_to_N.begin(); it!=lrsminfo.maps_WR_to_N.end(); it++){

      double m_WR = it->first;
      vector<double> this_m_Ns = it->second;

      cout << "m_WR = " << m_WR << endl;

      for(unsigned int it_Suffix=0; it_Suffix<Suffixs.size(); it_Suffix++){

        TString Suffix = Suffixs.at(it_Suffix);

        cout << "  Suffix = " << Suffix << endl;

        TString channel = "ee";
        if(Suffix.Contains("SingleMuon")) channel = "#mu#mu";

        TString this_plotdir = base_plotpath+"/"+Config+"/"+Suffix;
        gSystem->mkdir(this_plotdir, kTRUE);

        TCanvas *c_eff = new TCanvas("c_eff", "", 800, 800);
        canvas_margin(c_eff);
        c_eff->cd();

        TH1D *hist_dummy = new TH1D("hist_dummy", "", 7000, 0., 7000.);
        hist_dummy->Draw("hist");
        hist_axis(hist_dummy);
        hist_dummy->GetXaxis()->SetRangeUser(this_m_Ns.at(0), this_m_Ns.at(this_m_Ns.size()-1));
        if(IsCR) hist_dummy->GetYaxis()->SetRangeUser(0., 0.2);
        hist_dummy->GetXaxis()->SetTitle("m_{N} (GeV)");
        hist_dummy->GetYaxis()->SetTitle("Efficiency");

        TLatex str_m_WR;
        str_m_WR.SetNDC();
        str_m_WR.SetTextSize(0.035);
        str_m_WR.DrawLatex(0.2, 0.85, "m_{WR} = "+TString::Itoa(m_WR,10)+" (GeV)");

        TLegend *lg = new TLegend(0.6, 0.7, 0.93, 0.93);
        lg->SetBorderSize(0);
        lg->SetFillStyle(0);

        if(!IsCR) lg->AddEntry(forlg, "Combined", "pl");

        const int n_mass = this_m_Ns.size();
        double x_total[n_mass], y_total[n_mass];
        for(int z=0;z<n_mass;z++){
          x_total[z] = 0;
          y_total[z] = 0;
        }

        for(unsigned int it_region=0; it_region<regions.size(); it_region++){

          TString region = regions.at(it_region);
          Color_t color = colors.at(it_region);
          TString region_alias = region_aliases.at(it_region);

          cout << "    region = " << region << endl;

          double x[n_mass], y[n_mass];

          for(int it_N=0; it_N<this_m_Ns.size(); it_N++){

            double m_N = this_m_Ns.at(it_N);

            //cout << m_N <<", ";

            TString this_filename = "HNWRAnalyzer_WRtoNLtoLLJJ_WR"+TString::Itoa(m_WR,10)+"_N"+TString::Itoa(m_N,10)+".root";
            TFile *file = new TFile(base_filepath+"/"+this_filename);

            //==== Get Gen flavour
            TH1D *hist_SignalFlavour = (TH1D *)file->Get("SignalFlavour");
            int N_gen_EE = hist_SignalFlavour->GetBinContent(2);
            int N_gen_MM = hist_SignalFlavour->GetBinContent(3);

            TString dirname = Config+"_"+Suffix+"_"+region;
            TH1D *hist = (TH1D *)file->Get(dirname+"/NEvent_"+dirname);

            TH1D *hist_TrigEff = (TH1D *)file->Get(dirname+"/TriggerEff_"+dirname);

            double this_yield = 0;
            double this_eff = 0;
            if(hist){
              double this_Den = 1.;
              if( Suffix.Contains("SingleElectron") ) this_Den = N_gen_EE;
              else this_Den = N_gen_MM;

              this_yield = hist->GetBinContent(1);
              this_eff = hist->GetEntries()/this_Den;
            }
            x[it_N] = m_N;
            y[it_N] = this_eff;

            cout << "      m_N = " << m_N << " : " << this_eff << endl;

            x_total[it_N] = m_N;
            y_total[it_N] += this_eff;

            if(IsCR) cout << region << "\t" << m_WR << "\t" << m_N << "\t" << this_yield << endl;

            file->Close();

          } // END Loop m_N

          //cout << endl;

          TGraph *gr_eff = new TGraph(n_mass, x, y);

          gr_eff->SetLineColor(color);
          gr_eff->SetLineWidth(3);
          gr_eff->SetMarkerStyle(15);
          gr_eff->SetMarkerColor(color);
          gr_eff->Draw("lpsame");

          lg->AddEntry(gr_eff, region_alias, "pl");

        } //END Loop region


        TGraph *gr_eff_total = new TGraph(n_mass,x_total,y_total);
        gr_eff_total->SetLineColor(kBlack);
        gr_eff_total->SetLineWidth(3);
        gr_eff_total->SetMarkerStyle(15); 
        gr_eff_total->SetMarkerColor(kBlack);
        if(!IsCR) gr_eff_total->Draw("lpsame");

        lg->Draw();

        hist_dummy->Draw("axissame");

        TLatex channelname;
        channelname.SetNDC();
        channelname.SetTextSize(0.037);
        channelname.DrawLatex(0.2, 0.88, channel);

        c_eff->SaveAs(this_plotdir+"/WR"+TString::Itoa(m_WR,10)+".pdf");
        c_eff->SaveAs(this_plotdir+"/WR"+TString::Itoa(m_WR,10)+".png");
        c_eff->Close();

      } // END Loop Suffix

    } // END Loop m_WR

  } // END Loop Config

}


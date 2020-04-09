#ifndef Plotter_h
#define Plotter_h

#include "TSystem.h"
#include "TFile.h"
#include "TDirectory.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TString.h"
#include "THStack.h"
#include "TLegend.h"
#include "TStyle.h"
#include <iostream>
#include <vector>
#include <map>
#include "canvas_margin.h"
#include "AnalysisInputs.h"
#include "mylib.h"
#include "LRSMSignalInfo.h"

using namespace std;

class Plotter{
public:
  //==== constructors
  Plotter();
  ~Plotter();

  enum signal_class{
    NoClass = -1,
    All = 0,
    Boosted = 1,
    Resolved = 2,
  };
  vector<signal_class> AllSignalClasses;
  signal_class CurrentSC;
  
  //==== functions for setting
  void make_bkglist();
  void SetRebins(TString filepath);
  void SetYAxis(TString filepath);
  void SetXAxis(TString filepath);
  void MakeRebins();
  void MakeYAxis();
  void MakeXAxis();
  AnalysisInputs analysisInputs;

  //==== functions for drawing
  void draw_hist();
  TString find_MCsector();
  void clear_legend_info();
  void fill_legend(TLegend *lg, TH1D *hist);
  void draw_legend(TLegend *lg, bool DrawData);
  void draw_canvas(THStack *mc_stack, TH1D *mc_staterror, TGraphAsymmErrors *mc_allerror, TH1D *hist_data, vector<TH1D *> hist_signal, TLegend *legend, bool DrawData);

  int n_rebin();
  double y_max();
  void SetXaxisRange(TH1D* hist);
  void SetXaxisRange(THStack* mc_stack);
  void SetXaxisRangeBoth(THStack* mc_stack, TH1D* hist);
  TH1D* MakeOverflowBin(TH1D* hist);
  TString DoubleToString(double dx);

  void mkdir(TString path);
  void make_plot_directory();

  void MakeTexFile(map< TString, TH1D * > hs);
  
  //==== variables
  bool DoDebug;
  unsigned int i_cut, i_var, i_file;
  TString filename_prefix, filename_suffix, data_class, plotpath, thiscut_plotpath;
  TString filename_skim;
  vector<TString> histname_suffix, bkglist, samples_to_use, histname, x_title, units, PrimaryDataset;
  vector<bool> drawdata, ApplyMCNormSF, drawratio;

  //=== Year
  int DataYear;
  TString TotalLumi();

  //==== channel type
  vector<int> LeptonChannels, RegionType;
  TString GetStringChannel(int A);
  TString GetStringRegion(int B);
  TString GetStringChannelRegion(int A, int B);

  vector<double> UseLogy;
  //map<TString, double> MCNormSF, MCNormSF_uncert, CalculatedSysts;
  vector< pair<TString, TString> > CutVarSkips;
  map< TString, vector<TString> > map_sample_string_to_list;
  map< TString, pair<TString, Color_t> > map_sample_string_to_legendinfo;
  vector<LRSMSignalInfo> signal_survive;
  vector<int> MCsector_first_index;
  vector<LRSMSignalInfo> signal_LRSMinfo;
  map< signal_class, vector<LRSMSignalInfo> > map_class_to_LRSMSignalInfo;
  vector<double> signa_xsec;
  vector<bool> signal_draw;
  vector<Color_t> signal_color;
  vector<Style_t> signal_style;
  map<TString, bool> MCsector_survive;
  TH1D *hist_for_legend_data;
  vector<TH1D*> hist_for_legend_bkg;
  vector<TH1D*> hist_for_legend_signal;

  TString path_rebins, path_y_axis, path_x_axis;

  map< TString, int > temp_rebins;
  map< TString, double > temp_y_maxs, temp_x_mins, temp_x_maxs;

  double default_y_max = 1000, default_y_min = 0.;
  double k_factor;
  double log_of_generation_mixing;

  bool ZeroDataCheckCut(double xlow, double xhigh);
  vector<double> GetRebinZeroBackground(THStack *mc_stack, TH1D *mc_staterror, TH1D *mc_allerror, TH1D *hist_data, vector<TH1D *> &hist_signal);

  //==== Syst
  double LumiError();
  vector<TString> Systs;

  TH1D *Rebin(TH1D *hist);

  void AddIfExist(map<TString, TH1D *>& map, TString key, TH1D *hist, TH1D *hist_Nominal, int Year, bool IsCorr);

  bool IsNoLSFCutPlot;
  bool DrawPostFit;

};
#endif

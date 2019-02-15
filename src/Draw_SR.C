#include "Plotter.cc"
#include <fstream>

void Draw_SR(int XXX=0){

  bool ScaleMC = false;
  bool UseBinnedDY = false;
  bool UsePromptMC = false;

  int Year = 2016;

  //==============
  //==== get env
  //==============
  
  TString WORKING_DIR = getenv("PLOTTER_WORKING_DIR");
  TString catversion = getenv("CATVERSION");
  TString dataset = getenv("CATANVERSION");
  TString ENV_PLOT_PATH = getenv("PLOT_PATH");

  //====================
  //==== decalre class
  //====================
  
  Plotter m;
  m.DoDebug = false;
  
  //=====================
  //==== set data class
  //=====================
  
  m.data_class = dataset+"/Regions/"+TString::Itoa(Year,10)+"/";
  
  //================================
  //==== set prefixes and suffixes
  //================================
  
  m.filename_prefix = "HNWRAnalyzer";
  m.filename_suffix = ".root";
  //m.filename_skim = "_SkimTree_LRSMHighPt";
  
  //=========================
  //==== set sample mapping
  //=========================

  if(Year==2016){
    m.map_sample_string_to_list["DY"] = {"DYJets10to50", "DYJets"};
    m.map_sample_string_to_list["WJets_MG"] = {"WJets_MG"};
    m.map_sample_string_to_list["VV_incl"] = {"WZ_pythia", "ZZ_pythia", "WW_pythia"};
    m.map_sample_string_to_list["ttbar"] = {"TT_powheg"};

    m.map_sample_string_to_legendinfo["DY"] = make_pair("DY", kYellow);
    m.map_sample_string_to_legendinfo["VV_incl"] = make_pair("diboson", kSpring-1);
    m.map_sample_string_to_legendinfo["WJets_MG"] = make_pair("W", 870);
    m.map_sample_string_to_legendinfo["ttbar"] = make_pair("ttbar", kRed);
  }
  else if(Year==2017){

    m.map_sample_string_to_list["DY"] = {"DYJets10to50_MG", "DYJets"};
    m.map_sample_string_to_list["ZToLL"] = {"DYJets10to50_MG", "ZToLL"};
    m.map_sample_string_to_list["WJets_MG"] = {"WJets_MG"};
    m.map_sample_string_to_list["VV_incl"] = {"WZ_pythia", "ZZ_pythia", "WW_pythia"};
    m.map_sample_string_to_list["VV_excl"] = {"ZZTo2L2Q", "ZZTo4L_powheg", "WZTo2L2Q", "WZTo3LNu", "WWTo2L2Nu_powheg"};
    m.map_sample_string_to_list["ttbar"] = {"TTLL_powheg", "TTLJ_powheg", "TTJJ_powheg"};
    m.map_sample_string_to_list["VVV"] = {"WWW", "WWZ", "WZZ", "ZZZ"};
    m.map_sample_string_to_list["SingleTop"] = {"SingleTop_sch", "SingleTop_tW_antitop", "SingleTop_tW_top", "SingleTop_tch_antitop", "SingleTop_tch_top"};
    m.map_sample_string_to_list["ttX"] = {"ttW", "ttZ", "TTG"};
    m.map_sample_string_to_list["chargeflip"] = {"chargeflip"};
    m.map_sample_string_to_list["fake"] = {"fake"};

    m.map_sample_string_to_legendinfo["DY"] = make_pair("DY", kYellow);
    m.map_sample_string_to_legendinfo["ZToLL"] = make_pair("DY", kYellow);
    m.map_sample_string_to_legendinfo["WJets_MG"] = make_pair("W", kCyan);
    m.map_sample_string_to_legendinfo["VV_incl"] = make_pair("diboson", kSpring-1);
    m.map_sample_string_to_legendinfo["VV_excl"] = make_pair("diboson", kSpring-1);
    m.map_sample_string_to_legendinfo["ttbar"] = make_pair("ttbar", kRed);
    m.map_sample_string_to_legendinfo["VVV"] = make_pair("triboson", kMagenta);
    m.map_sample_string_to_legendinfo["SingleTop"] = make_pair("singletop", kRed+2);
    m.map_sample_string_to_legendinfo["ttX"] = make_pair("ttX", kOrange+2);
    m.map_sample_string_to_legendinfo["chargeflip"] = make_pair("Mismeas. sign bkgd.", kYellow);
    m.map_sample_string_to_legendinfo["fake"] = make_pair("Misid. lepton background", 870);

  }

  //===============================
  //==== set and make sample list
  //===============================

  //==== _Di<Lepton>_<JetSel>_<ifOffZ>_<charge>

  //==== One Lepton
  if(XXX==0){
    m.samples_to_use = {"VVV", "VV_incl", "ttX", "SingleTop", "WJets_MG", "DY", "ttbar"};

    m.histname_suffix = {

      "HNWR_SingleMuon_Boosted",
      "HNWR_SingleMuon_Boosted_TwoLepton",

      "HNWR_SingleElectron_Boosted",
      "HNWR_SingleElectron_Boosted_TwoLepton",

/*
      "HNWR_SingleElectron_Resolved",
      "HNWR_SingleMuon_Resolved",
*/

    };

  }

  //==== Two Lepton OS
  if(XXX==1){
    m.samples_to_use = {"SingleTop", "VVV", "WJets_MG", "VV_incl", "ttbar", "DY"};

    m.histname_suffix = {

      "HNWR_SingleElectron_Resolved",
      "HNWR_SingleMuon_Resolved",

    };

  }

  //==== Two Lepton SS
  if(XXX==2){
    m.samples_to_use = {"VVV", "VV_incl", "fake", "chargeflip"};

    m.histname_suffix = {

      "HNWR_SingleElectron_TwoLepton_TwoJet_mllgt150_SS",
      "HNWR_SingleMuon_TwoLepton_TwoJet_mllgt150_SS",

    };

  }

  //============================
  //==== set variables to draw
  //============================
  
  m.histname = {
    "NEvent", "nPileUp", "nPV",
    "Lepton_0_Pt", "Lepton_0_Eta", "Lepton_0_TrkRelIso",
    "Lepton_1_Pt", "Lepton_1_Eta", "Lepton_1_TrkRelIso",
    "dPhi_ll",
    "Jet_0_Pt", "Jet_0_Eta",
    "Jet_1_Pt", "Jet_1_Eta",
    "HNFatJet_Pt", "HNFatJet_Eta", "HNFatJet_Mass", "HNFatJet_SDMass",
    "ZCand_Mass", "dR_ll",
    "MET", "HT",
    "MT",
    "Jet_Size", "NBJets",
    "NCand_Mass", "WRCand_Mass",
    "NCand_Pt", "WRCand_Pt",
    "LSFFatJet_Size",
  };

  m.x_title = {
    "# of events", "# of PU", "# of PV",
    "Leading lepton p_{T} (GeV)", "Leading lepton #eta", "Leading lepton TrkRelIso",
    "Subleading lepton p_{T} (GeV)", "Subleading lepton #eta", "Subleading lepton TrkRelIso",
    "#DeltaR(l_{1},l_{2})",
    "Leading jet p_{T} (GeV)", "Leading jet #eta",
    "Subleading jet p_{T} (GeV)", "Subleading jet #eta",
    "Away AK8 jet p_{T} (GeV)", "Away AK8 jet #eta", "Away AK8 jet Mass", "Away AK8 jet SFMass",
    "m(ll) (GeV)", "#DeltaR(ll)",
    "#slash{E}_{T}^{miss} (GeV)", "H_{T} (GeV)",
    "m_{T} (GeV)",
    "# of jets", "# of b-tagged jets",
    "m_{N} (GeV)", "m_{W_{R}} (GeV)",
    "p_{T} of N (GeV)", "p_{T} of W (GeV)",
    "# of merged jets",
  };

  m.units = {
    "int", "int", "int",
    "GeV", "", "",
    "GeV", "", "",
    "",
    "GeV", "",
    "GeV", "",
    "GeV", "", "GeV", "GeV",
    "GeV", "GeV",
    "GeV", "GeV",
    "GeV",
    "int", "int",
    "GeV", "GeV",
    "GeV", "GeV",
    "",
  };

/*
  m.histname = {
    "Lepton_0_Pt", "Lepton_1_Pt",
    "N_Mass", "ZP_Mass",
  };
  m.x_title = {
    "Leading lepton p_{T} (GeV)", "Subleading lepton p_{T} (GeV)",
    "m_{N} (GeV)", "m_{Z'} (GeV)",
  };
  m.units = {
    "GeV", "GeV",
    "GeV", "GeV",
  };
*/

  m.histname = {
    "WRCand_Mass"
  };
  m.x_title = {
    "m_{WR} (GeV)",
  };
  m.units = {
    "GeV",
  };


  for(unsigned int i=0; i<m.histname_suffix.size(); i++){

    TString this_region = m.histname_suffix.at(i);
    //==== PD
    if(this_region.Contains("SingleElectron")){
      m.PrimaryDataset.push_back("SingleElectron");
    }
    else if(this_region.Contains("SingleMuon")){
      m.PrimaryDataset.push_back("SingleMuon");
    }
    else if(this_region.Contains("EMu")){
      m.PrimaryDataset.push_back("SingleMuon");
    }
    else{
      cout << "ERROR : PD not correct" << endl;
      return;
    }

    //==== RegionType : + when SS, - when OS
    bool IsSS = this_region.Contains("_SS");
    int int_IsSS = +1;
    if(!IsSS) int_IsSS = -1;

    if(this_region.Contains("Boosted")){
      if(this_region.Contains("SingleElectron")){
        m.LeptonChannels.push_back(int_IsSS*11);
        m.RegionType.push_back(10);
      }
      else if(this_region.Contains("SingleMuon")){
        m.LeptonChannels.push_back(int_IsSS*12);
        m.RegionType.push_back(10);
      }
      else{
        cout << "Boosted but WTF : " << this_region << endl;
        return;
      }
    }
    else if(this_region.Contains("Resolved")){
      if(this_region.Contains("SingleElectron")){
        m.LeptonChannels.push_back(int_IsSS*21);
        m.RegionType.push_back(20);
      }
      else if(this_region.Contains("SingleMuon")){
        m.LeptonChannels.push_back(int_IsSS*22);
        m.RegionType.push_back(20);
      }
      else{
        cout << "Boosted but WTF" << endl;
        return;
      }
    }
    else{
      m.RegionType.push_back(0);
    }

    //==== Log plot boolean
    if(XXX==0) m.UseLogy.push_back(1);
    if(XXX==1) m.UseLogy.push_back(1);
    if(XXX==2) m.UseLogy.push_back(1);

    if(ScaleMC) m.ApplyMCNormSF.push_back(true);
    else m.ApplyMCNormSF.push_back(false);

    m.drawdata.push_back(false);

    m.drawratio.push_back(true);

  }

/*
  //==== FIXME test
  m.histname = {"m_lljj_lljjWclosest", "m_llj"};
  m.x_title = {"m(l^{#pm}l^{#pm}W_{jet}) (GeV)", "m(l^{#pm}l^{#pm}W_{jet}) (GeV)"};
  m.units = {"GeV", "GeV"};
*/

  cout << "m.histname.size() = " <<  m.histname.size() << endl;
  cout << "m.x_title.size() = " << m.x_title.size() << endl;
  cout << "m.units.size() = " << m.units.size() << endl;
  if(m.histname.size()==m.x_title.size() && m.x_title.size()==m.units.size()){
    cout << "--> OKAY" << endl;
  }
  else{
    cout << "--> WRONG" << endl;
    return;
  }
  cout << "Histname\tXTitle\tUnit" << endl;
  for(unsigned int i=0; i<m.histname.size(); i++){
    cout << m.histname.at(i) << "\t" << m.x_title.at(i) << "\t" << m.units.at(i) << endl;
  }

  //====================
  //==== make bkg list
  //====================

  m.make_bkglist();

  //=====================
  //==== Fill MCNorm SF
  //=====================

  m.analysisInputs.SetMCSF(WORKING_DIR+"/data/"+dataset+"/"+TString::Itoa(Year,10)+"/MCSF.txt", m.bkglist);

  //======================
  //==== Get Systematics
  //======================

  m.analysisInputs.SetCalculatedSysts(WORKING_DIR+"/data/"+dataset+"/"+TString::Itoa(Year,10)+"/Syst.txt");

  //=============================
  //==== set signal mass points
  //=============================

  LRSMSignalInfo lrsminfo;
  lrsminfo.GetMassMapsPlot();
  //vector<Color_t> colors_WR = {kGreen, kViolet, kGray, kOrange};
  vector<Color_t> colors_WR = {kBlack, kGray, kGray+2, kSpring};

  int it_sig=-1;
  for(map< double, vector<double> >::iterator it=lrsminfo.maps_WR_to_N.begin(); it!=lrsminfo.maps_WR_to_N.end(); it++){

    it_sig++;

    double m_WR = it->first;
    vector<double> this_m_Ns = it->second;

    for(int it_N=0; it_N<this_m_Ns.size(); it_N++){

      double m_N = this_m_Ns.at(it_N);

      LRSMSignalInfo lrsminfo;
      lrsminfo.prod_channel="SchWR";
      lrsminfo.generator="aMCNLO";
      lrsminfo.lep_channel = "EE";
      lrsminfo.mass_WR = m_WR;
      lrsminfo.mass_N = m_N;
      lrsminfo.SetNames();

      m.signal_LRSMinfo.push_back(lrsminfo);
      m.signal_color.push_back(colors_WR.at(it_sig));
      m.signal_style.push_back(it_N+2);
      m.signal_draw.push_back(true);


      //==== copy MuMu from EE
      lrsminfo.lep_channel = "MuMu";
      lrsminfo.SetNames();
      m.signal_LRSMinfo.push_back(lrsminfo);
      m.signal_color.push_back(colors_WR.at(it_sig));
      m.signal_style.push_back(it_N+2);
      m.signal_draw.push_back(true);

    }

  }

  //=====================================
  //==== set signal mass for each class
  //=====================================

  for(unsigned int i=0; i<m.signal_LRSMinfo.size(); i++){
    LRSMSignalInfo this_lrsm = m.signal_LRSMinfo.at(i);

    double m_WR = this_lrsm.mass_WR;
    double m_N = this_lrsm.mass_N;

    if(m_N/m_WR < 0.3){
      m.map_class_to_LRSMSignalInfo[Plotter::Boosted].push_back( this_lrsm );
    }
    else{
      m.map_class_to_LRSMSignalInfo[Plotter::Resolved].push_back( this_lrsm );
    }

    //m.map_class_to_LRSMSignalInfo[Plotter::All].push_back( this_lrsm );

  }
  m.AllSignalClasses = {Plotter::Resolved, Plotter::Boosted};

  //=============
  //==== rebins
  //=============
  
  //==== script to generate rebins
  ofstream skeleton_rebins("./data/tmp_SR_rebins.txt", ios::trunc);
  for(unsigned int i=0; i<m.histname_suffix.size(); i++){
    for(unsigned int j=0; j<m.histname.size(); j++){
      skeleton_rebins
      //<< "m.rebins[make_pair(\""+m.histname_suffix.at(i)+"\", \""+m.histname.at(j)+"\")] = 1;" << endl;
      <<m.histname_suffix.at(i)<<"\t"<<m.histname.at(j)<<"\t"<<-999<<endl;
    }
  }
  skeleton_rebins.close();

  m.SetRebins(WORKING_DIR+"/data/"+dataset+"/"+TString::Itoa(Year,10)+"/SR_rebins.txt");

  //=============
  //==== y_maxs
  //=============
  
  //==== script to generate rebins
  ofstream skeleton_y_maxs("./data/tmp_SR_yaxis.txt", ios::trunc);
  for(unsigned int i=0; i<m.histname_suffix.size(); i++){
    for(unsigned int j=0; j<m.histname.size(); j++){
      skeleton_y_maxs
      //<< "  m.y_maxs[make_pair(\""+m.histname_suffix.at(i)+"\", \""+m.histname.at(j)+"\")] = 1;" << endl;
      <<m.histname_suffix.at(i)<<"\t"<<m.histname.at(j)<<"\t"<<-999<<endl;
    }
  }
  skeleton_y_maxs.close();


  //==== default max
  m.default_y_max = 20.;
  m.default_y_min = 0.;

  m.SetYAxis(WORKING_DIR+"/data/"+dataset+"/"+TString::Itoa(Year,10)+"/SR_yaxis.txt");

  //=============
  //==== x_mins
  //=============

  //==== script to generate rebins
  ofstream skeleton_x_mins("./data/tmp_SR_xaxis.txt", ios::trunc);
  for(unsigned int i=0; i<m.histname_suffix.size(); i++){
    for(unsigned int j=0; j<m.histname.size(); j++){
      skeleton_x_mins
      //<< "  m.x_mins[make_pair(\""+m.histname_suffix.at(i)+"\", \""+m.histname.at(j)+"\")] = 1;" << endl;
      <<m.histname_suffix.at(i)<<"\t"<<m.histname.at(j)<<"\t"<<-999<<"\t"<<-999<<endl;
    }
  }
  skeleton_x_mins.close();

  m.SetXAxis(WORKING_DIR+"/data/"+dataset+"/"+TString::Itoa(Year,10)+"/SR_xaxis.txt");

  //===============
  //==== k-factor
  //===============
  
  m.k_factor = 1;
  
  //=================================
  //==== mixing at generation level
  //=================================
  
  m.log_of_generation_mixing = 1.;
  
  //===============================
  //==== prepare plot directories
  //===============================

  m.plotpath = ENV_PLOT_PATH+"/"+dataset+"/SR/"+TString::Itoa(Year,10)+"/";
  m.make_plot_directory();
  
  //==========================
  //==== finally, draw plots
  //==========================
  
  m.draw_hist();
  
}







#include "Plotter.cc"
#include <fstream>

void Draw_CR(int Year=2016, int WhichRegion=0, bool UseDYPtReweight=false, bool ScaleMC=false){

  bool UsePromptMC = false;

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
  m.DataYear = Year;
  
  //=====================
  //==== set data class
  //=====================
  
  m.data_class = dataset+"/Regions/"+TString::Itoa(Year,10)+"/";
  
  //================================
  //==== set prefixes and suffixes
  //================================
  
  m.filename_prefix = "HNWRAnalyzer";
  m.filename_suffix = ".root";
  m.filename_skim = "_SkimTree_LRSMHighPt";

  //=========================
  //==== set sample mapping
  //=========================

  if(Year==2016){

    m.map_sample_string_to_list["ZJets_MG_HT"] = {"DYJets10to50_MG", "DYJets_MG_HT"};
    m.map_sample_string_to_list["ZJets_MG_HT_Reweighted"] = {"DYJets10to50_MG_Reweighted", "DYJets_MG_HT_Reweighted"};
    m.map_sample_string_to_list["WJets_MG"] = {"WJets_MG"};
    m.map_sample_string_to_list["WJets_MG_HT"] = {"WJets_MG_HT"};
    m.map_sample_string_to_list["VV_incl"] = {"WZ_pythia", "ZZ_pythia", "WW_pythia"};
    m.map_sample_string_to_list["VVV"] = {"WWW", "WWZ", "WZZ", "ZZZ"};
    m.map_sample_string_to_list["SingleTop"] = {"SingleTop_sch_Lep", "SingleTop_tW_antitop_NoFullyHad", "SingleTop_tW_top_NoFullyHad", "SingleTop_tch_antitop_Incl", "SingleTop_tch_top_Incl"};
    m.map_sample_string_to_list["ttX"] = {"ttWToLNu", "ttWToQQ", "ttZ"};
    m.map_sample_string_to_list["ttbar"] = {"TTLL_powheg", "TTLJ_powheg"};
    m.map_sample_string_to_list["EMuMethod"] = {"EMuMethod_LowWRCR_TTLX_powheg"};

    m.map_sample_string_to_legendinfo["ZJets_MG_HT"] = make_pair("Z+Jets", kYellow);
    m.map_sample_string_to_legendinfo["ZJets_MG_HT_Reweighted"] = make_pair("Z+Jets", kYellow);
    m.map_sample_string_to_legendinfo["WJets_MG"] = make_pair("W+Jets", 870);
    m.map_sample_string_to_legendinfo["WJets_MG_HT"] = make_pair("W+Jets", 870);
    m.map_sample_string_to_legendinfo["VV_incl"] = make_pair("diboson", kSpring-1);
    m.map_sample_string_to_legendinfo["VVV"] = make_pair("triboson", kMagenta);
    m.map_sample_string_to_legendinfo["SingleTop"] = make_pair("singletop", kRed+2);
    m.map_sample_string_to_legendinfo["ttX"] = make_pair("ttX", kOrange+2);
    m.map_sample_string_to_legendinfo["ttbar"] = make_pair("ttbar", kRed);
    m.map_sample_string_to_legendinfo["EMuMethod"] = make_pair("ttbar", kRed);

  }
  else if(Year==2017){

    //==== TODO Now using JetBinned for 2017. Must be fixed
    m.map_sample_string_to_list["ZJets_MG_HT"] = {"DYJets10to50_MG", "DYJets_MG_JetBinned"};
    m.map_sample_string_to_list["ZJets_MG_HT_Reweighted"] = {"DYJets10to50_MG_Reweighted", "DYJets_MG_JetBinned_Reweighted"};
    //==== TODO Now using inclusive for 2018. Must be fixed
    m.map_sample_string_to_list["WJets_MG"] = {"WJets_MG"};
    m.map_sample_string_to_list["WJets_MG_HT"] = {"WJets_MG"};
    m.map_sample_string_to_list["VV_incl"] = {"WZ_pythia", "ZZ_pythia", "WW_pythia"};
    m.map_sample_string_to_list["VVV"] = {"WWW", "WWZ", "WZZ", "ZZZ"};
    m.map_sample_string_to_list["SingleTop"] = {"SingleTop_sch_Lep", "SingleTop_tW_antitop_NoFullyHad", "SingleTop_tW_top_NoFullyHad" ,"SingleTop_tch_antitop_Incl" ,"SingleTop_tch_top_Incl"};
    m.map_sample_string_to_list["ttX"] = {"ttW", "ttZ"};
    m.map_sample_string_to_list["ttbar"] = {"TTLL_powheg", "TTLJ_powheg"};
    m.map_sample_string_to_list["EMuMethod"] = {"EMuMethod_LowWRCR_TTLX_powheg"};

    m.map_sample_string_to_legendinfo["ZJets_MG_HT"] = make_pair("Z+Jets", kYellow);
    m.map_sample_string_to_legendinfo["ZJets_MG_HT_Reweighted"] = make_pair("Z+Jets", kYellow);
    m.map_sample_string_to_legendinfo["WJets_MG"] = make_pair("W+Jets", 870);
    m.map_sample_string_to_legendinfo["WJets_MG_HT"] = make_pair("W+Jets", 870);
    m.map_sample_string_to_legendinfo["VV_incl"] = make_pair("diboson", kSpring-1);
    m.map_sample_string_to_legendinfo["VVV"] = make_pair("triboson", kMagenta);
    m.map_sample_string_to_legendinfo["SingleTop"] = make_pair("singletop", kRed+2);
    m.map_sample_string_to_legendinfo["ttX"] = make_pair("ttX", kOrange+2);
    m.map_sample_string_to_legendinfo["ttbar"] = make_pair("ttbar", kRed);
    m.map_sample_string_to_legendinfo["EMuMethod"] = make_pair("ttbar", kRed);

  }
  else if(Year==2018){

    m.map_sample_string_to_list["ZJets_MG_HT"] = {"DYJets10to50_MG", "DYJets_MG_HT"};
    m.map_sample_string_to_list["ZJets_MG_HT_Reweighted"] = {"DYJets10to50_MG_Reweighted", "DYJets_MG_HT_Reweighted"};
    m.map_sample_string_to_list["WJets_MG"] = {"WJets_MG"};
    m.map_sample_string_to_list["WJets_MG_HT"] = {"WJets_MG_HT"};
    m.map_sample_string_to_list["VV_incl"] = {"WZ_pythia", "ZZ_pythia", "WW_pythia"};
    m.map_sample_string_to_list["VVV"] = {"WWW", "WWZ", "WZZ", "ZZZ"};
    m.map_sample_string_to_list["SingleTop"] = {"SingleTop_sch_Lep", "SingleTop_tW_antitop_Incl", "SingleTop_tW_top_Incl", "SingleTop_tch_antitop_Incl", "SingleTop_tch_top_Incl"};
    m.map_sample_string_to_list["ttX"] = {"ttW", "ttZ"};
    m.map_sample_string_to_list["ttbar"] = {"TTLL_powheg", "TTLJ_powheg"};
    m.map_sample_string_to_list["EMuMethod"] = {"EMuMethod_LowWRCR_TTLX_powheg"};

    m.map_sample_string_to_legendinfo["ZJets_MG_HT"] = make_pair("Z+Jets", kYellow);
    m.map_sample_string_to_legendinfo["ZJets_MG_HT_Reweighted"] = make_pair("Z+Jets", kYellow);
    m.map_sample_string_to_legendinfo["WJets_MG"] = make_pair("W+Jets", 870);
    m.map_sample_string_to_legendinfo["WJets_MG_HT"] = make_pair("W+Jets", 870);
    m.map_sample_string_to_legendinfo["VV_incl"] = make_pair("diboson", kSpring-1);
    m.map_sample_string_to_legendinfo["VVV"] = make_pair("triboson", kMagenta);
    m.map_sample_string_to_legendinfo["SingleTop"] = make_pair("singletop", kRed+2);
    m.map_sample_string_to_legendinfo["ttX"] = make_pair("ttX", kOrange+2);
    m.map_sample_string_to_legendinfo["ttbar"] = make_pair("ttbar", kRed);
    m.map_sample_string_to_legendinfo["EMuMethod"] = make_pair("ttbar", kRed);

  }

  //===============================
  //==== set and make sample list
  //===============================

  //==== _Di<Lepton>_<JetSel>_<ifOffZ>_<charge>

  //==== DY CR, log scale
  if(WhichRegion==0){

    m.samples_to_use = {"VVV", "VV_incl", "ttX", "SingleTop", "WJets_MG_HT", "ttbar", "ZJets_MG_HT"};
    if(UseDYPtReweight) m.samples_to_use = {"VVV", "VV_incl", "ttX", "SingleTop", "WJets_MG_HT", "ttbar", "ZJets_MG_HT_Reweighted"};

    m.histname_suffix = {

      "HNWR_SingleElectron_Resolved_DYCR",
      "HNWR_SingleMuon_Resolved_DYCR",
      "HNWR_SingleElectron_Boosted_DYCR",
      "HNWR_SingleMuon_Boosted_DYCR",


/*

      //==== Resolved EMu
      "HNWR_EMu_Resolved_SR",

      //==== Boosted EMu
      "HNWR_SingleElectron_EMu_Boosted_CR",
      "HNWR_SingleMuon_EMu_Boosted_CR",

      //==== Resolved EMu, but DYCR. This is dominated by ttbar, so it is here..
      //==== This region is not important
      "HNWR_EMu_Resolved_DYCR",

      //==== LowWRCR emu regions
      "HNWR_EMu_Resolved_LowWRCR",
      "HNWR_SingleElectron_EMu_Boosted_LowWRCR",
      "HNWR_SingleMuon_EMu_Boosted_LowWRCR",

      //==== Resolved LowWRCR for validation
      "HNWR_SingleElectron_Resolved_LowWRCR",
      "HNWR_SingleMuon_Resolved_LowWRCR",

      //==== Boosted LowWRCR for validation
      "HNWR_SingleElectron_Boosted_LowWRCR",
      "HNWR_SingleMuon_Boosted_LowWRCR",

*/






    };

  }

  //==== EMu
  if(WhichRegion==1){

    m.samples_to_use = {"VVV", "VV_incl", "ttX", "SingleTop", "WJets_MG_HT", "ZJets_MG_HT", "ttbar"};
    if(UseDYPtReweight) m.samples_to_use = {"VVV", "VV_incl", "ttX", "SingleTop", "WJets_MG_HT", "ZJets_MG_HT_Reweighted", "ttbar"};

    m.histname_suffix = {

      //==== Resolved EMu
      "HNWR_EMu_Resolved_SR",

      //==== Boosted EMu
      "HNWR_SingleElectron_EMu_Boosted_CR",
      "HNWR_SingleMuon_EMu_Boosted_CR",

      //==== Resolved EMu, but DYCR. This is dominated by ttbar, so it is here..
      //==== This region is not important
      "HNWR_EMu_Resolved_DYCR",

      //==== LowWRCR emu regions
      "HNWR_EMu_Resolved_LowWRCR",
      "HNWR_SingleElectron_EMu_Boosted_LowWRCR",
      "HNWR_SingleMuon_EMu_Boosted_LowWRCR",

    };

  }

  //==== EMu Validation (i.e., LowWR)
  if(WhichRegion==2){

    m.samples_to_use = {"VVV", "VV_incl", "ttX", "SingleTop", "WJets_MG_HT", "ZJets_MG_HT", "EMuMethod"};
    if(UseDYPtReweight) m.samples_to_use = {"VVV", "VV_incl", "ttX", "SingleTop", "WJets_MG_HT", "ZJets_MG_HT_Reweighted", "EMuMethod"};

    m.histname_suffix = {

      //==== Resolved LowWRCR for validation
      "HNWR_SingleElectron_Resolved_LowWRCR",
      "HNWR_SingleMuon_Resolved_LowWRCR",

      //==== Boosted LowWRCR for validation
      "HNWR_SingleElectron_Boosted_LowWRCR",
      "HNWR_SingleMuon_Boosted_LowWRCR",

    };

  }

  //============================
  //==== set variables to draw
  //============================

  m.histname = {
    "NEvent", "nPileUp", "nPV", "N_VTX",
    "Lepton_0_Pt", "Lepton_0_Eta", "Lepton_0_TrkRelIso",
    "Lepton_1_Pt", "Lepton_1_Eta", "Lepton_1_TrkRelIso",
    "dPhi_ll",
    "Jet_0_Pt", "Jet_0_Eta",
    "Jet_1_Pt", "Jet_1_Eta",
    "HNFatJet_Pt", "HNFatJet_Eta", "HNFatJet_Mass", "HNFatJet_SDMass",
    "ZCand_Pt", "ZCand_Mass", "dR_ll",
    "MET", "HT",
    "MT",
    "Jet_Size", "NBJets",
    "NCand_Mass", "WRCand_Mass",
    "NCand_Pt", "WRCand_Pt",
    "LSFFatJet_Size",
  };

  m.x_title = {
    "# of events", "# of PU", "# of PV", "# of vtx",
    "Leading lepton p_{T} (GeV)", "Leading lepton #eta", "Leading lepton TrkRelIso",
    "Subleading lepton p_{T} (GeV)", "Subleading lepton #eta", "Subleading lepton TrkRelIso",
    "#DeltaR(l_{1},l_{2})",
    "Leading jet p_{T} (GeV)", "Leading jet #eta",
    "Subleading jet p_{T} (GeV)", "Subleading jet #eta",
    "Away AK8 jet p_{T} (GeV)", "Away AK8 jet #eta", "Away AK8 jet Mass", "Away AK8 jet SFMass",
    "p_{T} of dilepton (GeV)", "m(ll) (GeV)", "#DeltaR(ll)",
    "#slash{E}_{T}^{miss} (GeV)", "H_{T} (GeV)",
    "m_{T} (GeV)",
    "# of jets", "# of b-tagged jets",
    "m_{N} (GeV)", "m_{W_{R}} (GeV)",
    "p_{T} of N (GeV)", "p_{T} of W (GeV)",
    "# of merged jets",
  };

  m.units = {
    "int", "int", "int", "int",
    "GeV", "", "",
    "GeV", "", "",
    "",
    "GeV", "",
    "GeV", "",
    "GeV", "", "GeV", "GeV",
    "GeV", "GeV", "GeV",
    "GeV", "GeV",
    "GeV",
    "int", "int",
    "GeV", "GeV",
    "GeV", "GeV",
    "",
  };

/*
  m.histname = {
    "NEvent",
  };
  m.x_title = {
    "# of events",
  };
  m.units = {
    "int",
  };
*/
/*
  m.histname = {
    "Lepton_0_Pt", "Lepton_1_Pt",
  };
  m.x_title = {
    "Leading lepton p_{T} (GeV)", "Subleading lepton p_{T} (GeV)",
  };
  m.units = {
    "GeV", "GeV",
  };
*/
/*
  m.histname = {
    "WRCand_Mass"
  };
  m.x_title = {
    "m_{W_{R}} (GeV)"
  };
  m.units = {
    "GeV",
  };
*/
/*
  m.histname = {
    "Jet_1_Pt",
  };
  m.x_title = {
    "p_{T} of dilepton (GeV)",
  };
  m.units = {
    "GeV",
  };
*/

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

    if(this_region=="HNWR_SingleElectron_Resolved_DYCR"){
      m.LeptonChannels.push_back(21);
      m.RegionType.push_back(11);
    }
    else if(this_region=="HNWR_SingleMuon_Resolved_DYCR"){
      m.LeptonChannels.push_back(22);
      m.RegionType.push_back(11);
    }
    else if(this_region=="HNWR_SingleElectron_Boosted_DYCR"){
      m.LeptonChannels.push_back(21);
      m.RegionType.push_back(21);
    }
    else if(this_region=="HNWR_SingleMuon_Boosted_DYCR"){
      m.LeptonChannels.push_back(22);
      m.RegionType.push_back(21);
    }
    else if(this_region=="HNWR_EMu_Resolved_SR"){
      m.LeptonChannels.push_back(23);
      m.RegionType.push_back(12);
    }
    else if(this_region=="HNWR_SingleElectron_EMu_Boosted_CR"){
      m.LeptonChannels.push_back(23);
      m.RegionType.push_back(23);
    }
    else if(this_region=="HNWR_SingleMuon_EMu_Boosted_CR"){
      m.LeptonChannels.push_back(23);
      m.RegionType.push_back(22);
    }
    else if(this_region=="HNWR_EMu_Resolved_DYCR"){
      m.LeptonChannels.push_back(23);
      m.RegionType.push_back(11);
    }
    else if(this_region=="HNWR_EMu_Resolved_LowWRCR"){
      m.LeptonChannels.push_back(23);
      m.RegionType.push_back(13);
    }
    else if(this_region=="HNWR_SingleElectron_EMu_Boosted_LowWRCR"){
      m.LeptonChannels.push_back(23);
      m.RegionType.push_back(26);
    }
    else if(this_region=="HNWR_SingleMuon_EMu_Boosted_LowWRCR"){
      m.LeptonChannels.push_back(23);
      m.RegionType.push_back(25);
    }
    else if(this_region=="HNWR_SingleElectron_Resolved_LowWRCR"){
      m.LeptonChannels.push_back(21);
      m.RegionType.push_back(13);
    }
    else if(this_region=="HNWR_SingleMuon_Resolved_LowWRCR"){
      m.LeptonChannels.push_back(22);
      m.RegionType.push_back(13);
    }
    else if(this_region=="HNWR_SingleElectron_Boosted_LowWRCR"){
      m.LeptonChannels.push_back(21);
      m.RegionType.push_back(24);
    }
    else if(this_region=="HNWR_SingleMuon_Boosted_LowWRCR"){
      m.LeptonChannels.push_back(22);
      m.RegionType.push_back(24);
    }

    else{
      cout << "Wrong region = " << this_region << endl;
      return;
    }

    //==== Log plot boolean
    if(WhichRegion==0) m.UseLogy.push_back(1);
    if(WhichRegion==1) m.UseLogy.push_back(-1);
    if(WhichRegion==2) m.UseLogy.push_back(-1);
    if(WhichRegion==3) m.UseLogy.push_back(1);

    if(ScaleMC) m.ApplyMCNormSF.push_back(true);
    else m.ApplyMCNormSF.push_back(false);

    m.drawdata.push_back(true);

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
  m.Systs = {
    "JetRes",
    "JetEn",
    "MuonEn",
    "MuonIDSF",
    "MuonTriggerSF",
    "ElectronRes",
    "ElectronEn",
    "ElectronIDSF",
    "ElectronTriggerSF",
    "LSFSF",
    "PU",
  };

  //=============
  //==== rebins
  //=============
  
  //==== script to generate rebins
  ofstream skeleton_rebins("./data/tmp_CR_rebins.txt", ios::trunc);
  for(unsigned int i=0; i<m.histname_suffix.size(); i++){
    for(unsigned int j=0; j<m.histname.size(); j++){
      skeleton_rebins
      //<< "m.rebins[make_pair(\""+m.histname_suffix.at(i)+"\", \""+m.histname.at(j)+"\")] = 1;" << endl;
      <<m.histname_suffix.at(i)<<"\t"<<m.histname.at(j)<<"\t"<<-999<<endl;
    }
  }
  skeleton_rebins.close();

  m.SetRebins(WORKING_DIR+"/data/"+dataset+"/"+TString::Itoa(Year,10)+"/CR_rebins.txt");

  //=============
  //==== y_maxs
  //=============
  
  //==== script to generate rebins
  ofstream skeleton_y_maxs("./data/tmp_CR_yaxis.txt", ios::trunc);
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

  m.SetYAxis(WORKING_DIR+"/data/"+dataset+"/"+TString::Itoa(Year,10)+"/CR_yaxis.txt"); 

  //=============
  //==== x_mins
  //=============

  //==== script to generate rebins
  ofstream skeleton_x_mins("./data/tmp_CR_xaxis.txt", ios::trunc);
  for(unsigned int i=0; i<m.histname_suffix.size(); i++){
    for(unsigned int j=0; j<m.histname.size(); j++){
      skeleton_x_mins
      //<< "  m.x_mins[make_pair(\""+m.histname_suffix.at(i)+"\", \""+m.histname.at(j)+"\")] = 1;" << endl;
      <<m.histname_suffix.at(i)<<"\t"<<m.histname.at(j)<<"\t"<<-999<<"\t"<<-999<<endl;
    }
  }
  skeleton_x_mins.close();

  m.SetXAxis(WORKING_DIR+"/data/"+dataset+"/"+TString::Itoa(Year,10)+"/CR_xaxis.txt");

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

  m.plotpath = ENV_PLOT_PATH+"/"+dataset+"/CR/"+TString::Itoa(Year,10)+"/";;

  if(UseDYPtReweight) m.plotpath += "/DYPtReweight/";
  if(UsePromptMC) m.plotpath += "/UsePromptMC/";

  m.make_plot_directory();

  //==========================
  //==== finally, draw plots
  //==========================
  
  m.draw_hist();
  
}







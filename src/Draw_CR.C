#include "Plotter.cc"
#include <fstream>

void Draw_CR(int XXX=0, bool MakeShape=false){

  bool ScaleMC = false;
  bool UseBinnedDY = false;
  bool UsePromptMC = true;

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
  m.MakeShape = MakeShape;
  
  //=====================
  //==== set data class
  //=====================
  
  m.data_class = dataset+"/Regions/";
  
  //================================
  //==== set prefixes and suffixes
  //================================
  
  m.filename_prefix = "HNWRAnalyzer";
  m.filename_suffix = ".root";
  m.filename_skim = "_SkimTree_LRSMHighPt";

  //=========================
  //==== set sample mapping
  //=========================
  
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
  
  //===============================
  //==== set and make sample list
  //===============================

  //==== _Di<Lepton>_<JetSel>_<ifOffZ>_<charge>

  //==== One Lepton
  if(XXX==0){
    m.samples_to_use = {"DY", "VVV", "VV_incl", "ttX", "SingleTop", "WJets_MG", "ttbar"};
    if(UsePromptMC) m.samples_to_use = {"DY", "VVV", "VV_incl", "ttX", "SingleTop", "WJets_MG", "fake", "ttbar"};

    m.histname_suffix = {

      "HNWR_SingleMuon_OneLepton_AwayFatJetWithOFLepton",
      "HNWR_SingleElectron_OneLepton_AwayFatJetWithOFLepton",

      "HNWR_SingleMuon_OneLepton_AwayFatJetWithOFLepton100GeV",
      "HNWR_SingleElectron_OneLepton_AwayFatJetWithOFLepton100GeV",

/*
      "HNWR_EMu_TwoLepton_TwoJet_OS",
      "HNWR_SingleMuon_TwoLepton_TwoJet_mlllt150_OS",
      "HNWR_SingleElectron_TwoLepton_TwoJet_mlllt150_OS",

      "HNWR_EMu_TwoLepton_TwoJet_SS",
      "HNWR_SingleMuon_TwoLepton_TwoJet_mlllt150_SS",
      "HNWR_SingleElectron_TwoLepton_TwoJet_mlllt150_SS",
*/


    };

  }

  //==== Two Lepton OS
  if(XXX==1){
    m.samples_to_use = {"SingleTop", "VVV", "WJets_MG", "VV_incl", "ttbar", "DY"};
    if(UseBinnedDY) m.samples_to_use = {"SingleTop", "VVV", "WJets_MG", "VV_incl", "ttbar", "ZToLL"};
    if(UsePromptMC) m.samples_to_use = {"SingleTop", "VVV", "WJets_MG", "VV_incl", "fake", "ttbar", "DY"};

    m.histname_suffix = {
      "HNWR_EMu_TwoLepton_TwoJet_OS",
      "HNWR_SingleMuon_TwoLepton_TwoJet_mlllt150_OS",
      "HNWR_SingleElectron_TwoLepton_TwoJet_mlllt150_OS",
    };

  }

  //==== Two Lepton SS
  if(XXX==2){
    m.samples_to_use = {"VVV", "VV_incl", "fake", "chargeflip"};

    m.histname_suffix = {

      "HNWR_EMu_TwoLepton_TwoJet_SS",
      "HNWR_SingleMuon_TwoLepton_TwoJet_mlllt150_SS",
      "HNWR_SingleElectron_TwoLepton_TwoJet_mlllt150_SS",

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
  if(MakeShape){
    m.histname = {
      "ZP_Mass"
    };
    m.x_title = {
      "m_{Z'} (GeV)"
    };
    m.units = {
      "GeV",
    };
  }
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

    //==== RegionType : + when SS, - when OS
    bool IsSS = this_region.Contains("_SS");
    int int_IsSS = +1;
    if(!IsSS) int_IsSS = -1;

    if(this_region.Contains("OneLepton_AwayFatJet")){
      if(this_region.Contains("WithOF")){
        if(this_region.Contains("SingleElectron")){
          m.LeptonChannels.push_back(1);
          m.RegionType.push_back(12);
        }
        else if(this_region.Contains("SingleMuon")){
          m.LeptonChannels.push_back(2);
          m.RegionType.push_back(11);
        }
        else{
          cout << "OneLepton_AwayFatJet && WithOF but WTF : " << this_region << endl;
          return;
        }
      }
      else{
        m.RegionType.push_back(10);
        if(this_region.Contains("SingleElectron")){
          m.LeptonChannels.push_back(1);
        }
        else if(this_region.Contains("SingleMuon")){
          m.LeptonChannels.push_back(2);
        }
        else{
          cout << "OneLepton_AwayFatJet && !(WithOF) but WTF : " << this_region << endl;
          return;
        }
      }
    }
    else if(this_region.Contains("TwoLepton_TwoJet")){
      if(this_region.Contains("EMu")){
        m.LeptonChannels.push_back(int_IsSS*23);
        m.RegionType.push_back(22);
      }
      else if(this_region.Contains("mlllt150")){
        if(this_region.Contains("SingleElectron")){
          m.LeptonChannels.push_back(int_IsSS*21);
          m.RegionType.push_back(21);
        }
        else if(this_region.Contains("SingleMuon")){
          m.LeptonChannels.push_back(int_IsSS*22);
          m.RegionType.push_back(21);
        }
        else{
          cout << "TwoLepton_TwoJet && !EMu && mlllt150 but WTF" << endl;
          return;
        }
      }
      else if(this_region.Contains("mllgt150")){
        if(this_region.Contains("SingleElectron")){
          m.LeptonChannels.push_back(int_IsSS*21);
          m.RegionType.push_back(20);
        }
        else if(this_region.Contains("SingleMuon")){
          m.LeptonChannels.push_back(int_IsSS*22);
          m.RegionType.push_back(20);
        }
        else{
          cout << "TwoLepton_TwoJet && !EMu && mllgt150 but WTF" << endl;
          return;
        }
      }
      else{
        cout << "TwoLepton_TwoJet but WTF : " << this_region << endl;
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

  m.analysisInputs.SetMCSF(WORKING_DIR+"/data/"+dataset+"/MCSF.txt", m.bkglist);

  //======================
  //==== Get Systematics
  //======================

  m.analysisInputs.SetCalculatedSysts(WORKING_DIR+"/data/"+dataset+"/Syst.txt");

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

  m.SetRebins(WORKING_DIR+"/data/"+dataset+"/CR_rebins.txt");

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

  m.SetYAxis(WORKING_DIR+"/data/"+dataset+"/CR_yaxis.txt"); 

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

  m.SetXAxis(WORKING_DIR+"/data/"+dataset+"/CR_xaxis.txt");

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

  m.plotpath = ENV_PLOT_PATH+"/"+dataset+"/CR/";
  if(UseBinnedDY) m.plotpath = ENV_PLOT_PATH+"/"+dataset+"/CR/BinnedDY/";
  if(UsePromptMC) m.plotpath = ENV_PLOT_PATH+"/"+dataset+"/CR/UsePromptMC/";

  m.make_plot_directory();
  m.outputdir_for_shape = ENV_PLOT_PATH+"/"+dataset+"/FilesForShapes/CR/";
  
  //==========================
  //==== finally, draw plots
  //==========================
  
  m.draw_hist();
  
}







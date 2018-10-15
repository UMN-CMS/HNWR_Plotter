#include "Plotter.cc"
#include <fstream>

void Draw_Validation(int XXX=0){

  bool ScaleMC = false;

  bool UseBinnedDY = false;

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
  
  m.data_class = dataset+"/SKFlatValidation/";
  
  //================================
  //==== set prefixes and suffixes
  //================================
  
  m.filename_prefix = "SKFlatValidation";
  m.filename_suffix = ".root";
  
  //=========================
  //==== set sample mapping
  //=========================
  
  m.map_sample_string_to_list["DY"] = {"DYJets_10to50_MG", "DYJets"};
  m.map_sample_string_to_list["ZToLL_M_50_120"] = {"ZToLL_M_50_120"};
  m.map_sample_string_to_list["ZToLL_M_120_200"] = {"ZToLL_M_120_200"};
  m.map_sample_string_to_list["ZToLL_M_200_400"] = {"ZToLL_M_200_400"};
  m.map_sample_string_to_list["ZToLL_M_400_800"] = {"ZToLL_M_400_800"};
  m.map_sample_string_to_list["ZToLL_M_800_1400"] = {"ZToLL_M_800_1400"};
  m.map_sample_string_to_list["ZToLL_M_1400_2300"] = {"ZToLL_M_1400_2300"};
  m.map_sample_string_to_list["ZToLL_M_2300_3500"] = {"ZToLL_M_2300_3500"};
  m.map_sample_string_to_list["ZToLL_M_3500_4500"] = {"ZToLL_M_3500_4500"};
  m.map_sample_string_to_list["ZToLL_M_4500_6000"] = {"ZToLL_M_4500_6000"};
  m.map_sample_string_to_list["ZToLL_M_6000_Inf"] = {"ZToLL_M_6000_Inf"};
  m.map_sample_string_to_list["WJets_MG"] = {"WJets_MG"};
  m.map_sample_string_to_list["VV_incl"] = {"WZ_pythia", "ZZ_pythia", "WW_pythia"};
  m.map_sample_string_to_list["ttbar"] = {"TTLL_powheg", "TTLJ_powheg", "TTJJ_powheg"};

  m.map_sample_string_to_legendinfo["DY"] = make_pair("DY", kYellow);
  m.map_sample_string_to_legendinfo["ZToLL_M_50_120"] = make_pair("m(Z) : 50-120", kRed-2);
  m.map_sample_string_to_legendinfo["ZToLL_M_120_200"] = make_pair("m(Z) : 120-200", kRed-10);
  m.map_sample_string_to_legendinfo["ZToLL_M_200_400"] = make_pair("m(Z) : 200-400", kOrange);
  m.map_sample_string_to_legendinfo["ZToLL_M_400_800"] = make_pair("m(Z) : 400-800", kYellow);
  m.map_sample_string_to_legendinfo["ZToLL_M_800_1400"] = make_pair("m(Z) : 800-1400", kGreen+3);
  m.map_sample_string_to_legendinfo["ZToLL_M_1400_2300"] = make_pair("m(Z) : 1400-2300", kBlue+3);
  m.map_sample_string_to_legendinfo["ZToLL_M_2300_3500"] = make_pair("m(Z) : 2300-3500", kViolet);
  m.map_sample_string_to_legendinfo["ZToLL_M_3500_4500"] = make_pair("m(Z) : 3500-4500", kGray);
  m.map_sample_string_to_legendinfo["ZToLL_M_4500_6000"] = make_pair("m(Z) : 4500-6000", kGray+2);
  m.map_sample_string_to_legendinfo["ZToLL_M_6000_Inf"] = make_pair("m(Z) : 6000-Inf", kBlack);
  m.map_sample_string_to_legendinfo["WJets_MG"] = make_pair("W", 870);
  m.map_sample_string_to_legendinfo["VV_incl"] = make_pair("diboson", kSpring-1);
  m.map_sample_string_to_legendinfo["ttbar"] = make_pair("ttbar", kRed);
  
  //===============================
  //==== set and make sample list
  //===============================

  //==== _Di<Lepton>_<JetSel>_<ifOffZ>_<charge>

  if(XXX==0){
    if(UseBinnedDY)
      m.samples_to_use = {"WJets_MG", "VV_incl", "ttbar", "ZToLL_M_50_120", "ZToLL_M_120_200", "ZToLL_M_200_400", "ZToLL_M_400_800", "ZToLL_M_800_1400", "ZToLL_M_1400_2300", "ZToLL_M_2300_3500", "ZToLL_M_3500_4500", "ZToLL_M_4500_6000", "ZToLL_M_6000_Inf"};
    else
      m.samples_to_use = {"WJets_MG", "VV_incl", "ttbar", "DY"};

    vector<TString> paramnames = {
      "POG",
      "POGHighPt",
      "HNPair",
      "HNWR",
    };

    vector<TString> regions = {
      "SingleMuon_OS",
      "SingleMuon_OnZ_OS",
      "SingleMuon_ZMassgt50_OS",
      "SingleMuon_WithBJet_METgt30_OS",
      "SingleElectron_OS",
      "SingleElectron_OnZ_OS",
      "SingleElectron_ZMassgt50_OS",
      "SingleElectron_WithBJet_METgt30_OS",
      "SingleMuon_SS",
      "SingleMuon_OnZ_SS",
      "SingleMuon_ZMassgt50_SS",
      "SingleMuon_WithBJet_METgt30_SS",
      "SingleElectron_SS",
      "SingleElectron_OnZ_SS",
      "SingleElectron_ZMassgt50_SS",
      "SingleElectron_WithBJet_METgt30_SS",

    };

    for(unsigned int i=0; i<paramnames.size(); i++){
      for(unsigned int j=0; j<regions.size(); j++){
        m.histname_suffix.push_back( paramnames.at(i)+"_"+regions.at(j) );
      }
    }

  }
  if(XXX==1){
    if(UseBinnedDY)
      m.samples_to_use = {"VV_incl", "ttbar", "ZToLL_M_50_120", "ZToLL_M_120_200", "ZToLL_M_200_400", "ZToLL_M_400_800", "ZToLL_M_800_1400", "ZToLL_M_1400_2300", "ZToLL_M_2300_3500", "ZToLL_M_3500_4500", "ZToLL_M_4500_6000", "ZToLL_M_6000_Inf", "WJets_MG"};
    else
      m.samples_to_use = {"VV_incl", "ttbar", "DY", "WJets_MG"};

    vector<TString> paramnames = {
      "POG",
      "POGHighPt",
      "HNPair",
      "HNWR",
    };

    vector<TString> regions = {
      "SingleMuon_W_CR",
      "SingleElectron_W_CR",
    };

    for(unsigned int i=0; i<paramnames.size(); i++){
      for(unsigned int j=0; j<regions.size(); j++){
        m.histname_suffix.push_back( paramnames.at(i)+"_"+regions.at(j) );
      }
    }

  }

  //============================
  //==== set variables to draw
  //============================
  
  m.histname = {
    "NEvent",
    "Lepton_0_Pt", "Lepton_0_Eta", "Lepton_0_RelIso", "Lepton_0_MiniRelIso", "Lepton_0_MVANoIso",
    "Lepton_1_Pt", "Lepton_1_Eta", "Lepton_1_RelIso", "Lepton_1_MiniRelIso", "Lepton_1_MVANoIso",
    "ZCand_Mass", "ZCand_Pt", "ZCand_Eta",
    "MET", "HT",
    "MT",
    "Jet_Size", "NBJets",
  };

  m.x_title = {
    "# of events",
    "Leading lepton p_{T} (GeV)", "Leading lepton #eta", "Leading lepton RelIso","Leading lepton MiniRelIso","Leading lepton MVA (NoIso)",
    "Subleading lepton p_{T} (GeV)", "Subleading lepton #eta", "Subleading lepton RelIso","Subleading lepton MiniRelIso","Subleading lepton MVA (NoIso)",
    "m(Z) (GeV)", "Z p_{T} (GeV)", "Z #eta",
    "#slash{E}_{T}^{miss} (GeV)", "H_{T} (GeV)",
    "m_{T} (GeV)",
    "# of jets", "# of b-tagged jets",
  };

  m.units = {
    "int",
    "GeV", "", "", "", "",
    "GeV", "", "", "", "",
    "GeV", "GeV", "",
    "GeV", "GeV",
    "GeV",
    "int", "int",
  };

  for(unsigned int i=0; i<m.histname_suffix.size(); i++){

    //==== PD
    if(m.histname_suffix.at(i).Contains("SingleMuon")){
      m.PrimaryDataset.push_back("SingleMuon");
      m.LeptonChannels.push_back(22); //FIXME
    }
    else if(m.histname_suffix.at(i).Contains("SingleElectron")){
      m.PrimaryDataset.push_back("SingleElectron");
      m.LeptonChannels.push_back(22); //FIXME
    }
    else{
      cout << "ERROR : PD not correct" << endl;
      return;
    }

    m.RegionType.push_back(0);

    //==== Log plot boolean
    if(XXX==0) m.UseLogy.push_back(1);
    if(XXX==1) m.UseLogy.push_back(1);

    if(ScaleMC) m.ApplyMCNormSF.push_back(true);
    else m.ApplyMCNormSF.push_back(false);

    //if(m.histname_suffix.at(i).Contains("Preselection")) m.drawdata.push_back(true);
    //else m.drawdata.push_back(false);

    //m.drawdata.push_back(false);

    m.drawdata.push_back(true);

    //if(m.histname_suffix.at(i).Contains("DiLepton")) m.drawratio.push_back(false);
    //else m.drawratio.push_back(true);

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

  //=============================
  //==== set signal mass points
  //=============================

  //=====================================
  //==== set signal mass for each class
  //=====================================

  for(unsigned int i=0; i<m.signal_LRSMinfo.size(); i++){
    LRSMSignalInfo this_lrsm = m.signal_LRSMinfo.at(i);

    m.map_class_to_LRSMSignalInfo[Plotter::All].push_back( this_lrsm );
  }
  m.AllSignalClasses = {Plotter::All};

  //=============
  //==== rebins
  //=============
  
  //==== script to generate rebins
  ofstream skeleton_rebins("./data/tmp_Validation_rebins.txt", ios::trunc);
  for(unsigned int i=0; i<m.histname_suffix.size(); i++){
    for(unsigned int j=0; j<m.histname.size(); j++){
      skeleton_rebins
      //<< "m.rebins[make_pair(\""+m.histname_suffix.at(i)+"\", \""+m.histname.at(j)+"\")] = 1;" << endl;
      <<m.histname_suffix.at(i)<<"\t"<<m.histname.at(j)<<"\t"<<-999<<endl;
    }
  }
  skeleton_rebins.close();

  m.SetRebins(WORKING_DIR+"/data/"+dataset+"/Validation_rebins.txt");

  //=============
  //==== y_maxs
  //=============
  
  //==== script to generate rebins
  ofstream skeleton_y_maxs("./data/tmp_Validation_yaxis.txt", ios::trunc);
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

  m.SetYAxis(WORKING_DIR+"/data/"+dataset+"/Validation_yaxis.txt"); 

  //=============
  //==== x_mins
  //=============


  //==== script to generate rebins
  ofstream skeleton_x_mins("./data/tmp_Validation_xaxis.txt", ios::trunc);
  for(unsigned int i=0; i<m.histname_suffix.size(); i++){
    for(unsigned int j=0; j<m.histname.size(); j++){
      skeleton_x_mins
      //<< "  m.x_mins[make_pair(\""+m.histname_suffix.at(i)+"\", \""+m.histname.at(j)+"\")] = 1;" << endl;
      <<m.histname_suffix.at(i)<<"\t"<<m.histname.at(j)<<"\t"<<-999<<"\t"<<-999<<endl;
    }
  }
  skeleton_x_mins.close();

  m.SetXAxis(WORKING_DIR+"/data/"+dataset+"/Validation_xaxis.txt");

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

  if(UseBinnedDY) m.plotpath = ENV_PLOT_PATH+"/"+m.data_class+"/BinnedDY/";
  else            m.plotpath = ENV_PLOT_PATH+"/"+m.data_class+"/InclusiveDY/";
  m.make_plot_directory();
  
  //==========================
  //==== finally, draw plots
  //==========================
  
  m.draw_hist();
  
}







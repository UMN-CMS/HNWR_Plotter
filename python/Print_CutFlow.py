import os,ROOT,math
import CMS_lumi, tdrstyle

WORKING_DIR = os.environ['PLOTTER_WORKING_DIR']
dataset = os.environ['CATANVERSION']
ENV_PLOT_PATH = os.environ['PLOT_PATH']

Years = [
'2016',
'2017',
'2018',
]

Bkgds = [
'TTLX_powheg',
'DYJets_MG_HT_Reweighted',
]
BkgdAliases = [
'\\ttbar',
'DY'
]

Suffixs = [
'SingleElectron',
'SingleMuon',
]

LepChs = [
'dielectron',
'dimuon',
]

for Year in Years:

  base_filepath = WORKING_DIR+"/rootfiles/"+dataset+"/Regions/"+Year

  Bkgds = []
  BkgdAliases = []

  for i_S in range(0,len(Suffixs)):

    Suffix = Suffixs[i_S]
    LepCh = LepChs[i_S]

    CutSets = [
      [
    'NoCut', 
    'METFilter',
    'NTightLeptonIsTwo_'+Suffix,
    'NTightLeptonIsTwo_'+Suffix+'_PassTrigger',
    'NTightLeptonIsTwo_'+Suffix+'_TwoAK4Jets',
    'NTightLeptonIsTwo_'+Suffix+'_dRSeparation',
    'NTightLeptonIsTwo_'+Suffix+'_mllGT200',
    'NTightLeptonIsTwo_'+Suffix+'_mWRGT800',
    'NTightLeptonIsTwo_'+Suffix+'_mWRGT800_mll400',
      ],


      [
    'NoCut',
    'METFilter',
    'NotResolved',
    'NotResolved_'+Suffix,
    'NotResolved_'+Suffix+'_PassTrigger',
    'NotResolved_'+Suffix+'_NoLowmll',
    'NotResolved_'+Suffix+'_HasMergedJet',
    'NotResolved_'+Suffix+'_NoExtraTight',
    'NotResolved_'+Suffix+'_HasSFLooseLepton',
    'NotResolved_'+Suffix+'_NoHasOFLooseLepton',
    'NotResolved_'+Suffix+'_mllGT200',
    'NotResolved_'+Suffix+'_mWRGT800',
      ],

    ]

    Regions = [
    'resolved',
    'boosted',
    ]

    AliasSets = [

      [
    'No cut',
    'METFilter',
    'Number of tight lepton is two',
    'Trigger fired',
    'Number of AK4 jets $\\ge$ 2',
    '$\\Delta\\mathrm{R} > 0.4$',
    '$m(\\ell\\ell)>200~\\GeV$',
    '$m(\\ell\\ell\\jet\\jet)>800~\\GeV$',
    '$m(\\ell\\ell)>400~\\GeV$',
      ],

      [
    'No cut',
    'METFilter',
    'Not resolved',
    'Leading lepton is electron (muon)',
    'Trigger fired',
    'No $60 < m(\\ell_{\\mathrm{Tight}}\\ell_{\\mathrm{Loose}}) < 150~\\GeV$',
    'Merged AK8 jet with $\\Delta\\phi>2.0$',
    'No extra tight lepton',
    'Loose SF lepton inside the merged jet',
    'No loose OF lepton inside the merged jet',
    '$m(\\ell_{\\mathrm{Tight}}\\ell_{\\mathrm{Loose}})>200~\\GeV$',
    '$m(\\ell_{\\mathrm{Tight}}\\Jet)>800~\\GeV$',
      ],

    ]

    SignalsSets = [
      [
    [3000, 1400], [4000, 2000], [5000, 3000],
      ],

      [
    [3000,200], [4000,200], [5000,200],
      ],

    ]

    SignalXsecs = [
      [
    1.427e-02, 1.370e-03, 1.407e-04,
      ],

      [
    2.301e-02, 3.049e-03, 6.707e-04,
      ],


    ]

    for i_CutSet in range(0,len(CutSets)):

      CutSet = CutSets[i_CutSet]
      Region = Regions[i_CutSet]
      Signals = SignalsSets[i_CutSet]
      Xsecs = SignalXsecs[i_CutSet]
      AliasSet = AliasSets[i_CutSet]

      print '''
\\begin{{table}}[htp]
\\topcaption{{Cutflow for {0} {1} with {2} samples.}}
\\label{{tab:{4}}}
\\centering

\\cmsTable{{

  \\begin{{tabular}}{{ {3} }}
    \\hline
'''.format(Region, LepCh, Year, 'c|'+'c'*len(Bkgds)+'|'+'c'*len(Signals),'cutflow'+Region+LepCh+Year)

      #### header
      header = 'Selection&'
      for Bkgd in BkgdAliases:
        header += Bkgd+'&'
      for Signal in Signals:
        mWR = Signal[0]
        mN = Signal[1]
        header += '(%d,%d)&' % (mWR,mN)
      header = header[:-1]
      print header+'\\\\'
      print '\\hline'

      #### Loop over cut
      for i_Cut in range(0,len(CutSet)):

        Cut = CutSet[i_Cut]
        Alias = AliasSet[i_Cut]

        row = Alias+'&'

        #### Bkgd
        for Bkgd in Bkgds:
          f = ROOT.TFile(base_filepath+'/HNWRAnalyzer_SkimTree_LRSMHighPt_'+Bkgd+'.root')
          h = f.Get('CutFlow/'+Cut+'_HNWR')
          y = 0.
          if h:
            y = h.GetBinContent(1)
          row += '%.2E' % (y)+'&'
          #row += '%1.2f' % (y)+'&'

        #### Signal
        for i_Signal in range(0,len(Signals)):

          Signal = Signals[i_Signal]
          Xsec = Xsecs[i_Signal]

          mWR = Signal[0]
          mN = Signal[1]

          SignalDir = "Signal_EE"
          if Suffix=="SingleMuon":
            SignalDir = "Signal_MuMu"
          f = ROOT.TFile(base_filepath+'/'+SignalDir+'/HNWRAnalyzer_WRtoNLtoLLJJ_WR'+str(mWR)+'_N'+str(mN)+'.root')
          h = f.Get('CutFlow/'+Cut+'_HNWR')
          y = 0.
          if h:
            y = h.GetBinContent(1)
          row += '%1.2f' % (y*Xsec)+'&'

        row = row[:-1]
        print row+'\\\\'

      print '''
    \\hline
  \\end{tabular}
}
\\end{table}'''

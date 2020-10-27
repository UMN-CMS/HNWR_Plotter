import os,ROOT,math,mylib
import CMS_lumi, tdrstyle

WORKING_DIR = os.environ['PLOTTER_WORKING_DIR']
dataset = os.environ['CATANVERSION']
ENV_PLOT_PATH = os.environ['PLOT_PATH']

Years = [
'2016',
'2017',
'2018',
]

#RUNMODE = 0 ## Data
RUNMODE = 1 ## MC

Bkgd = 'DYJets_MG_HT_Reweighted'
#Bkgd = 'DYJets_MG_HT'
#Bkgd = 'DYJets'

Suffixs = [
#'SingleElectron',
'SingleMuon',
]

LepChs = [
#'dielectron',
'dimuon',
]

for i_S in range(0,len(Suffixs)):

  Suffix = Suffixs[i_S]
  LepCh = LepChs[i_S]

  CutSets = [
    [
  'NoCut',
  'METFilter',
  'NotResolved',
  'NotResolved_TightLeptonExist',
  'NotResolved_'+Suffix,
  'NotResolved_'+Suffix+'_PassTrigger',
  'NotResolved_'+Suffix+'_HasLowMll',
  'NotResolved_'+Suffix+'_HasLowMll_HasBackToBackAK8Jet',
  'NotResolved_'+Suffix+'_DYCR',
    ],

  ]

  Regions = [
  'boosted',
  ]

  AliasSets = [

    [
  'No cut',
  'METFilter',
  'Not resolved',
  'Tight lepton exists',
  'Leading lepton is electron (muon)',
  'Trigger fired',
  '$60 < m(\\ell_{\\mathrm{Tight}}\\ell_{\\mathrm{Loose}}) < 150\\GeV$',
  'Merged AK8 jet with $\\Delta\\phi>2.0$',
  '$m(\\ell \\Jet) > 800\\GeV$',
    ],

  ]

  for i_CutSet in range(0,len(CutSets)):

    CutSet = CutSets[i_CutSet]
    Region = Regions[i_CutSet]
    AliasSet = AliasSets[i_CutSet]

    print '''
\\begin{{table}}[htp]
\\topcaption{{Cutflow for {0} {1}}}
\\label{{tab:{3}}}
\\centering

\\cmsTable{{

  \\begin{{tabular}}{{ {2} }}
    \\hline
'''.format(Region, LepCh, 'c|ccc','TESTcutflow'+Region+LepCh)

    #### header
    header = 'Selection & 2016 & 2017 & 2018'
    print header+' \\\\'
    print '\\hline'

    #### Loop over cut
    PreviousYields = []
    for i_Cut in range(0,len(CutSet)):

      Cut = CutSet[i_Cut]
      Alias = AliasSet[i_Cut]

      row = Alias+'&'

      ThisYields = []
      for i_Year in range(0,len(Years)):
        Year = Years[i_Year]
        base_filepath = '/data6/Users/jskim/SKFlatOutput/Run2Legacy_v4/HNWRAnalyzer/'+Year+'/NoSkim__/'
        fname = 'HNWRAnalyzer_data_'+Suffix+'.root' if (RUNMODE==0) else 'HNWRAnalyzer_'+Bkgd+'.root'

        f = ROOT.TFile(base_filepath+'/'+fname)

        h = f.Get('CutFlow/'+Cut+'_HNWR')
        y = 0.
        if h:
          y = h.GetBinContent(1)
        y = y/float(mylib.TotalLumi(int(Year)))

        ThisYields.append(y)

      if i_Cut==0:
        PreviousYields = ThisYields

      for i_Year in range(0,len(Years)):
        row += '%1.3f' % ( ThisYields[i_Year] )+'&'
        #row += '%1.1f' % ( 100. * ThisYields[i_Year] / PreviousYields[i_Year] )+'&'

      PreviousYields = ThisYields

      row = row[:-1]
      print row+'\\\\'

    print '''
    \\hline
  \\end{tabular}
}
\\end{table}'''

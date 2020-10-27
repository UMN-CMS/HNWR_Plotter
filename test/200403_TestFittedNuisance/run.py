import os,ROOT
from array import array

Years = [
"2016",
"2017",
"2018",
]

Channels = [
"EE",
"MuMu",
]

Regions = [
"Resolved_SR",
"Boosted_SR",
]

Samples = [
'TTLX_powheg',
'VVV',
'VV',
'ttX',
'SingleTop',
'WJets_MG_HT',
'DYJets_MG_HT_Reweighted',
]

for Sample in Samples:

  for Year in Years:

    print '-------------------------------------------'
    print 'Making '+Sample+' for '+Year+'...'
    this_filename = Year+'/HNWRAnalyzer_SkimTree_LRSMHighPt_'+Sample+'.root'
    print '- filename = '+this_filename

    out = ROOT.TFile(this_filename,'RECREATE')

    for Channel in Channels:

      f = ROOT.TFile('fitDiagnostics_YearCombined_'+Channel+'_WR2000_N1000.root')
      dir_shapes_fit_b = f.Get('shapes_prefit')
      # shapes_fit_b
      # shapes_prefit

      PD = 'SingleElectron'
      if Channel=='MuMu':
        PD = 'SingleMuon'

      print '- Channel = '+Channel

      for Region in Regions:

        print '  - Region = '+Region

        massbins = [0, 800, 1000, 1200, 1400, 1600, 2000, 2400, 2800, 3200, 8000]
        if "Boosted" in Region:
          if Year=="2016" and Channel=="EE" and Region=="Boosted_SR":
            massbins = [0, 800, 1000, 1200, 1500, 1800, 8000]
          else:
            massbins = [0, 800, 1000, 1200, 1500, 1700, 8000]

        dirName = 'HNWR_'+PD+'_'+Region
        print '    - mass bin =',
        print massbins

        dir_fitDiag_thisShapes = dir_shapes_fit_b.Get('Run'+Year+'_'+Channel+'_'+Region)
        hist_binned = dir_fitDiag_thisShapes.Get(Sample)

        hist_massbinned = ROOT.TH1D('WRCand_Mass_'+dirName, '', len(massbins)-1, array('d', massbins))
        for i in range(0, hist_massbinned.GetXaxis().GetNbins()):
          ix = i+1
          hist_massbinned.SetBinContent( ix, hist_binned.GetBinContent(ix) )
          hist_massbinned.SetBinError( ix, hist_binned.GetBinError(ix) )
          #print hist_binned.GetBinContent(ix)

        out.mkdir(dirName)
        out.cd(dirName)
        hist_massbinned.Write()
        out.cd()


      #### Closing fitDiag file
      f.Close()

    #### Closing output file
    out.Close()

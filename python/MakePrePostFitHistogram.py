import os,ROOT
from array import array

WORKING_DIR = os.environ['PLOTTER_WORKING_DIR']
dataset = os.environ['CATANVERSION']
FILE_PATH = os.environ['FILE_PATH']
PLOT_PATH = os.environ['PLOT_PATH']

PreORPosts = [
"Pre",
"Post",
]

for PreORPost in PreORPosts:

  Mass_Resolved = "WR5000_N3000"
  Mass_Boosted = "WR5000_N100"

  basedir = FILE_PATH+'/'+dataset+'/FitDiagnostics/'
  outdirBase = FILE_PATH+'/'+dataset+'/'+PreORPost+'Fit/'

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
  "ResolvedEMuCR",
  "BoostedEMuCR",
  "Resolved_SR",
  "Boosted_SR",
  "ResolvedDYCR",
  "BoostedDYCR",
  ]

  Samples = [
  'TT_TW',
  'DYJets_MG_HT_Reweighted_Reshaped',
  'NonPrompt',
  'Others',
  'total_background',
  ]

  for Sample in Samples:

    for Year in Years:

      print '-------------------------------------------'
      print 'Making '+Sample+' for '+Year+'...'
      this_filename = 'HNWRAnalyzer_SkimTree_LRSMHighPt_'+Sample+'.root'
      print '- filename = '+this_filename

      this_outdirBase = outdirBase+'/'+Year+'/'
      os.system('mkdir -p '+this_outdirBase)

      out = ROOT.TFile(this_outdirBase+this_filename,'RECREATE')

      for Channel in Channels:

        shapedirName = 'shapes_fit_b'
        if 'Pre' in PreORPost:
          shapedirName = 'shapes_prefit'

        PD = 'SingleElectron'
        if Channel=='MuMu':
          PD = 'SingleMuon'

        print '- Channel = '+Channel

        for Region in Regions:

          Mass = Mass_Resolved if ('Resolved' in Region) else Mass_Boosted
          shapefilename = 'fitDiagnostics_YearCombined_card_CRAdded_'+Channel+'_Combined_'+Mass+'.root'
          if 'CROnly' in PreORPost:
            shapefilename = 'fitDiagnostics_CROnlyFit_YearCombined_card_CRAdded_'+Channel+'_Combined_'+Mass+'.root'

          f = ROOT.TFile(basedir+'/'+shapefilename)
          dir_shapes_fit_b = f.Get(shapedirName)

          print '  - Region = '+Region

          massbins = [0, 800, 1000, 1200, 1400, 1600, 2000, 2400, 2800, 3200, 8000]
          if "Boosted" in Region:
            massbins = [0, 800, 1000, 1200, 1500, 1800, 8000]

          dirName = 'HNWR_'+PD+'_'+Region
          #### Exception for CRs
          if Region=="ResolvedEMuCR":
            if Channel=="EE":
              dirName = "HNWR_EMu_Resolved_SR"
              #continue
            else:
              dirName = "HNWR_EMu_Resolved_SR"
              continue
          elif Region=="BoostedEMuCR":
            if Channel=="EE":
              dirName = "HNWR_SingleMuon_EMu_Boosted_CR"
            if Channel=="MuMu":
              dirName = "HNWR_SingleElectron_EMu_Boosted_CR"
          elif 'DYCR' in Region:
            dirName = dirName.replace('DYCR','_DYCR')

          print '    - mass bin =',
          print massbins

          thisShapeDirName = 'Run'+Year+'_'+Channel+Region
          if "SR" in Region:
            thisShapeDirName = 'Run'+Year+'_'+Channel+'_'+Region
          print thisShapeDirName
          dir_fitDiag_thisShapes = dir_shapes_fit_b.Get(thisShapeDirName)
          ## hist_binned starts from 800 GeV
          hist_binned = dir_fitDiag_thisShapes.Get(Sample)

          hist_massbinned = ROOT.TH1D('WRCand_Mass_'+dirName, '', len(massbins)-1, array('d', massbins))
          for i in range(0, hist_massbinned.GetXaxis().GetNbins()):
            ## hist_binned starts from 800 GeV
            ix = i+1
            if ix==1:
              hist_massbinned.SetBinContent( ix, 0 )
              hist_massbinned.SetBinError( ix, 0 )
            else:
              hist_massbinned.SetBinContent( ix, hist_binned.GetBinContent(ix-1) )
              hist_massbinned.SetBinError( ix, hist_binned.GetBinError(ix-1) )
            #print hist_binned.GetBinContent(ix)

          out.mkdir(dirName)
          out.cd(dirName)
          hist_massbinned.Write()
          out.cd()


          #### Closing fitDiag file
          f.Close()

      #### Closing output file
      out.Close()

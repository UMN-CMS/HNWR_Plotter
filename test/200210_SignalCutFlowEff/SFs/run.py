import os,ROOT,tdrstyle
from array import array

tdrstyle.setTDRStyle()
ROOT.TH1.AddDirectory(False);

Years = [
"2016",
"2017",
"2018",
]

'''
#### ID SF
fileNames = [
'RunAveraged_SF_ID.root',
'RunBCDEF_SF_ID.root',
'RunABCD_SF_ID.root',
]
histNames = [
'NUM_HighPtID_DEN_genTracks_eta_pair_newTuneP_probe_pt',
'NUM_HighPtID_DEN_genTracks_pair_newTuneP_probe_pt_abseta',
'NUM_HighPtID_DEN_TrackerMuons_pair_newTuneP_probe_pt_abseta',
]
'''
'''
#### ISO SF
fileNames = [
'RunAveraged_SF_ISO.root',
'RunBCDEF_SF_ISO.root',
'RunABCD_SF_ISO.root',
]
histNames = [
'NUM_LooseRelTkIso_DEN_HighPtIDandIPCut_eta_pair_newTuneP_probe_pt',
'NUM_LooseRelTkIso_DEN_HighPtIDandIPCut_pair_newTuneP_probe_pt_abseta',
'NUM_LooseRelTkIso_DEN_HighPtIDandIPCut_pair_newTuneP_probe_pt_abseta',
]
'''

#### Trigger SF
fileNames = [
'HighPtMuonTriggerSF_Run2016.root',
'HighPtMuonTriggerSF_Run2017.root',
'HighPtMuonTriggerSF_Run2018.root',
]
histNames = [
'Eff_Data',
'Eff_Data',
'Eff_Data',
]
#histNames = [
#'Eff_MC',
#'Eff_MC',
#'Eff_MC',
#]

for i in range(0,3):
  Year = Years[i]
  fileName = fileNames[i]
  histName = histNames[i]

  h = ROOT.TFile(Year+'/'+fileName).Get(histName)

  c1 = ROOT.TCanvas('c1','',600,600)
  #c1.SetLogy()
  c1.cd()
  h.Draw("text")
  c1.SaveAs(Year+'.pdf')
  c1.Close()

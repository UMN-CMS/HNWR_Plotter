def IsCorrelated(syst):

  #==== Most of the systematics are Correlated

  #==== Let's return "false" for UNcorrelated
  #==== i.e., separate nuisance parameter FOR EACH YEAR
  if "JetRes" in syst:
    return False
  if "TriggerSF" in syst:
    return False
  if "LSFSF" in syst:
    return False
  #if "Lumi" in syst:
  #  return False
  if "DYNorm" in syst:
    return False
  if "NonPromptNorm" in syst:
    return False

  return True

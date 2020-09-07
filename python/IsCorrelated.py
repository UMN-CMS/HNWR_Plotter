def IsCorrelated(syst):

  #==== Most of the systematics are Correlated
  #==== Let's return "false" for noncorrelated

  if "JetRes" in syst:
    return False
  if "TriggerSF" in syst:
    return False
  if "LSFSF" in syst:
    return False
  if "Lumi" in syst:
    return False
  if "DYReshapeSyst" in syst:
    return False

  return True

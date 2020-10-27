#!/bin/bash
rsync -auv -e 'ssh -p 1240' jskim@147.47.242.42:/data4/Users/jskim/SKFlatOutput/Run2Legacy_v3/HNWRAnalyzer/2016/RunSyst__Signal__RunXsecSyst__RunNewPDF__*__ ./

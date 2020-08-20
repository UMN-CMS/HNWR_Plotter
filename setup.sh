#!/bin/bash

export CATANVERSION="Run2Legacy_v4__Default"

export PLOTTER_WORKING_DIR=`pwd`

if [[ $HOSTNAME == *"tamsa1"* ]] || [[ $HOSTNAME == *"tamsa2"* ]] ; then

  #### use cvmfs for root ####
  export CMS_PATH=/cvmfs/cms.cern.ch
  source $CMS_PATH/cmsset_default.sh
  export SCRAM_ARCH=slc7_amd64_gcc900
  export cmsswrel='cmssw-patch/CMSSW_10_4_0_patch1'
  export cmsswrel='cmssw/CMSSW_11_1_0_pre6'
  cd /cvmfs/cms.cern.ch/$SCRAM_ARCH/cms/$cmsswrel/src
  echo "@@@@ SCRAM_ARCH = "$SCRAM_ARCH
  echo "@@@@ cmsswrel = "$cmsswrel
  echo "@@@@ scram..."
  eval `scramv1 runtime -sh`
  cd -
  source /cvmfs/cms.cern.ch/$SCRAM_ARCH/cms/$cmsswrel/external/$SCRAM_ARCH/bin/thisroot.sh

  export PLOT_PATH=$PLOTTER_WORKING_DIR/output/

elif [[ $HOSTNAME == *"cms1"* ]] || [[ $HOSTNAME == *"cms2"* ]] ; then

  #### use cvmfs for root ####
  export CMS_PATH=/cvmfs/cms.cern.ch
  source $CMS_PATH/cmsset_default.sh
  export SCRAM_ARCH=slc6_amd64_gcc700
  export cmsswrel='cmssw-patch/CMSSW_10_4_0_patch1'
  #export cmsswrel='cmssw/CMSSW_10_5_0'
  cd /cvmfs/cms.cern.ch/$SCRAM_ARCH/cms/$cmsswrel/src
  echo "@@@@ SCRAM_ARCH = "$SCRAM_ARCH
  echo "@@@@ cmsswrel = "$cmsswrel
  echo "@@@@ scram..."
  eval `scramv1 runtime -sh`
  cd -
  source /cvmfs/cms.cern.ch/$SCRAM_ARCH/cms/$cmsswrel/external/$SCRAM_ARCH/bin/thisroot.sh

  export PLOT_PATH=$PLOTTER_WORKING_DIR/output/

fi

export FILE_PATH=$PLOTTER_WORKING_DIR/rootfiles/
export PYTHONPATH=$PYTHONPATH:$PLOTTER_WORKING_DIR/python/:$PLOTTER_WORKING_DIR/include/

export SCRIPT_DIR=$PLOTTER_WORKING_DIR/script/
export ROOT_INCLUDE_PATH=$ROOT_INCLUDE_PATH:$PLOTTER_WORKING_DIR/include/:$PLOTTER_WORKING_DIR/src/

mkdir -p $FILE_PATH/$CATANVERSION

CurrentGitBranch=`git branch | grep \* | cut -d ' ' -f2`
source bin/BashColorSets.sh
printf "@@@@ Current HNWR_Plotter branch : "${BGreen}$CurrentGitBranch${Color_Off}"\n"

# Instructions to setup the code on lxplus
```
git clone git@github.com:UMN-CMS/HNWR_Plotter.git
cd HNWR_Plotter
git checkout Run2Legacy_v4__MK
```
## env setup
```
source setup.sh
```
## copy input histograms
Get tarball from here: https://cernbox.cern.ch/s/VC3elTzvDKrBnrJ
```
cp Regions.tar.gz $FILE_PATH/$CATANVERSION/
cd $FILE_PATH/$CATANVERSION/
tar -xvf Region.tar.gz
cd ${PLOTTER_WORKING_DIR}
```
## now draw plots
```
source script/run_SR.sh
source script/run_CR.sh
```
In those scripts you can see how to run commands for python/Draw_SR.py or python/Draw_CR.py to draw specific regions/years.

## plot path
```
cd $PLOTTER_WORKING_DIR/output/Run2Legacy_v4__Default/SR/YearCombined/
cd $PLOTTER_WORKING_DIR/output/Run2Legacy_v4__Default/CR/YearCombined/
```

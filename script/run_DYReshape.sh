#for i in 2016 2017 2018
for i in 2018
do
  #root -l -b -q "src/Get_DYReshapeRatio.C($i) "
  root -l -b -q "src/Get_DYReshapeRatioJetPt.C($i)"
done

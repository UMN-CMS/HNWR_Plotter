OnlyLatex="false"

for i in 2016 2017 2018
do
  root -l -b -q "src/Get_EMuRatio.C($i,$OnlyLatex) "
  root -l -b -q "src/Get_EMuRatio.C(-$i,$OnlyLatex) "
done

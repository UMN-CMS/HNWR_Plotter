for Year in 2016 2017 2018
#for Year in 2018
do
  root -l -b -q "src/Make_ShapeForLimit.C(${Year})" &> tmp/log_Make_ShapeForLimit_${Year}.log &
  #root -l -b -q "src/Make_EMuShape.C(${Year},0)" &> tmp/log_Make_EMuShape_${Year}_0.log &
  #root -l -b -q "src/Make_EMuShape.C(${Year},1)" &> tmp/log_Make_EMuShape_${Year}_1.log &
  root -l -b -q "src/Make_DYShape.C(${Year})" &> tmp/log_Make_DYShape_${Year}.log &
done

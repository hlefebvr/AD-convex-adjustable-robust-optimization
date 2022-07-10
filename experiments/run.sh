EXECUTABLE=../cmake-build-debug/location_problem/flp_solve
ALGORITHM=1
DATA=../location_problem/data

mkdir output

for DEV in 1.2 1.4
do
  for FILE in $DATA/*
  do
    $EXECUTABLE $FILE DEV $ALGORITHM > output/${FILE}_d${DEV}.log
  done
done
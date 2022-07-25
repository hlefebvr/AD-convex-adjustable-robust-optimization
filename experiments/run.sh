EXECUTABLE=../cmake-build-debug/location_problem/flp_solve
ALGORITHM=1
DATA=../location_problem/data

mkdir output

for DEV in 0.05 0.10
do
  for FILE in $DATA/*
  do
    BASENAME=$(basename $FILE)
    $EXECUTABLE $FILE $DEV $ALGORITHM > output/${BASENAME}_d${DEV}.log
  done
done

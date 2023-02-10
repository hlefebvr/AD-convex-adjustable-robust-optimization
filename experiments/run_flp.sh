EXECUTABLE=../cmake-build-debug/location_problem/flp_solve
DATA=../location_problem/data

mkdir output

for FILE in $DATA/*
  do
    BASENAME=$(basename $FILE)
    $EXECUTABLE $FILE > output/${BASENAME}.log
done


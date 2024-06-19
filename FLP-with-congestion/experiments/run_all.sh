#!/bin/bash

if [ "$(whoami)" = "utr_lefebvre" ]
then
  PROJECT_DIRECTORY=/home/utr_lefebvre/AD-convex-adjustable-robust-optimization
else
  PROJECT_DIRECTORY=/home/henri/CLionProjects/AD-convex-adjustable-robust-optimization
fi

INSTANCE_DIRECTORY=FLP-with-congestion/data/basic
BUILD_DIRECTORY=cmake-build-debug
EXECUTABLE=FLP-with-congestion/solver/FLP_cong_solve
EXPERIMENTS_DIRECTORY=FLP-with-congestion/experiments

COUNTER=0

for FILE in $PROJECT_DIRECTORY/$INSTANCE_DIRECTORY/*
do
  for P in .10 .20 .30
  do

    for DEV in .25 .50
    do

      for METHOD in CCG GBD
      do

        for USE_HEURISTIC in 0 1
        do

          ARGS="$PROJECT_DIRECTORY/$EXPERIMENTS_DIRECTORY/run_one.sh $PROJECT_DIRECTORY/$BUILD_DIRECTORY/$EXECUTABLE $FILE $P $DEV $METHOD 7200 $USE_HEURISTIC"

          echo "Submitting $ARGS"

          if [ "$(whoami)" = "utr_lefebvre" ]
          then
            sbatch $ARGS
          else
            $ARGS
          fi

          COUNTER=$(($COUNTER+1))

        done

      done

    done

  done
done

echo "Submitted ${COUNTER}"

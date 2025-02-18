#!/bin/bash

if [ "$(whoami)" = "utr_lefebvre" ]
then
  PROJECT_DIRECTORY=/home/utr_lefebvre/AD-convex-adjustable-robust-optimization
else
  PROJECT_DIRECTORY=/home/henri/Research/AD-convex-adjustable-robust-optimization
fi

INSTANCE_DIRECTORY=RAP/data
BUILD_DIRECTORY=cmake-build-debug
EXECUTABLE=RAP/solver/RAP_solve
EXPERIMENTS_DIRECTORY=RAP/experiments

COUNTER=0

for FILE in $PROJECT_DIRECTORY/$INSTANCE_DIRECTORY/*
do

  for P in .10 .20 .30
  do

    for DEV in .25 .50
    do

      for METHOD in CCG
      do

        for USE_HEURISTIC in 0 1
        do

          for BUDEGTED_UNC in 0 1
          do

            ARGS="$PROJECT_DIRECTORY/$EXPERIMENTS_DIRECTORY/run_one.sh $PROJECT_DIRECTORY/$BUILD_DIRECTORY/$EXECUTABLE $FILE $P $DEV $METHOD 7200 $USE_HEURISTIC 1 $BUDEGTED_UNC"

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

done

echo "Submitted ${COUNTER}"

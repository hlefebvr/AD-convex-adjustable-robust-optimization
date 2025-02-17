#!/bin/bash
#SBATCH --nodes 1
#SBATCH --mem 32GB
#SBATCH --time 0-02:30:00
#SBATCH --mail-type FAIL
#SBATCH --ntasks 1 # 1 processor to be used
#SBATCH --partition=haswell-256

if [ "$(whoami)" = "utr_lefebvre" ]
then
  #module purge
  #module load gcc
  :
fi

echo "-- Running $@"

$@

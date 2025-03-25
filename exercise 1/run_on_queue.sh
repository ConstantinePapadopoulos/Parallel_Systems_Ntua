#!/bin/bash

## Give the Job a descriptive name
#PBS -N run_omp_GOL

## Output and error files
#PBS -o run_omp_GOL.out
#PBS -e run_omp_GOL.err

## How many machines should we get?
#PBS -l nodes=1:ppn=8

## How long should the job run for?
#PBS -l walltime=02:00:00

## Start
## Run make in the src folder (modify properly)

module load openmp
cd /home/parallel/parlab29/a1

for i in 1 2 4 6 8
do
  for j in 64 1024 4096
  do
    export OMP_NUM_THREADS=$i
    # Create separate output and error files for each combination of i and j
    ./Game_Of_Life $j 1000 > "run_omp_GOL_${i}_${j}.out" 2> "run_omp_GOL_${i}_${j}.err"
  done
done


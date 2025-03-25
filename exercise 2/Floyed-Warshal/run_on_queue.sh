#!/bin/bash

## Give the Job a descriptive name
#PBS -N run_fw

## Output and error files
#PBS -o run_fw_tiled_tasks_com.out
#PBS -e run_fw.err

## How many machines should we get? 
#PBS -l nodes=1:ppn=8

##How long should the job run for?
#PBS -l walltime=00:10:00

## Start 
## Run make in the src folder (modify properly)

module load openmp
cd /home/parallel/parlab29/a2/FW


for i in 1 2 4 8 16 32 64; do
    export OMP_NUM_THREADS="$i"
    export GOMP_CPU_AFFINITY="0-7 32-39"
    
    for b 64 128; do
        ./fw_tiled_tasks 4096 "$b"
    done
done

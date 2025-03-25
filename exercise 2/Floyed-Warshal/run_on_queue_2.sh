#!/bin/bash

#Give the Job a descriptive name
#PBS -N run_fw

## Output and error files
#PBS -o run_fw_tiled_tasks1.out
#PBS -e run_fw_tiled_tasks.err

## How many machines should we get?
#PBS -l nodes=1:ppn=8

##How long should the job run for?
#PBS -l walltime=00:07:00

## Start
## Run make in the src folder (modify properly)

module load openmp
cd /home/parallel/parlab29/a2/FW


for i in 32 64; do
    export OMP_NUM_THREADS="$i"
    export GOMP_CPU_AFFINITY="0-63"
	for j in 4096; do
	    for b in 64 128; do
        	numactl --interleave=all ./fw_tiled_tasks "$j" "$b"
    	    done
	done

done  

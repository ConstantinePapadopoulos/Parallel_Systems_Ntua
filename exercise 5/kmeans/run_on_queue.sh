#!/bin/bash
#PBS -N run_mpi

## Output and error files
#PBS -o run_mpi.out
#PBS -e run_mpi.err

## How many machines should we get? 
#PBS -l nodes=8:ppn=8

##How long should the job run for?
#PBS -l walltime=00:30:00

## Start 
## Run make in the src folder (modify properly)

module load openmpi/1.8.3
cd /home/parallel/parlab29/a4/kmeans


for i in  1 2 4 8 16 32 64 80
	do
   		mpirun --mca btl tcp,self -np ${i} ./kmeans_mpi -s 256 -c 16 -n 32 -l 10
	done

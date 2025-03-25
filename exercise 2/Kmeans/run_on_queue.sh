#!/bin/bash

## Give the Job a descriptive name
#PBS -N run_kmeans

## Output and error files
#PBS -o run_kmeans_reduction_final_areti.out
#PBS -e run_kmeans_reduction_final_areti.err



##How long should the job run for?
#PBS -l walltime=00:10:00
#PBS -l nodes=sandman:ppn=64

## Start 
## Run make in the src folder (modify properly)

module load openmp
cd /home/parallel/parlab29/a2/kmeans
SIZE=256
COORDS=16
CLUSTERS=32
LOOPS=10
for i in 1 2 4 8 16 32 64; do
	export OMP_NUM_THREADS="$i"
	export GOMP_CPU_AFFINITY="0-63"


	./kmeans_omp_reduction -s $SIZE -n $COORDS -c $CLUSTERS -l $LOOPS

done

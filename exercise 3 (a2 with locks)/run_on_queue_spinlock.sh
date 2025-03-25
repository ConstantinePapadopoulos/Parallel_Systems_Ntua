#!/bin/bash

## Give the Job a descriptive name
#PBS -N run_kmeans

## Output and error files
#PBS -o run_kmeans.out
#PBS -e run_kmeans.err

## How many machines should we get? 
#PBS -l nodes=1:ppn=8

##How long should the job run for?
#PBS -l walltime=02:00:00

## Start 
## Run make in the src folder (modify properly)

module load openmp
cd /home/parallel/parlab29/a2_locks

SIZE=32
COORDS=16
CLUSTERS=32
LOOPS=10
for i in 1 2 4 8 16 32 64; do
        export OMP_NUM_THREADS="$i"
        export GOMP_CPU_AFFINITY="0-63"

 #       ./kmeans_omp_naive -s $SIZE -n $COORDS -c $CLUSTERS -l $LOOPS > "outputs/naive/${i}.out"
 #       ./kmeans_omp_array_lock -s $SIZE -n $COORDS -c $CLUSTERS -l $LOOPS > "outputs/array/${i}.out"
 #       ./kmeans_omp_tas_lock -s $SIZE -n $COORDS -c $CLUSTERS -l $LOOPS > "outputs/tas/${i}.out"
 #       ./kmeans_omp_ttas_lock -s $SIZE -n $COORDS -c $CLUSTERS -l $LOOPS > "outputs/ttas/${i}.out"
 #       ./kmeans_omp_clh_lock -s $SIZE -n $COORDS -c $CLUSTERS -l $LOOPS > "outputs/clh/${i}.out"
 #       ./kmeans_omp_critical -s $SIZE -n $COORDS -c $CLUSTERS -l $LOOPS > "outputs/critical/${i}.out"
 #       ./kmeans_omp_pthread_mutex_lock -s $SIZE -n $COORDS -c $CLUSTERS -l $LOOPS > "outputs/mutex/${i}.out"
        ./kmeans_omp_pthread_spin_lock -s $SIZE -n $COORDS -c $CLUSTERS -l $LOOPS > "outputs/spinlock/${i}.out"
 #       ./kmeans_omp_nosync_lock -s $SIZE -n $COORDS -c $CLUSTERS -l $LOOPS > "outputs/nosync/${i}.out"



done


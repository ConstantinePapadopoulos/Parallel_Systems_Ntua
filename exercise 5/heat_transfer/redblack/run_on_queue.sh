#!/bin/bash
#PBS-N run_jacobi

## Output and error files
#PBS -o run_redblackend.out
#PBS -e run_redblack.err

## How many machines should we get?
#PBS -l nodes=8:ppn=8

## How long should the job run for?
#PBS -l walltime=00:50:00

## Start
## Run make in the src folder (modify as needed)

module load openmpi/1.8.3
cd /home/parallel/parlab29/a4/heat_transfer/redblack

# Run Jacobi with different process counts (1, 2, 4, 8, 16, 32, 64)
for i in 64
do
    mpirun --mca btl tcp,self -np ${i} ./RedBlack_mpi 2048 2048  8 8
    mpirun --mca btl tcp,self -np ${i} ./RedBlack_mpi 4096 4096  8 8
    mpirun --mca btl tcp,self -np ${i} ./RedBlack_mpi 6144 6144  8 8
done


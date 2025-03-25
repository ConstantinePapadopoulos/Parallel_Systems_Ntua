#!/bin/bash
#PBS-N run_jacobi

## Output and error files
#PBS -o run_redblack.out
#PBS -e run_redblack.err

## How many machines should we get?
#PBS -l nodes=8:ppn=8

## How long should the job run for?
#PBS -l walltime=00:50:00

## Start
## Run make in the src folder (modify as needed)

module load openmpi/1.8.3
cd /home/parallel/parlab29/a4/heat_transfer/redblack_conv

mpirun --mca btl tcp,self -np 64  ./RedBlack_mpi 512 512 8 8



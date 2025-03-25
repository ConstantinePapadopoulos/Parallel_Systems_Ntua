#!/bin/bash 
#PBS -N make_jacobi
#PBS -o make_redblack.out
#PBS -e make_redblack.err
#PBS -l nodes=1:ppn=1
#PBS -l walltime=00:10:00

module load openmpi/1.8.3
cd /home/parallel/parlab29/a4/heat_transfer/redblack
make


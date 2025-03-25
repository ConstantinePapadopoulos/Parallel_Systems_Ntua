#!/bin/bash
#PBS-N run_jacobi

## Output and error files
#PBS -o run_jacobi.out
#PBS -e run_jacobi.err

## How many machines should we get?
#PBS -l nodes=8:ppn=8

## How long should the job run for?
#PBS -l walltime=00:50:00

## Start
## Run make in the src folder (modify as needed)

module load openmpi/1.8.3
cd /home/parallel/parlab29/a4/heat_transfer/mpi

# Run Jacobi with different process counts (1, 2, 4, 8, 16, 32, 64)
for i in 32
do
    mpirun --mca btl tcp,self -np ${i} ./jacobi_mpi 2048 2048  8 4
    mpirun --mca btl tcp,self -np ${i} ./jacobi_mpi 4096 4096  8 4
    mpirun --mca btl tcp,self -np ${i} ./jacobi_mpi 6144 6144  8 4
done
for i in 16
do
    mpirun --mca btl tcp,self -np ${i} ./jacobi_mpi 2048 2048  4 4
    mpirun --mca btl tcp,self -np ${i} ./jacobi_mpi 4096 4096  4 4
    mpirun --mca btl tcp,self -np ${i} ./jacobi_mpi 6144 6144  4 4
done
for i in 8
do
    mpirun --mca btl tcp,self -np ${i} ./jacobi_mpi 2048 2048  2 4
    mpirun --mca btl tcp,self -np ${i} ./jacobi_mpi 4096 4096  2 4
    mpirun --mca btl tcp,self -np ${i} ./jacobi_mpi 6144 6144  2 4
done
for i in 4
do
    mpirun --mca btl tcp,self -np ${i} ./jacobi_mpi 2048 2048  2 2
    mpirun --mca btl tcp,self -np ${i} ./jacobi_mpi 4096 4096  2 2
    mpirun --mca btl tcp,self -np ${i} ./jacobi_mpi 6144 6144  2 2
done
for i in 2
do
    mpirun --mca btl tcp,self -np ${i} ./jacobi_mpi 2048 2048  1 2
    mpirun --mca btl tcp,self -np ${i} ./jacobi_mpi 4096 4096  1 2
    mpirun --mca btl tcp,self -np ${i} ./jacobi_mpi 6144 6144  1 2
done
for i in 1
do
    mpirun --mca btl tcp,self -np ${i} ./jacobi_mpi 2048 2048  1 1
    mpirun --mca btl tcp,self -np ${i} ./jacobi_mpi 4096 4096  1 1
    mpirun --mca btl tcp,self -np ${i} ./jacobi_mpi 6144 6144  1 1
done


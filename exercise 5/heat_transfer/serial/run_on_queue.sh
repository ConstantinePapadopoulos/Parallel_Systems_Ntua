#!/bin/bash

## Give the Job a descriptive name
#PBS -N runjob

## Output and error files
#PBS -o outrunjob
#PBS -e errorrunjob

## How many machines should we get?
#PBS -l nodes=1:ppn=1

#PBS -l walltime=00:20:00

## Start 
## Run make in the src folder (modify properly)
cd /home/parallel/parlab29/a4/heat_transfer/serial
#./jacobi 512
#./seidelsor 256
./redblacksor 512

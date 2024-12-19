#!/bin/bash
#SBATCH --nodes=4
#SBATCH --ntasks-per-node=4
#SBATCH --output=mpi-out.%j
#SBATCH --error=mpi-err.%j
#SBATCH --time=00:00:00
#SBATCH --partition=cronos
cd $SLURM_SUBMIT_DIR
mpirun ejercicio1
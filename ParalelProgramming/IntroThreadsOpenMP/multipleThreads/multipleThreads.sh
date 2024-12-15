#!/bin/bash
#SBATCH --nodes=1
#SBATCH --output=omp-out.%j
#SBATCH --error=omp-err.%j
#SBATCH --partition=cronos
export OMP_NUM_THREADS=4
./multipleThreads
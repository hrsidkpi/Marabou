#!/bin/bash

mkdir -p build
cd build

echo "building..."
cmake --build . -j 4

cd ../run_batches
ls
echo "running all..."
# sbatch --mem=400m -c4 --time=10:0:0 n_1_1_p_1.sh
# sbatch --mem=400m -c4 --time=10:0:0 n_1_1_p_2.sh
# sbatch --mem=400m -c4 --time=10:0:0 n_1_1_p_3.sh
# sbatch --mem=400m -c4 --time=10:0:0 n_1_1_p_4.sh
# sbatch --mem=400m -c4 --time=10:0:0 n_1_1_p_sat.sh
# 
# sbatch --mem=400m -c4 --time=10:0:0 n_1_2_p_1.sh
# sbatch --mem=400m -c4 --time=10:0:0 n_1_2_p_2.sh
# sbatch --mem=400m -c4 --time=10:0:0 n_1_2_p_3.sh
# sbatch --mem=400m -c4 --time=10:0:0 n_1_2_p_4.sh
# sbatch --mem=400m -c4 --time=10:0:0 n_1_2_p_sat.sh
# 
# sbatch --mem=400m -c4 --time=10:0:0 n_1_3_p_1.sh
# sbatch --mem=400m -c4 --time=10:0:0 n_1_3_p_2.sh
# sbatch --mem=400m -c4 --time=10:0:0 n_1_3_p_3.sh
# sbatch --mem=400m -c4 --time=10:0:0 n_1_3_p_4.sh
# sbatch --mem=400m -c4 --time=10:0:0 n_1_3_p_sat.sh

sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 1 1 1
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 1 2 1
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 1 3 1
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 1 4 1
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 1 5 1
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 1 6 1
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 1 7 1
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 1 8 1
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 1 9 1
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 2 1 1
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 2 2 1
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 2 3 1
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 2 4 1
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 2 5 1
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 2 6 1
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 2 7 1
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 2 8 1
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 2 9 1
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 3 1 1
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 3 2 1
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 3 3 1
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 3 4 1
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 3 5 1
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 3 6 1
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 3 7 1
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 3 8 1
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 3 9 1
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 4 1 1
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 4 2 1
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 4 3 1
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 4 4 1
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 4 5 1
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 4 6 1
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 4 7 1
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 4 8 1
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 4 9 1
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 5 1 1
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 5 2 1
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 5 3 1
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 5 4 1
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 5 5 1
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 5 6 1
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 5 7 1
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 5 8 1
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 5 9 1

sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 1 1 2
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 1 2 2
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 1 3 2
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 1 4 2
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 1 5 2
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 1 6 2
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 1 7 2
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 1 8 2
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 1 9 2
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 2 1 2
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 2 2 2
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 2 3 2
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 2 4 2
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 2 5 2
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 2 6 2
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 2 7 2
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 2 8 2
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 2 9 2
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 3 1 2
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 3 2 2
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 3 3 2
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 3 4 2
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 3 5 2
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 3 6 2
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 3 7 2
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 3 8 2
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 3 9 2
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 4 1 2
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 4 2 2
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 4 3 2
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 4 4 2
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 4 5 2
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 4 6 2
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 4 7 2
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 4 8 2
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 4 9 2
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 5 1 2
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 5 2 2
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 5 3 2
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 5 4 2
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 5 5 2
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 5 6 2
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 5 7 2
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 5 8 2
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 5 9 2

sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 1 1 3
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 1 2 3
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 1 3 3
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 1 4 3
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 1 5 3
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 1 6 3
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 1 7 3
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 1 8 3
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 1 9 3
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 2 1 3
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 2 2 3
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 2 3 3
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 2 4 3
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 2 5 3
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 2 6 3
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 2 7 3
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 2 8 3
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 2 9 3
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 3 1 3
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 3 2 3
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 3 3 3
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 3 4 3
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 3 5 3
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 3 6 3
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 3 7 3
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 3 8 3
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 3 9 3
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 4 1 3
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 4 2 3
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 4 3 3
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 4 4 3
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 4 5 3
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 4 6 3
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 4 7 3
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 4 8 3
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 4 9 3
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 5 1 3
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 5 2 3
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 5 3 3
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 5 4 3
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 5 5 3
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 5 6 3
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 5 7 3
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 5 8 3
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 5 9 3

sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 1 1 4
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 1 2 4
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 1 3 4
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 1 4 4
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 1 5 4
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 1 6 4
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 1 7 4
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 1 8 4
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 1 9 4
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 2 1 4
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 2 2 4
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 2 3 4
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 2 4 4
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 2 5 4
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 2 6 4
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 2 7 4
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 2 8 4
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 2 9 4
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 3 1 4
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 3 2 4
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 3 3 4
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 3 4 4
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 3 5 4
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 3 6 4
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 3 7 4
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 3 8 4
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 3 9 4
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 4 1 4
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 4 2 4
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 4 3 4
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 4 4 4
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 4 5 4
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 4 6 4
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 4 7 4
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 4 8 4
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 4 9 4
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 5 1 4
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 5 2 4
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 5 3 4
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 5 4 4
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 5 5 4
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 5 6 4
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 5 7 4
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 5 8 4
sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 5 9 4
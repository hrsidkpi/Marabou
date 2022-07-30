#!/bin/bash

mkdir -p build256
cd build256
cmake .. -DFETCHCONTENT_QUIET=OFF 

echo "building..."
cmake --build . -j 4

cd ../run_batches

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

sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 2 2 2

sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 3 3 3

sbatch --mem=2g -c4 --time=10:0:0 n_general.sh 4 4 4

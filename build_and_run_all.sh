#!/bin/bash

mkdir -p build
cd build

echo "building..."
cmake --build . -j 4

cd ../run_batches
ls
echo "running all..."
sbatch --mem=400m -c4 --time=6:0:0 n_1_1_p_1.sh
sbatch --mem=400m -c4 --time=6:0:0 n_1_1_p_2.sh
sbatch --mem=400m -c4 --time=6:0:0 n_1_1_p_3.sh
sbatch --mem=400m -c4 --time=6:0:0 n_1_1_p_4.sh
sbatch --mem=400m -c4 --time=6:0:0 n_1_1_p_sat.sh

sbatch --mem=400m -c4 --time=6:0:0 n_1_2_p_1.sh
sbatch --mem=400m -c4 --time=6:0:0 n_1_2_p_2.sh
sbatch --mem=400m -c4 --time=6:0:0 n_1_2_p_3.sh
sbatch --mem=400m -c4 --time=6:0:0 n_1_2_p_4.sh
sbatch --mem=400m -c4 --time=6:0:0 n_1_2_p_sat.sh

sbatch --mem=400m -c4 --time=6:0:0 n_1_3_p_1.sh
sbatch --mem=400m -c4 --time=6:0:0 n_1_3_p_2.sh
sbatch --mem=400m -c4 --time=6:0:0 n_1_3_p_3.sh
sbatch --mem=400m -c4 --time=6:0:0 n_1_3_p_4.sh
sbatch --mem=400m -c4 --time=6:0:0 n_1_3_p_sat.sh

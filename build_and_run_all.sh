mkdir -p build
cd build

echo "building..."
cmake --build . -j 4

cd ..
echo "running all..."
sbatch --mem=400m -c4 --time=6:0:0 sbatch run_batches/n_1_1_p_1.sh
sbatch --mem=400m -c4 --time=6:0:0 sbatch run_batches/n_1_1_p_2.sh
sbatch --mem=400m -c4 --time=6:0:0 sbatch run_batches/n_1_1_p_3.sh
sbatch --mem=400m -c4 --time=6:0:0 sbatch run_batches/n_1_1_p_4.sh

sbatch --mem=400m -c4 --time=6:0:0 sbatch run_batches/n_1_2_p_1.sh
sbatch --mem=400m -c4 --time=6:0:0 sbatch run_batches/n_1_2_p_2.sh
sbatch --mem=400m -c4 --time=6:0:0 sbatch run_batches/n_1_2_p_3.sh
sbatch --mem=400m -c4 --time=6:0:0 sbatch run_batches/n_1_2_p_4.sh

sbatch --mem=400m -c4 --time=6:0:0 sbatch run_batches/n_1_3_p_1.sh
sbatch --mem=400m -c4 --time=6:0:0 sbatch run_batches/n_1_3_p_2.sh
sbatch --mem=400m -c4 --time=6:0:0 sbatch run_batches/n_1_3_p_3.sh
sbatch --mem=400m -c4 --time=6:0:0 sbatch run_batches/n_1_3_p_4.sh
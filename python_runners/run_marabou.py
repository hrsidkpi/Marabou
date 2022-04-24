import os


def run_marabou_ai(net_name_suffix: str, query_name_suffix: str, ai_interval: int, out_file: str, output_layer_index: int):
    net_name = f"../resources/nnet/acasxu/ACASXU_experimental_v2a_{net_name_suffix}.nnet"
    property_name = f"../resources/properties/acas_property_{query_name_suffix}.txt"

    os.chdir('../')
    os.system('ls')
    os.system('mkdir -p build')
    os.chdir('build')
    os.system('module load gurobi')
    os.system('echo "building..."')
    os.system('cmake --build . -j 4')
    os.system(f'./Marabou {net_name} {property_name} --bounds-o-file {out_file} -output-layer-index {output_layer_index}')

    

def run_marabou_no_ai(net_name_suffix: str, query_name_suffix: str):
    os.chdir("..")
    os.system("./")



def run_basic_sat_test():
    run_marabou_ai("1_1", "always_sat", 0, "out_1_1_always_sat", 12)
    print("done.")


run_basic_sat_test()
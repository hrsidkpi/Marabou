#!/bin/bash
cd ../build

net_name_prefix="../resources/nnet/acasxu/ACASXU_experimental_v2a_"
net_name_suffix=".nnet"
net_name="${net_name_prefix}${1}_${2}${net_name_suffix}"

echo "network: "
echo $net_name

prop_name_prefix="../resources/properties/acas_property_"
prop_name_suffix=".txt"
prop_name=$prop_name_prefix$3$prop_name_suffix

mkdir -p "res_zono_32_30_07_2022/"
summary_path_prefix="res_zono_32_30_07_2022/res_"
summary_path_suffix=".txt"
summary_path="${summary_path_prefix}${1}_${2}_${3}${summary_path_suffix}"

touch $summary_path
echo "not finished yet" > $summary_path

echo "property: "
echo $prop_name

echo "summary path: "
echo $summary_path

./Marabou $net_name $prop_name --summary-file=$summary_path --tightening-strategy zonotope
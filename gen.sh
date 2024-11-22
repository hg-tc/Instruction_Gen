PE_num=$1
data='MH'

./inst_gen 1 Data/$data/mono_output_pos_.txt Data/$data/mono_output_num_.txt mono $PE_num
./inst_gen 2 Data/$data/fusion_output_pos_.txt Data/$data/fusion_output_num_.txt fusion $PE_num
./inst_gen 3 Data/$data/global_BA_1_output_pos_.txt Data/$data/global_BA_1_output_num_.txt sfm2 $PE_num
./inst_gen 4 Data/$data/global_BA_3_output_pos_.txt Data/$data/global_BA_3_output_num_.txt sfm3 $PE_num
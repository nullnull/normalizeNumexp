#!/bin/sh
python create_dic_number_modifier.py num_settouji.txt > ../num_prefix_json.txt
python create_dic_number_modifier.py num_setubiji.txt > ../num_suffix_json.txt
python create_dic_num.py > ../num_counter_json.txt
python create_dic_num_prefix_counter.py > ../num_prefix_counter_json.txt
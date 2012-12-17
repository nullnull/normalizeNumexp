#!/bin/sh
python create_dic_number_modifier.py num_settouji.txt > ../num_prefix_json.txt
python create_dic_number_modifier.py num_setubiji.txt > ../num_suffix_json.txt
python create_dic_number_modifier.py abstime_settouji.txt > ../abstime_prefix_json.txt
python create_dic_number_modifier.py abstime_setubiji.txt > ../abstime_suffix_json.txt
python create_dic_number_modifier.py reltime_settouji.txt > ../reltime_prefix_json.txt
python create_dic_number_modifier.py abstime_setubiji.txt > ../reltime_suffix_json.txt #reltime自体の接尾辞は存在しない。相対絶対表現でabsの接尾辞を使う
python create_dic_number_modifier.py duration_setouji.txt > ../duration_prefix_json.txt
python create_dic_number_modifier.py duration_setubiji.txt > ../duration_suffix_json.txt
python create_dic_num.py > ../num_counter_json.txt
python create_dic_num_prefix_counter.py > ../num_prefix_counter_json.txt
python create_dic_abstime.py > ../abstime_expression_json.txt
python create_dic_abstime_prefix_counter.py > ../abstime_prefix_counter_json.txt
python create_dic_reltime.py > ../reltime_expression_json.txt
python create_dic_reltime_prefix_counter.py > ../reltime_prefix_counter_json.txt
python create_dic_duration.py > ../duration_expression_json.txt
python create_dic_inappropriate.py > ../inappropriate_strings_json.txt
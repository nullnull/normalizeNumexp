# -*- coding: utf-8 -*-

fin = open("reltime_prefix_counter.txt", "r")
for line in fin.readlines() :
  lst = line.rstrip().split()
  print "{\"pattern\":\""+lst[0]+"\", \"corresponding_time_position\":[\""+lst[1]+"\"], \"process_type\":[\""+lst[2]+"\"], \"ordinary\":false, \"option\":\"add_relation\"}"

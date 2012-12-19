# -*- coding: utf-8 -*-


fin = open("abstime_nengou.txt", "r")
for line in fin.readlines() :
  lst = line.rstrip().split()
  print "{\"pattern\":\""+lst[0]+"\", \"corresponding_time_position\":[], \"process_type\":[\""+lst[1]+"\"], \"ordinary\":false, \"option\":\"seireki\"}"

fin = open("abstime_prefix_counter.txt", "r")
for line in fin.readlines() :
  lst = line.rstrip().split()
  print "{\"pattern\":\""+lst[0]+"\", \"corresponding_time_position\":[], \"process_type\":[], \"ordinary\":false, \"option\":\""+lst[1]+"\"}"
# -*- coding: utf-8 -*-

fin = open("num_prefix_counter.txt", "r")
for line in fin.readlines() :
  l = line.rstrip().split()
  print "{\"pattern\":\""+l[0]+"\", \"counter\":\""+l[1]+"\", \"SI_prefix\":0, \"optional_power_of_ten\":0, \"ordinary\":false, \"option\":\""+l[2]+"\"}"

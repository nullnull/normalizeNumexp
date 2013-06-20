# -*- coding: utf-8 -*-
import sys

filename = sys.argv[1]
fin = open(filename, "r")
for line in fin.readlines() :
  line = line.replace("<=", "or_over").replace(">=", "or_less").replace("<","over").replace(">", "less")
  l = line.rstrip().split("\t")
  print "{\"pattern\":\""+l[0]+" \", \"process_type\":\""+l[1]+"\"}"



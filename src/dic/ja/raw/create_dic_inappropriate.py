# -*- coding: utf-8 -*-

fin = open("inappropriate_strings.txt", "r")
for line in fin.readlines() :
  l = line.rstrip()
  print "{\"str\":\""+l+"\"}"

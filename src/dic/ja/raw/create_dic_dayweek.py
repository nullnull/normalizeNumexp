# -*- coding: utf-8 -*-
fin = open("abstime_dayweek.txt", "r")
for line in fin.readlines() :
	dayweek = line.rstrip()
	fin2 = open("abstime_dayweek_pattern.txt", "r")
	for pattern in fin2.readlines() :
		pattern = pattern.rstrip("\n")
		print "{\"pattern\":\""+pattern.replace("*",dayweek)+"}"

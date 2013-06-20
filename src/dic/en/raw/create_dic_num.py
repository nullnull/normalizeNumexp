# -*- coding: utf-8 -*-

fin = open("num.txt", "r")
for line in fin.readlines() :
  l = line.rstrip().split()
  print "{\"pattern\":\" "+l[0]+" \", \"counter\":\""+l[1]+"\", \"SI_prefix\":0, \"optional_power_of_ten\":0, \"ordinary\":false, \"option\":\"\"}"
  print "{\"pattern\":\" "+l[0]+",\", \"counter\":\""+l[1]+"\", \"SI_prefix\":0, \"optional_power_of_ten\":0, \"ordinary\":false, \"option\":\"\"}"
  print "{\"pattern\":\" "+l[0]+".\", \"counter\":\""+l[1]+"\", \"SI_prefix\":0, \"optional_power_of_ten\":0, \"ordinary\":false, \"option\":\"\"}"
  print "{\"pattern\":\" "+l[0]+"?\", \"counter\":\""+l[1]+"\", \"SI_prefix\":0, \"optional_power_of_ten\":0, \"ordinary\":false, \"option\":\"\"}"
  print "{\"pattern\":\" "+l[0]+"!\", \"counter\":\""+l[1]+"\", \"SI_prefix\":0, \"optional_power_of_ten\":0, \"ordinary\":false, \"option\":\"\"}"        

fin = open("num_SI_prefix.txt")
prefix = [["",0]]
for line in fin.readlines() :
  l = line.rstrip().split()
  prefix.append(l)
fin = open("num_SI.txt", "r")
for line in fin.readlines() :
  l = line.rstrip().split()
  for p in prefix :
	print "{\"pattern\":\" "+p[0]+l[0]+" \", \"counter\":\""+l[1]+"\", \"SI_prefix\":"+str(p[1])+", \"optional_power_of_ten\":0, \"ordinary\":false, \"option\":\"\"}"
	print "{\"pattern\":\" "+p[0]+l[0]+",\", \"counter\":\""+l[1]+"\", \"SI_prefix\":"+str(p[1])+", \"optional_power_of_ten\":0, \"ordinary\":false, \"option\":\"\"}"
	print "{\"pattern\":\" "+p[0]+l[0]+".\", \"counter\":\""+l[1]+"\", \"SI_prefix\":"+str(p[1])+", \"optional_power_of_ten\":0, \"ordinary\":false, \"option\":\"\"}"
	print "{\"pattern\":\" "+p[0]+l[0]+"!\", \"counter\":\""+l[1]+"\", \"SI_prefix\":"+str(p[1])+", \"optional_power_of_ten\":0, \"ordinary\":false, \"option\":\"\"}"
	print "{\"pattern\":\" "+p[0]+l[0]+"?\", \"counter\":\""+l[1]+"\", \"SI_prefix\":"+str(p[1])+", \"optional_power_of_ten\":0, \"ordinary\":false, \"option\":\"\"}"		

fin = open("num_SI_prefix_strings.txt")
prefix = [["",0]]
for line in fin.readlines() :
	l = line.rstrip().split()
	prefix.append(l)
fin = open("num_SI_strings.txt", "r")
for line in fin.readlines() :
	l = line.rstrip().split()
	for p in prefix :
		print "{\"pattern\":\" "+p[0]+l[0]+" \", \"counter\":\""+l[1]+"\", \"SI_prefix\":"+str(p[1])+", \"optional_power_of_ten\":0, \"ordinary\":false, \"option\":\"\"}"
		print "{\"pattern\":\" "+p[0]+l[0]+",\", \"counter\":\""+l[1]+"\", \"SI_prefix\":"+str(p[1])+", \"optional_power_of_ten\":0, \"ordinary\":false, \"option\":\"\"}"
		print "{\"pattern\":\" "+p[0]+l[0]+".\", \"counter\":\""+l[1]+"\", \"SI_prefix\":"+str(p[1])+", \"optional_power_of_ten\":0, \"ordinary\":false, \"option\":\"\"}"
		print "{\"pattern\":\" "+p[0]+l[0]+"!\", \"counter\":\""+l[1]+"\", \"SI_prefix\":"+str(p[1])+", \"optional_power_of_ten\":0, \"ordinary\":false, \"option\":\"\"}"
		print "{\"pattern\":\" "+p[0]+l[0]+"?\", \"counter\":\""+l[1]+"\", \"SI_prefix\":"+str(p[1])+", \"optional_power_of_ten\":0, \"ordinary\":false, \"option\":\"\"}"		



fin = open("num_expand.txt")
for line in fin.readlines() :
	l = line.rstrip().split()
	print "{\"pattern\":\" "+l[0]+" \", \"counter\":\""+l[1]+"\", \"SI_prefix\":0, \"optional_power_of_ten\":"+l[2]+", \"ordinary\":false, \"option\":\"\"}"
	print "{\"pattern\":\" "+l[0]+",\", \"counter\":\""+l[1]+"\", \"SI_prefix\":0, \"optional_power_of_ten\":"+l[2]+", \"ordinary\":false, \"option\":\"\"}"
	print "{\"pattern\":\" "+l[0]+".\", \"counter\":\""+l[1]+"\", \"SI_prefix\":0, \"optional_power_of_ten\":"+l[2]+", \"ordinary\":false, \"option\":\"\"}"
	print "{\"pattern\":\" "+l[0]+"?\", \"counter\":\""+l[1]+"\", \"SI_prefix\":0, \"optional_power_of_ten\":"+l[2]+", \"ordinary\":false, \"option\":\"\"}"
	print "{\"pattern\":\" "+l[0]+"!\", \"counter\":\""+l[1]+"\", \"SI_prefix\":0, \"optional_power_of_ten\":"+l[2]+", \"ordinary\":false, \"option\":\"\"}"
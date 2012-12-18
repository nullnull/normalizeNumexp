# -*- coding: utf-8 -*-


def create_list_expression(lst) :
  if len(lst)==0 : return "[]"
  ret = "["
  for l in lst :
    ret += "\""+l+"\""
    ret += ", "
  ret = ret[:-2] + "]"
  return ret
	
def create_process_type(str) :
	process_type = []
	if(str.endswith("半")) : process_type.append("han")
	return process_type
	

#create date expression
fin = open("abstime_date.txt")
lst = []
for line in fin.readlines() :
  l = line.rstrip()
  l = l.replace("*","ǂ")
  lst.append(l)
  if len(lst) == 2 :
		corresponding_time_position = create_list_expression( lst[1].split() )
		print "{\"pattern\":\""+lst[0]+"\", \"corresponding_time_position\":"+corresponding_time_position+", \"process_type\":[], \"ordinary\":false, \"option\":\"\"}"
		lst = []
fin.close()


#create time expression
fin = open("abstime_time.txt")
lst = []
for line in fin.readlines() :
  l = line.rstrip()
  l = l.replace("*","ǂ")
  lst.append(l)
  if len(lst) == 2 :
		corresponding_time_position = create_list_expression( lst[1].split() )
		process_type = create_process_type(lst[0])
		print "{\"pattern\":\""+lst[0]+"\", \"corresponding_time_position\":"+corresponding_time_position+", \"process_type\":"+create_list_expression(process_type)+", \"ordinary\":false, \"option\":\"\"}"
		lst = []
fin.close()

"""
#普通にやろうとすると18MBほどになる。検索時間はあまり変わらないが、最初の読み込み時に5秒程時間がかかる。どうする？

#create date+time expression
fin = open("abstime_date.txt")
lst = []
for line in fin.readlines() :
  l = line.rstrip()
  l = l.replace("*","ǂ")
  lst.append(l)
  if len(lst) == 2 :
		if lst[1].find("d") != -1 :
			for dayweek in lst_dayweek :
				fin2 = open("abstime_time.txt")
				lst2 = []
				for line2 in fin2.readlines() :
					l2 = l2.replace("*","ǂ")
					lst2.append(l2)
					if len(lst2) == 2 :
						if lst2[1].find("h") == -1 : continue
						tmp = lst[1].split()
						tmp2 = lst2[1].split()
						tmp.extend(tmp2)
						corresponding_time_position = create_list_expression(tmp)
						process_type = create_process_type(lst[0]+dayweek+lst2[0])
						print "{\"pattern\":\""+lst[0]+dayweek+"ǂ"+lst2[0]+"\", \"corresponding_time_position\":"+corresponding_time_position+", \"process_type\":"+create_list_expression(process_type)+", \"ordinary\":false, \"option\":\"\"}"
						lst2 = []
		lst = []
fin.close()
"""
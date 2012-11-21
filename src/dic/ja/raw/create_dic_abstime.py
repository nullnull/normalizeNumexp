# -*- coding: utf-8 -*-


def create_list_expression(lst) :
  if len(lst)==0 : return "[]"
  ret = "["
  for l in lst :
    ret += "\""+l+"\""
    ret += ", "
  ret = ret[:-2] + "]"
  return ret

fin = open("abstime.txt")
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


fin = open("abstime_expand.txt")
lst = []
for line in fin.readlines() :
  l = line.rstrip()
  l = l.replace("*","ǂ")
  lst.append(l)
  if len(lst) == 4 :
    corresponding_time_position = create_list_expression( lst[2].split() )
    process_type = []
    if("h + 12" in lst[3]) : process_type.append("gogo")
    if(lst[0]=="世紀") : process_type.append("seiki")
    if(lst[0].endswith("半")) : process_type.append("han")
    print "{\"pattern\":\""+lst[0]+"\", \"corresponding_time_position\":"+corresponding_time_position+", \"process_type\":"+create_list_expression(process_type)+", \"ordinary\":false, \"option\":\"\"}"
    lst = []
fin.close()

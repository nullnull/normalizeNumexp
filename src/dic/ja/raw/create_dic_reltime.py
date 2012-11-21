# -*- coding: utf-8 -*-


def create_list_expression(fugou,lst_org) :
  lst = lst_org.split(",")
  if len(lst)==0 : return "[]"
  ret = "["
  for l in lst :
    ret += "\""+fugou+l+"\""
    ret += ", "
  ret = ret[:-2] + "]"
  return ret

fin = open("reltime_time_position.txt", "r")
for line in fin.readlines() :
  lst = line.rstrip().split()
  lst[0] = lst[0].replace("*","ǂ")
  fin2 = open("reltime_time_option.txt", "r")
  for line2 in fin2.readlines() :
    lst2 = line2.rstrip().split()
    if len(lst2) == 3 :
      print "{\"pattern\":\""+lst[0]+lst2[0]+"\", \"corresponding_time_position\":"+create_list_expression(lst2[1],lst[1])+", \"process_type\":[\""+lst2[2]+"\"], \"ordinary\":false, \"option\":\"\"}"
    else :
      print "{\"pattern\":\""+lst[0]+lst2[0]+"\", \"corresponding_time_position\":"+create_list_expression(lst2[1],lst[1])+", \"process_type\":[], \"ordinary\":false, \"option\":\"\"}"
#hanのため
    if len(lst2) == 3 :
      print "{\"pattern\":\""+lst[0]+"半"+lst2[0]+"\", \"corresponding_time_position\":"+create_list_expression(lst2[1],lst[1])+", \"process_type\":[\""+lst2[2]+"\", \"han\"], \"ordinary\":false, \"option\":\"\"}"
    else :
      print "{\"pattern\":\""+lst[0]+"半"+lst2[0]+"\", \"corresponding_time_position\":"+create_list_expression(lst2[1],lst[1])+", \"process_type\":[\"han\"], \"ordinary\":false, \"option\":\"\"}"

fin.close()

fin = open("reltime_specific.txt","r")
for line in fin.readlines() :
  print line.rstrip()
fin.close()

fin = open("../abstime_expression_json.txt", "r")
for line in fin.readlines() :
  print line.rstrip()
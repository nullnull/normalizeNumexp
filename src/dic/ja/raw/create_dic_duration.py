# -*- coding: utf-8 -*-

def create_list_expression(lst_org) :
  lst = lst_org.split(",")
  if len(lst)==0 : return "[]"
  ret = "["
  for l in lst :
    ret += "\""+l+"\""
    ret += ", "
  ret = ret[:-2] + "]"
  return ret

fin = open("duration_time_position.txt", "r")
for line in fin.readlines() :
  lst = line.rstrip().split()
  lst[0] = lst[0].replace("*","ǂ")
  print "{\"pattern\":\""+lst[0]+"\", \"corresponding_time_position\":"+create_list_expression(lst[1])+", \"process_type\":[], \"ordinary\":false, \"option\":\"\"}"
  print "{\"pattern\":\""+lst[0]+"半\", \"corresponding_time_position\":"+create_list_expression(lst[1])+", \"process_type\":[\"han\"], \"ordinary\":false, \"option\":\"\"}"
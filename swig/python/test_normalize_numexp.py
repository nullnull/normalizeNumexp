#coding:utf-8
import sys
from normalize_numexp import *

n = NormalizeNumexp("ja")
text = "魔女狩りは15世紀〜18世紀にかけてみられ、全ヨーロッパで4万人が処刑された"
result = StringVector(0)

n.normalize(text, result)
print "text:",text
for r in result :
    print r

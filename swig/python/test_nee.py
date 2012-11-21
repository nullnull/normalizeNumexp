#coding:utf-8
#from nee import *
from numerical_expression_extractor import *

n = NumericalExpressionExtractor("ja")
text = "私は三人の回になりたい"
result = StringVector(0)
n.extract_numerical_expression(text, "numerical", result)
for r in result :
    print r

#!/bin/sh
#http://www.swig.org/Doc2.0/SWIGDocumentation.html

#1. create _wrap.cxx and extractor.py by swig.
swig -c++ -python -I../../src/  -o numerical_expression_extractor_wrap.cxx ../numerical_expression_extractor.i

#2. compile _wrap.cxx (Python.h is necessary.)
gcc -O2 -fPIC -c numerical_expression_extractor_wrap.cxx -I/home/katsuma/local/python/include/python2.7 -I/home/katsuma/usr/local/include -I../../src/

#3. create _extractor.so
gcc -shared ../../build/src/numerical_expression_extractor.cpp.1.o ../../build/src/abstime_expression_normalizer.cpp.1.o ../../build/src/digit_utility.cpp.1.o ../../build/src/duration_expression_normalizer.cpp.1.o ../../build/src/normalizer_utility.cpp.1.o ../../build/src/number_normalizer.cpp.1.o ../../build/src/numerical_expression_normalizer.cpp.1.o ../../build/src/reltime_expression_normalizer.cpp.1.o numerical_expression_extractor_wrap.o -o _numerical_expression_extractor.so -I/home/katsuma/local/python/include/python2.7 -L/home/katsuma/usr/local/lib -lpficommon -lpficommon_visualization -lpficommon_text -lpficommon_network_base -lpficommon_concurrent -lpficommon_data -lpficommon_math -lpficommon_system -lpficommon_network_http -lpficommon_lang -lpficommon_network_rpc -lpficommon_network_cgi -lux
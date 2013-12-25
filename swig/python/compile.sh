#!/bin/sh
#http://www.swig.org/Doc2.0/SWIGDocumentation.html

#1. create _wrap.cxx and wrapper .py by swig.
swig -c++ -python -I../../src/  -o normalize_numexp_wrap.cxx ../normalize_numexp.i

#2. compile _wrap.cxx (Python.h is necessary.)
gcc -O2 -fPIC -c normalize_numexp_wrap.cxx -I/usr/include/python2.7 -I../../src/ -I/usr/local/include

#3. create shared object
gcc -shared ../../build/src/dictionary_dirpath.cpp.1.o ../../build/src/normalize_numexp.cpp.1.o ../../build/src/abstime_expression_normalizer.cpp.1.o ../../build/src/digit_utility.cpp.1.o ../../build/src/duration_expression_normalizer.cpp.1.o ../../build/src/normalizer_utility.cpp.1.o ../../build/src/number_normalizer.cpp.1.o ../../build/src/numerical_expression_normalizer.cpp.1.o ../../build/src/reltime_expression_normalizer.cpp.1.o ../../build/src/inappropriate_expression_remover.cpp.1.o normalize_numexp_wrap.o -o _normalize_numexp.so -I/usr/include/python2.7 -L/usr/local/lib -lpficommon -lpficommon_visualization -lpficommon_text -lpficommon_network_base -lpficommon_concurrent -lpficommon_data -lpficommon_math -lpficommon_system -lpficommon_network_http -lpficommon_lang -lpficommon_network_rpc -lpficommon_network_cgi -lux

#4.
echo finished!
echo 'please copy "_normalize_numexp.so" and "normalize_numexp.py" to your python "site-packages"'

#!/bin/sh
#http://www.swig.org/Doc2.0/SWIGDocumentation.html

#1. create _wrap.cxx by swig.
swig -c++ -ruby -I../../src/  -o normalize_numexp_wrap.cxx ../normalize_numexp.i

#2. compile _wrap.cxx (ruby.h and config.h are necessary.)
gcc -O2 -fPIC -c normalize_numexp_wrap.cxx -I../../src/ -I/usr/include/ruby-1.9.1 -I/usr/include/ruby-1.9.1/x86_64-linux

#3. create shared object
gcc -shared ../../build/src/dictionary_dirpath.cpp.1.o ../../build/src/normalize_numexp.cpp.1.o ../../build/src/abstime_expression_normalizer.cpp.1.o ../../build/src/digit_utility.cpp.1.o ../../build/src/duration_expression_normalizer.cpp.1.o ../../build/src/normalizer_utility.cpp.1.o ../../build/src/number_normalizer.cpp.1.o ../../build/src/numerical_expression_normalizer.cpp.1.o ../../build/src/reltime_expression_normalizer.cpp.1.o ../../build/src/inappropriate_expression_remover.cpp.1.o normalize_numexp_wrap.o -o normalize_numexp.so -I/usr/include/ruby-1.9.1 -L/usr/local/lib -lpficommon -lpficommon_visualization -lpficommon_text -lpficommon_network_base -lpficommon_concurrent -lpficommon_data -lpficommon_math -lpficommon_system -lpficommon_network_http -lpficommon_lang -lpficommon_network_rpc -lpficommon_network_cgi -lux

#4.
echo finished!
echo 'please copy "normalize_numexp.so" to your ruby "site-ruby"'

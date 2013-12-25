#!/bin/sh
#http://www.swig.org/Doc2.0/SWIGDocumentation.html

[ -d src ] || mkdir src
[ -d classes ] || mkdir classes

#1. create _wrap.cxx and wrapper source files by swig (you can change the package name)
swig -c++ -java -I../../src/  -o normalize_numexp_wrap.cxx -package jp.ac.tohoku.ecei.cl.numexp  -outdir src ../normalize_numexp.i

#2. compile _wrap.cxx (JNI header files are necessary)
gcc -O2 -fPIC -c normalize_numexp_wrap.cxx -I../../src/ -I/usr/lib/jvm/java-7-oracle/include -I/usr/lib/jvm/java-7-oracle/include/linux

#3. create shared library
gcc -shared ../../build/src/dictionary_dirpath.cpp.1.o ../../build/src/normalize_numexp.cpp.1.o ../../build/src/abstime_expression_normalizer.cpp.1.o ../../build/src/digit_utility.cpp.1.o ../../build/src/duration_expression_normalizer.cpp.1.o ../../build/src/normalizer_utility.cpp.1.o ../../build/src/number_normalizer.cpp.1.o ../../build/src/numerical_expression_normalizer.cpp.1.o ../../build/src/reltime_expression_normalizer.cpp.1.o ../../build/src/inappropriate_expression_remover.cpp.1.o normalize_numexp_wrap.o -o libnormalize_numexp.so -I/usr/lib/jvm/java-7-oracle/include -L/usr/local/lib -lpficommon -lpficommon_visualization -lpficommon_text -lpficommon_network_base -lpficommon_concurrent -lpficommon_data -lpficommon_math -lpficommon_system -lpficommon_network_http -lpficommon_lang -lpficommon_network_rpc -lpficommon_network_cgi -lux

#4. create wrapper classes
javac -d classes src/*.java

echo finished!
echo 'how to use: "java -Djava.library.path=<path to libnormalize_numexp.so> -classpath <path to wrapper classes> 〜"'

#!/bin/sh
pwd
rm _numerical_expression_extractor.so
#gcc -O2 -fPIC -c numerical_expression_extractor.cpp -I/home/katsuma/usr/local/include 
#echo compiled nee.cpp
gcc -O2 -fPIC -c numerical_expression_extractor_wrap.cxx -I/home/katsuma/local/python/include/python2.7 -I/home/katsuma/usr/local/include
echo compiled nee_wrap.cxx
gcc -shared /home/katsuma/src/digit_utils/build/src/numerical_expression_extractor.cpp.1.o /home/katsuma/src/digit_utils/build/src/abstime_expression_normalizer.cpp.1.o /home/katsuma/src/digit_utils/build/src/digit_utility.cpp.1.o /home/katsuma/src/digit_utils/build/src/duration_expression_normalizer.cpp.1.o /home/katsuma/src/digit_utils/build/src/normalizer_utility.cpp.1.o /home/katsuma/src/digit_utils/build/src/number_normalizer.cpp.1.o /home/katsuma/src/digit_utils/build/src/numerical_expression_normalizer.cpp.1.o /home/katsuma/src/digit_utils/build/src/reltime_expression_normalizer.cpp.1.o numerical_expression_extractor_wrap.o -o _numerical_expression_extractor.so -I/home/katsuma/local/python/include/python2.7 -L/home/katsuma/usr/local/lib -lpficommon -lpficommon_visualization -lpficommon_text -lpficommon_network_base -lpficommon_concurrent -lpficommon_data -lpficommon_math -lpficommon_system -lpficommon_network_http -lpficommon_lang -lpficommon_network_rpc -lpficommon_network_cgi -lux
echo finished!

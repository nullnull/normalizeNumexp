#!/bin/sh
swig -c++ -python -I../../src/  -o numerical_expression_extractor_wrap.cxx ../numerical_expression_extractor.i

%module numerical_expression_extractor

%{
  #define SWIG_FILE_WITH_INIT
  #include "numerical_expression_extractor.hpp"
%}

%include "std_string.i"
%include "std_vector.i"

namespace std {
  %template(IntVector) vector<int>;
  %template(DoubleVector) vector<double>;
  %template(StringVector) vector<std::string>;
 }

%include "numerical_expression_extractor.hpp"


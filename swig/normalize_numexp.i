%module normalize_numexp

%{
  #define SWIG_FILE_WITH_INIT
  #include "normalize_numexp.hpp"
%}

%include "std_string.i"
%include "std_vector.i"

namespace std {
  %template(IntVector) vector<int>;
  %template(DoubleVector) vector<double>;
  %template(StringVector) vector<std::string>;
 }

%include "normalize_numexp.hpp"


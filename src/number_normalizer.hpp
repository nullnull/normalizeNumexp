#ifndef NUMBER_NORMALIZER_H_
#define NUMBER_NORMALIZER_H_
#include <string>
#include <vector>
#include <float.h>
#include <pficommon/data/string/ustring.h>
#include <pficommon/data/string/utility.h>
#include "digit_utility.hpp"

namespace number_normalizer {
using namespace digit_utility;


class NumberExtractor {
public:
  void extract_number(const std::string& input, std::vector<Number>& output);
private:
  bool is_invalid_notation_type(int notation_type);
  void return_longest_number_strings(const pfi::data::string::ustring& utext, int &i, std::string& numstr);
};



class NumberConverterTemplate {
public:
  void convert_number(const pfi::data::string::ustring& number_string_org, double& value, int& number_type);
protected:
  virtual void convert_arabic_kansuji_mixed_of_4digit(const pfi::data::string::ustring& number_string, int& number_converted) = 0;
  void delete_comma(const pfi::data::string::ustring& ustr, pfi::data::string::ustring& ret);
  void convert_arabic_numerals(const pfi::data::string::ustring& number_string, double& value);
  void convert_arabic_kansuji_kurai_man_mixed(const pfi::data::string::ustring& number_string, double& value);
  void convert_arabic_kansuji_mixed(const pfi::data::string::ustring& number_string, double& value);
};
  
class JapaneseNumberConverter : public NumberConverterTemplate{
private:
  void convert_arabic_kansuji_mixed_of_4digit(const pfi::data::string::ustring& number_string, int& number_converted);
};

class ChineseNumberConverter : public NumberConverterTemplate{
private:
  void convert_arabic_kansuji_mixed_of_4digit(const pfi::data::string::ustring& number_string, int& number_converted);
};

class ArabicNumberConverter : public NumberConverterTemplate{
public:
  void convert_number(const pfi::data::string::ustring& number_string_org, double& value, int& number_type);
private:
  void convert_arabic_kansuji_mixed_of_4digit(const pfi::data::string::ustring& number_string, int& number_converted);
};
  


class SymbolFixer {
public:
  void fix_numbers_by_symbol(const std::string& text, std::vector<Number>& numbers);
private:
  bool is_plus(const pfi::data::string::ustring& utext, int i, pfi::data::string::ustring& plus_strings);
  bool is_minus(const pfi::data::string::ustring& utext, int i, pfi::data::string::ustring& plus_strings);
  void fix_prefix_symbol(const pfi::data::string::ustring& utext, std::vector<Number>& numbers, int i);
  double create_decimal_value(const Number& number);
  void fix_decimal_point(std::vector<Number>& numbers, int i, pfi::data::string::ustring decimal_strings);
  void fix_range_expression(std::vector<Number>& numbers, int i, pfi::data::string::ustring range_strings);
  void fix_intermediate_symbol(const pfi::data::string::ustring& utext, std::vector<Number>& numbers, int i);
  void fix_suffix_symbol(const pfi::data::string::ustring& utext, std::vector<Number>& numbers, int i);
};



class NumberNormalizer {
public:
  NumberNormalizer(const std::string& language) {language_ = language; digit_utility::init_kansuji(language); }
  ;
  void process(const std::string& input, std::vector<Number>& output);
  void process_dont_fix_by_symbol(const std::string& input, std::vector<Number>& output); //絶対時間表現の規格化の際に使用する（絶対時間表現では、前もって記号を処理させないため）
  
private:
  std::string language_;
};
  
} //namespace number_normalizer

#endif //NUMBER_NORMALIZER_H_

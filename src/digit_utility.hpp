#ifndef DIGIT_UTILITY_H_
#define DIGIT_UTILITY_H_
#include <map>
#include <string>
#include <float.h>
#include <complex>
#include <pficommon/data/string/ustring.h>

namespace digit_utility {
//const double DOUBLE_NULL = INFINITY;
//std::string dictionary_dirpath("/home/katsuma/src/digit_utils/src/dic/");

enum ENotationType {
  NOT_NUMBER = 0,
  KANSUJI_09 = 1,
  KANSUJI_KURAI_SEN = 2,
  KANSUJI_KURAI_MAN = 4,
  KANSUJI_KURAI = 6,
  KANSUJI = 7,
  ZENKAKU = 8,
  HANKAKU = 16,
};

struct Number {
  Number()
      : original_expression(pfi::data::string::string_to_ustring("")),
        position_start(-1),
        position_end(-1),
        value_lowerbound(INFINITY),
        value_upperbound(-INFINITY),
        notation_type(NOT_NUMBER) {
  }

  Number(pfi::data::string::ustring& original_expression, int position_start, int position_end)
      : original_expression(original_expression),
        position_start(position_start),
        position_end(position_end),
        value_lowerbound(INFINITY),
        value_upperbound(-INFINITY),
        notation_type(NOT_NUMBER) {
  }

  pfi::data::string::ustring original_expression;
  int position_start;
  int position_end;
  double value_lowerbound;
  double value_upperbound;
  int notation_type;
};

void init_kansuji(const std::string& language);
bool is_hankakusuji(pfi::data::string::uchar uc);
bool is_zenkakusuji(pfi::data::string::uchar uc);
bool is_arabic(pfi::data::string::uchar uc);
bool is_kansuji(pfi::data::string::uchar uc);
bool is_kansuji_09(pfi::data::string::uchar uc);
bool is_kansuji_kurai_sen(pfi::data::string::uchar uc);
bool is_kansuji_kurai_man(pfi::data::string::uchar uc);
bool is_kansuji_kurai(pfi::data::string::uchar uc);
bool is_number(pfi::data::string::uchar uc);
bool is_comma(pfi::data::string::uchar uc);
bool is_decimal_point(const pfi::data::string::ustring& ustr);
bool is_range_expression(const pfi::data::string::ustring& ustr);
int convert_kansuji_09_to_value(pfi::data::string::uchar uc);
int convert_kansuji_kurai_to_power_value(pfi::data::string::uchar uc);
}

#endif //DIGIT_UTILITY_H_

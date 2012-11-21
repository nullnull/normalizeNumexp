#ifndef NORMALIZER_UTILITY_H_
#define NORMALIZER_UTILITY_H_
#include <string>
#include <pficommon/data/string/ustring.h>
#include <pficommon/data/string/utility.h>
#include <pficommon/text/json.h>
#include <ux/ux.hpp>
#include "digit_utility.hpp"

namespace normalizer_utility {
using namespace digit_utility;
struct Time {
  Time(const double value) {
    year = month = day = hour = minute = second = value;
  }
  Time(const double &year, const double &month, const double &day, const double &hour, const double &minute, const double &second)
      : year(year),
        month(month),
        day(day),
        hour(hour),
        minute(minute),
        second(second) {
  }
  const bool operator==(const Time& t){
    return t.year == year &&
           t.month == month &&
           t.day == day &&
           t.hour == hour &&
           t.minute == minute &&
           t.second == second;
  }

  double year, month, day, hour, minute, second;
};

class NormalizedExpressionTemplate {
public:
  NormalizedExpressionTemplate(const pfi::data::string::ustring& original_expression, const int position_start, const int position_end)
      : original_expression(original_expression),
        position_start(position_start),
        position_end(position_end),
        number_notation_type(NOT_NUMBER),
        include_lowerbound(true),
        include_upperbound(true),
        is_over(false),
        is_less(false),
        ordinary(false) {
      options.clear();
  }
  
  void set_original_expression_from_position(const pfi::data::string::ustring& utext);

  pfi::data::string::ustring original_expression;
  int position_start, position_end;
  int number_notation_type;
  bool include_lowerbound, include_upperbound;
  bool is_over, is_less;
  bool ordinary;
  std::vector<std::string> options;
};
  
class LimitedExpressionTemplate {
public:
  void set_total_number_of_place_holder();
  void set_length_of_strings_after_final_place_holder();
  
  std::string pattern;
  bool ordinary;
  std::string option;
  int total_number_of_place_holder; //patternが含むPLACE_HOLDERの数（ *月*日 -> 2個）
  int length_of_strings_after_final_place_holder; //pattern中の最後のPLACE_HOLDERの後に続く文字列の長さ（*月*日 -> 1）　positionの同定に必要
};

struct NumberModifier {
  template<class Archive>
  void serialize(Archive &ar) {
    ar & MEMBER(pattern)& MEMBER(process_type);
  }
  std::string pattern, process_type;
};

void extract_after_string(const pfi::data::string::ustring& text, int i, pfi::data::string::ustring& after_string);
void extract_before_string(const pfi::data::string::ustring& text, int i, pfi::data::string::ustring& before_string);
void prefixSearch(const pfi::data::string::ustring& ustr, const ux::Map<int>& patterns, int& matching_pattern_id);
void suffixSearch(const pfi::data::string::ustring& ustr, const ux::Map<int>& patterns_rev, int& matching_pattern_id);
void search_suffix_number_modifier(const pfi::data::string::ustring& text, const int exp_position_end,
    const ux::Map<int>& suffix_number_modifier_patterns, int& matching_pattern_id);
void search_prefix_number_modifier(const pfi::data::string::ustring& text, const int exp_position_start,
    const ux::Map<int>& prefix_number_modifier_patterns, int& matching_pattern_id);
void replace_numbers_in_text(const pfi::data::string::ustring& utext, const std::vector<digit_utility::Number>& numbers,
    pfi::data::string::ustring& utext_replaced);
void shorten_place_holder_in_text(const pfi::data::string::ustring& utext, pfi::data::string::ustring& utext_shortened);
bool is_place_holder(pfi::data::string::uchar uc);
bool is_finite(double value);
bool is_null_time(const Time& t);
const std::string identify_time_detail(const normalizer_utility::Time& time);
std::string reverse_string(const std::string& str);

template<class T1, class T2>
void cast(const T1& a, T2& b) {
  std::stringstream ss;
  ss << a;
  ss >> b;
}
const pfi::data::string::ustring PLACE_HOLDER = pfi::data::string::string_to_ustring("ǂ"); //LATIN LETTER ALVEOLAR CLICK

} //normalizer_utility

#endif //NORMALIZER_UTILITY_H_

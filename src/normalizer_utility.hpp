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
class Time {
public:
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

  bool is_null_time_element(double t, bool is_upperbound){
		if(is_upperbound) return t==-INFINITY;
		else return t==INFINITY;
  }
	
	bool is_infinity_time_element(double t, bool is_upperbound){
		if(is_upperbound) return t==INFINITY;
		else return t==-INFINITY;
	}
  
  std::string to_string_from_time_element(double t, std::string null_string, std::string kugiri, bool is_upperbound, int width){
		std::stringstream ss;
	  std::string ret;
	  if(is_null_time_element(t, is_upperbound)){
		  return null_string + kugiri;
		}else{
			ss.fill('0'); ss.width(width);
		  ss << t << kugiri;
		  ss >> ret;
		  return ret;
	  }
  }
	
	std::string to_interval_string_from_time_element(double t, std::string time_position, bool is_upperbound){
		std::stringstream ss;
	  std::string ret;
	  if(is_null_time_element(t, is_upperbound)){
		  return "";
		}else{
		  ss << t << time_position;
		  ss >> ret;
		  return ret;
	  }
  }
	
	std::string to_string(bool is_upperbound){
		if(is_null_time_element(year, is_upperbound) and is_null_time_element(month, is_upperbound) and is_null_time_element(day, is_upperbound)){
			return to_time_string(is_upperbound);
		}else{
			return to_date_string(is_upperbound);
		}
	}
  
  std::string to_date_string(bool is_upperbound){
	 std::stringstream ss;
	 std::string ret;
	 if(is_infinity_time_element(year, is_upperbound)){ 
		if(is_upperbound) return "INF";
		else return "-INF";
	 }
	 ss << to_string_from_time_element(year, "XXXX", "-", is_upperbound, 4);
	 ss << to_string_from_time_element(month, "XX", "-", is_upperbound, 2);
	 ss << to_string_from_time_element(day, "XX", "", is_upperbound, 2);
	 ss >> ret;
	 return ret;
  }
	
	std::string to_time_string(bool is_upperbound){
		std::stringstream ss;
		std::string ret;
		if(is_infinity_time_element(year, is_upperbound)){ 
			if(is_upperbound) return "INF";
			else return "-INF";
		}
		ss << to_string_from_time_element(hour, "XX", ":", is_upperbound, 3);
		ss << to_string_from_time_element(minute, "XX", ":", is_upperbound, 3);
		ss << to_string_from_time_element(second, "XX", "", is_upperbound, 2);
		ss >> ret;
		return ret;
  }
	
	std::string to_duration_string(bool is_upperbound){
		std::stringstream ss;
		std::string ret;
		if(is_infinity_time_element(year, is_upperbound)){ 
			if(is_upperbound) return "INF";
			else return "-INF";
		}
		ss << "P";
		ss << to_interval_string_from_time_element(year, "Y", is_upperbound);
		ss << to_interval_string_from_time_element(month, "M", is_upperbound);
		ss << to_interval_string_from_time_element(day, "D", is_upperbound);
		ss << to_interval_string_from_time_element(hour, "h", is_upperbound);
		ss << to_interval_string_from_time_element(minute, "m", is_upperbound);
		ss << to_interval_string_from_time_element(second, "s", is_upperbound);
		ss >> ret;
		return ret;
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

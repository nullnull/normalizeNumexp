#include <algorithm>
#include "normalizer_utility.hpp"
#include <pficommon/lang/exception.h>

namespace normalizer_utility {
  
void NormalizedExpressionTemplate::set_original_expression_from_position(const pfi::data::string::ustring& utext){
  original_expression = utext.substr(position_start, position_end - position_start);
}

pfi::data::string::ustring reverse_string(const pfi::data::string::ustring& ustr) {
  return pfi::data::string::ustring(ustr.rbegin(), ustr.rend());
}
  
std::string reverse_string(const std::string& str) {
  pfi::data::string::ustring ustr = pfi::data::string::string_to_ustring(str);
  return pfi::data::string::ustring_to_string(reverse_string(ustr));
}

void extract_after_string(const pfi::data::string::ustring& text, const int i, pfi::data::string::ustring& after_string) {
  after_string = text.substr(i, text.size() - i);
}

void extract_before_string(const pfi::data::string::ustring& text, const int i, pfi::data::string::ustring& before_string) {
  before_string = text.substr(0, i);
}

void prefixSearch(const pfi::data::string::ustring& ustr, const ux::Map<int>& patterns, int& matching_pattern_id) {
  /*patternsの中から、ustrのprefixになっているものを探索（複数ある場合は最長のもの）
   */
  pfi::data::string::ustring ustr_shortened;
  shorten_place_holder_in_text(ustr, ustr_shortened); //ustrは数字が一字一字、「*」に変換されているので、patternsの表記と食い違っている。*を縮約する操作を行う
  std::string str = pfi::data::string::ustring_to_string(ustr_shortened);
  size_t retLen;
  int ret = patterns.prefixSearch(str.c_str(), str.size(), retLen, matching_pattern_id);
  if (ret == -1) matching_pattern_id = -1;
}

void suffixSearch(const pfi::data::string::ustring& ustr, const ux::Map<int>& patterns_rev, int& matching_pattern_id) {
  /*patternsの中から、ustrのsuffixになっているものを探索（複数ある場合は最長のもの）
   あらかじめpatternsの文字列を逆にしたものを保管しておき（patterns_rev）、ustrも逆にしてしまい、その状態でprefixSearchを行った結果を返す
   */
  pfi::data::string::ustring ustr_shortened;
  shorten_place_holder_in_text(ustr, ustr_shortened); //ustrは数字が一字一字、「*」に変換されているので、patternsの表記と食い違っている。*を縮約する操作を行う
  pfi::data::string::ustring ustr_rev = reverse_string(ustr_shortened);
  std::string str_rev = pfi::data::string::ustring_to_string(ustr_rev);
  size_t retLen;
  int ret = patterns_rev.prefixSearch(str_rev.c_str(), str_rev.size(), retLen, matching_pattern_id);
  if (ret == -1) matching_pattern_id = -1;
}

void search_suffix_number_modifier(const pfi::data::string::ustring& text, const int exp_position_end,
    const ux::Map<int>& suffix_number_modifier_patterns, int& matching_pattern_id) {
  pfi::data::string::ustring string_after_expression;
  extract_after_string(text, exp_position_end, string_after_expression);
  prefixSearch(string_after_expression, suffix_number_modifier_patterns, matching_pattern_id);
}

void search_prefix_number_modifier(const pfi::data::string::ustring& text, const int exp_position_start,
    const ux::Map<int>& prefix_number_modifier_patterns, int& matching_pattern_id) {
  pfi::data::string::ustring string_before_expression;
  extract_before_string(text, exp_position_start, string_before_expression);
  suffixSearch(string_before_expression, prefix_number_modifier_patterns, matching_pattern_id);
}

void replace_numbers_in_text(const pfi::data::string::ustring& utext, const std::vector<digit_utility::Number>& numbers,
    pfi::data::string::ustring& utext_replaced) {
  //「1989年7月」 -> 「****年*月」のように数の部分を置き換える（正規表現で一致させるため）
  utext_replaced = utext;
  for (int i = 0; i < static_cast<int>(numbers.size()); i++) {
    std::fill(utext_replaced.begin() + numbers[i].position_start, utext_replaced.begin() + numbers[i].position_end, PLACE_HOLDER[0]);
  }
}

void shorten_place_holder_in_text(const pfi::data::string::ustring& utext, pfi::data::string::ustring& utext_shortened) {
  //「****年*月」 -> 「*年*月」のように数の部分を縮約する（uxのprefixSearchで一致させるため）
  utext_shortened.clear();
  bool prev_is_place_holder = false;
  for (int i = 0; i < static_cast<int>(utext.size()); i++) {
    if (utext[i] == PLACE_HOLDER[0]) {
      if (prev_is_place_holder) {
        ;
      } else {
        utext_shortened += PLACE_HOLDER;
        prev_is_place_holder = true;
      }
    } else {
      utext_shortened += utext[i];
      prev_is_place_holder = false;
    }
  }
}

void build_number_modifier_patterns_from_number_modifiers(const std::vector<normalizer_utility::NumberModifier>& number_modifiers,
    ux::Map<int>& number_modifier_patterns) {
  //patternでの探索を可能にするため、トライ木を構築する。
  std::vector<std::pair<std::string, int> > kvs;
  for (int i = 0; i < static_cast<int>(number_modifiers.size()); i++) {
    kvs.push_back(std::make_pair(number_modifiers[i].pattern, i));
  }
  number_modifier_patterns.build(kvs);
}
  
bool is_place_holder(pfi::data::string::uchar uc) {
  return uc == normalizer_utility::PLACE_HOLDER[0];
}

bool is_finite(double value){
  return value != INFINITY and value != -INFINITY;
}
  
bool is_null_time(const Time& t){
  Time positive_inf(INFINITY), negative_inf(-INFINITY);
  return (positive_inf == t) || (negative_inf == t);
}

const std::string identify_time_detail(const normalizer_utility::Time& time) {
  if (normalizer_utility::is_finite(time.second)) {
    return "s";
  } else if (normalizer_utility::is_finite(time.minute)) {
    return "mn";
  } else if (normalizer_utility::is_finite(time.hour)) {
    return "h";
  } else if (normalizer_utility::is_finite(time.day)) {
    return "d";
  } else if (normalizer_utility::is_finite(time.month)) {
    return "m";
  } else if (normalizer_utility::is_finite(time.year)) {
    return "y";
  }
  return "";
}

void LimitedExpressionTemplate::set_total_number_of_place_holder() {
  pfi::data::string::ustring ustr_pattern = pfi::data::string::string_to_ustring(pattern);
  total_number_of_place_holder = static_cast<int>(count_if(ustr_pattern.begin(), ustr_pattern.end(), normalizer_utility::is_place_holder));
}

void LimitedExpressionTemplate::set_length_of_strings_after_final_place_holder() {
  pfi::data::string::ustring ustr_pattern = pfi::data::string::string_to_ustring(pattern);
  length_of_strings_after_final_place_holder = ustr_pattern.size() - ustr_pattern.rfind(normalizer_utility::PLACE_HOLDER[0]) - 1;
}
} //normalizer_utility


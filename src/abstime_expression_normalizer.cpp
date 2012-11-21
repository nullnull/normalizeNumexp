#include <math.h>
#include "abstime_expression_normalizer.hpp"
#include "digit_utility.hpp"
#include "number_normalizer.hpp"

namespace abstime_expression_normalizer {

void AbstimeExpressionNormalizer::init() {
  load_from_dictionaries("abstime_expression_json.txt", "abstime_prefix_counter_json.txt", "abstime_prefix_json.txt", "abstime_suffix_json.txt");
}

void AbstimeExpressionNormalizer::normalize_number(const std::string& text, std::vector<digit_utility::Number>& numbers) {
  NN.process_dont_fix_by_symbol(text, numbers);
}
  
void set_time(AbstimeExpression& abstimeexp, const std::string& corresponding_time_position, const AbstimeExpression& integrate_abstimeexp) {
  if (corresponding_time_position == "y") {
    abstimeexp.value_lowerbound.year = integrate_abstimeexp.org_value_lowerbound;
    abstimeexp.value_upperbound.year = integrate_abstimeexp.org_value_upperbound;
  } else if (corresponding_time_position == "m") {
    abstimeexp.value_lowerbound.month = integrate_abstimeexp.org_value_lowerbound;
    abstimeexp.value_upperbound.month = integrate_abstimeexp.org_value_upperbound;
  } else if (corresponding_time_position == "d") {
    abstimeexp.value_lowerbound.day = integrate_abstimeexp.org_value_lowerbound;
    abstimeexp.value_upperbound.day = integrate_abstimeexp.org_value_upperbound;
  } else if (corresponding_time_position == "h") {
    abstimeexp.value_lowerbound.hour = integrate_abstimeexp.org_value_lowerbound;
    abstimeexp.value_upperbound.hour = integrate_abstimeexp.org_value_upperbound;
  } else if (corresponding_time_position == "mn") {
    abstimeexp.value_lowerbound.minute = integrate_abstimeexp.org_value_lowerbound;
    abstimeexp.value_upperbound.minute = integrate_abstimeexp.org_value_upperbound;
  } else if (corresponding_time_position == "s") {
    abstimeexp.value_lowerbound.second = integrate_abstimeexp.org_value_lowerbound;
    abstimeexp.value_upperbound.second = integrate_abstimeexp.org_value_upperbound;
  } else if (corresponding_time_position == "seiki") {
    abstimeexp.value_lowerbound.year = integrate_abstimeexp.org_value_lowerbound*100-99;
    abstimeexp.value_upperbound.year = integrate_abstimeexp.org_value_upperbound*100;
  }
}

void revise_abstimeexp_by_process_type(AbstimeExpression& abstimeexp, std::string process_type) {
  if (process_type == "gogo") {
    abstimeexp.value_lowerbound.hour += 12;
    abstimeexp.value_upperbound.hour += 12;
  }else if (process_type == "han") {
    abstimeexp.value_lowerbound.minute = 30;
    abstimeexp.value_upperbound.minute = 30;
  }
}

void AbstimeExpressionNormalizer::revise_any_type_expression_by_matching_limited_expression(std::vector<AbstimeExpression>& abstimeexps, int &expression_id,
    const LimitedAbstimeExpression matching_limited_abstime_expression) {
  int final_integrated_abstimeexp_id = expression_id + matching_limited_abstime_expression.total_number_of_place_holder;
  abstimeexps[expression_id].position_end = abstimeexps[final_integrated_abstimeexp_id].position_end
      + matching_limited_abstime_expression.length_of_strings_after_final_place_holder;
  for (int i = 0; i < static_cast<int>(matching_limited_abstime_expression.corresponding_time_position.size()); i++) {
    set_time(abstimeexps[expression_id], matching_limited_abstime_expression.corresponding_time_position[i], abstimeexps[expression_id + i]);
  }
  for (int i = 0; i < static_cast<int>(matching_limited_abstime_expression.process_type.size()); i++) {
    revise_abstimeexp_by_process_type(abstimeexps[expression_id], matching_limited_abstime_expression.process_type[i]);
  }
  abstimeexps[expression_id].ordinary = matching_limited_abstime_expression.ordinary;
  abstimeexps.erase(abstimeexps.begin() + expression_id + 1,
      abstimeexps.begin() + expression_id + 1 + matching_limited_abstime_expression.total_number_of_place_holder);
}

void AbstimeExpressionNormalizer::revise_any_type_expression_by_matching_prefix_counter(AbstimeExpression& any_type_expression, const LimitedAbstimeExpression& matching_limited_expression) {
  if(matching_limited_expression.option == "seireki"){
    int tmp;
    normalizer_utility::cast(matching_limited_expression.process_type[0], tmp);
    any_type_expression.value_lowerbound.year += tmp;
    any_type_expression.value_upperbound.year += tmp;
  }
  any_type_expression.position_start -= pfi::data::string::string_to_ustring(matching_limited_expression.pattern).size();
}

/*
 aboutタイプなどの処理は規格化の際に行わないことになったので、コメントアウト。
 規格化の際に処理を行うとすると、以下のような処理を行う予定。
 
void do_time_about(AbstimeExpression& abstimeexp) { //TODO : 「about」という処理タイプの処理を行う
  normalizer_utility::Time &tvl = abstimeexp.value_lowerbound, &tvu = abstimeexp.value_upperbound;
  const std::string target_time_position = normalizer_utility::identify_time_detail(abstimeexp.value_lowerbound);
  if (target_time_position == "year") {
    //TODO : 「1800年ごろ」という指定なら1790~1810くらいだと感じる　「1811年ごろ」という指定なら1810~1812くらいだと感じる　　どんな実装が良いか相談
    tvl.year -= 1;
    tvu.year += 1;
  } else if (target_time_position == "month") {
    tvl.month -= 1;
    tvu.month += 1;
  } else if (target_time_position == "day") {
    tvl.day -= 1;
    tvu.day += 1;
  } else if (target_time_position == "hour") {
    tvl.hour -= 1;
    tvu.hour += 1;
  } else if (target_time_position == "minute") {
    tvl.minute -= 5;
    tvu.minute += 5;
  } else if (target_time_position == "second") {
    tvl.second -= 5;
    tvu.second += 5;
  }
}

 //~~他の処理も同様
void do_time_zenhann(AbstimeExpression& abstimeexp) {
}
 */

void AbstimeExpressionNormalizer::revise_any_type_expression_by_number_modifier(AbstimeExpression& abstimeexp,
    const normalizer_utility::NumberModifier& number_modifier) {
  std::string process_type = number_modifier.process_type; 
  if (process_type == "or_over") {
    abstimeexp.value_upperbound = normalizer_utility::Time(INFINITY);
  } else if (process_type == "or_less") {
    abstimeexp.value_lowerbound = normalizer_utility::Time(-INFINITY);
  } else if (process_type == "over") {
    abstimeexp.value_upperbound = normalizer_utility::Time(INFINITY);
    abstimeexp.include_lowerbound = false;
  } else if (process_type == "less") {
    abstimeexp.value_lowerbound = normalizer_utility::Time(-INFINITY);
    abstimeexp.include_upperbound = false;
  } else if (process_type == "none") {
    ;
  } else {
    abstimeexp.options.push_back(process_type); // TODO : 「約」などの処理は規格化行わない？　要検討
  }
}
  
void AbstimeExpressionNormalizer::delete_not_any_type_expression(std::vector<AbstimeExpression>& abstimeexps){
  for(int i=0; i<static_cast<int>(abstimeexps.size()); i++){
    if(normalizer_utility::is_null_time(abstimeexps[i].value_lowerbound) && normalizer_utility::is_null_time(abstimeexps[i].value_upperbound)){
      abstimeexps.erase(abstimeexps.begin() + i);
      i--;
    }
  }
}

void AbstimeExpressionNormalizer::fix_by_range_expression(const pfi::data::string::ustring& utext, std::vector<AbstimeExpression>& abstimeexps) {
  for(int i=0; i<static_cast<int>(abstimeexps.size()-1); i++){
    if(have_kara_suffix(abstimeexps[i].options) && have_kara_prefix(abstimeexps[i+1].options)){
      abstimeexps[i].value_upperbound = abstimeexps[i+1].value_upperbound;
      abstimeexps[i].position_end = abstimeexps[i+1].position_end;
      abstimeexps[i].set_original_expression_from_position(utext);
      merge_options(abstimeexps[i].options, abstimeexps[i+1].options);
      abstimeexps.erase(abstimeexps.begin()+i+1);
    }
  }
}
} //namespace abstime_expression_normalizer


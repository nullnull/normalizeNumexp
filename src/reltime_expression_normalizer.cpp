#include <string>
#include <math.h>
#include <iostream>
#include "reltime_expression_normalizer.hpp"
#include "digit_utility.hpp"
#include "number_normalizer.hpp"

namespace reltime_expression_normalizer {
  
  void ReltimeExpressionNormalizer::init() {
    load_from_dictionaries("reltime_expression_json.txt", "reltime_prefix_counter_json.txt", "reltime_prefix_json.txt", "reltime_suffix_json.txt");
  }
  
  void ReltimeExpressionNormalizer::normalize_number(const std::string& text, std::vector<digit_utility::Number>& numbers) {
    NN.process(text, numbers);
  }
  
  void set_time(ReltimeExpression& reltimeexp, const std::string& corresponding_time_position, const ReltimeExpression& integrate_reltimeexp) {
    if (corresponding_time_position == "y") {
      reltimeexp.value_lowerbound_abs.year = integrate_reltimeexp.org_value_lowerbound;
      reltimeexp.value_upperbound_abs.year = integrate_reltimeexp.org_value_upperbound;
    } else if (corresponding_time_position == "m") {
      reltimeexp.value_lowerbound_abs.month = integrate_reltimeexp.org_value_lowerbound;
      reltimeexp.value_upperbound_abs.month = integrate_reltimeexp.org_value_upperbound;
    } else if (corresponding_time_position == "d") {
      reltimeexp.value_lowerbound_abs.day = integrate_reltimeexp.org_value_lowerbound;
      reltimeexp.value_upperbound_abs.day = integrate_reltimeexp.org_value_upperbound;
    } else if (corresponding_time_position == "h") {
      reltimeexp.value_lowerbound_abs.hour = integrate_reltimeexp.org_value_lowerbound;
      reltimeexp.value_upperbound_abs.hour = integrate_reltimeexp.org_value_upperbound;
    } else if (corresponding_time_position == "mn") {
      reltimeexp.value_lowerbound_abs.minute = integrate_reltimeexp.org_value_lowerbound;
      reltimeexp.value_upperbound_abs.minute = integrate_reltimeexp.org_value_upperbound;
    } else if (corresponding_time_position == "s") {
      reltimeexp.value_lowerbound_abs.second = integrate_reltimeexp.org_value_lowerbound;
      reltimeexp.value_upperbound_abs.second = integrate_reltimeexp.org_value_upperbound;
    } else if (corresponding_time_position == "seiki") {
      reltimeexp.value_lowerbound_abs.year = integrate_reltimeexp.org_value_lowerbound*100-99;
      reltimeexp.value_upperbound_abs.year = integrate_reltimeexp.org_value_upperbound*100;
    } else if (corresponding_time_position == "w") {
      reltimeexp.value_lowerbound_abs.day = integrate_reltimeexp.org_value_lowerbound*7;
      reltimeexp.value_upperbound_abs.day = integrate_reltimeexp.org_value_upperbound*7;
    }
    else if (corresponding_time_position == "+y") {
      reltimeexp.value_lowerbound_rel.year = integrate_reltimeexp.org_value_lowerbound;
      reltimeexp.value_upperbound_rel.year = integrate_reltimeexp.org_value_upperbound;
    } else if (corresponding_time_position == "+m") {
      reltimeexp.value_lowerbound_rel.month = integrate_reltimeexp.org_value_lowerbound;
      reltimeexp.value_upperbound_rel.month = integrate_reltimeexp.org_value_upperbound;
    } else if (corresponding_time_position == "+d") {
      reltimeexp.value_lowerbound_rel.day = integrate_reltimeexp.org_value_lowerbound;
      reltimeexp.value_upperbound_rel.day = integrate_reltimeexp.org_value_upperbound;
    } else if (corresponding_time_position == "+h") {
      reltimeexp.value_lowerbound_rel.hour = integrate_reltimeexp.org_value_lowerbound;
      reltimeexp.value_upperbound_rel.hour = integrate_reltimeexp.org_value_upperbound;
    } else if (corresponding_time_position == "+mn") {
      reltimeexp.value_lowerbound_rel.minute = integrate_reltimeexp.org_value_lowerbound;
      reltimeexp.value_upperbound_rel.minute = integrate_reltimeexp.org_value_upperbound;
    } else if (corresponding_time_position == "+s") {
      reltimeexp.value_lowerbound_rel.second = integrate_reltimeexp.org_value_lowerbound;
      reltimeexp.value_upperbound_rel.second = integrate_reltimeexp.org_value_upperbound;
    } else if (corresponding_time_position == "+seiki") {
      reltimeexp.value_lowerbound_rel.year = integrate_reltimeexp.org_value_lowerbound*100;
      reltimeexp.value_upperbound_rel.year = integrate_reltimeexp.org_value_upperbound*100;
    } else if (corresponding_time_position == "+seiki") {
      reltimeexp.value_lowerbound_rel.day = integrate_reltimeexp.org_value_lowerbound*7;
      reltimeexp.value_upperbound_rel.day = integrate_reltimeexp.org_value_upperbound*7;
    }
    else if (corresponding_time_position == "-y") {
      reltimeexp.value_lowerbound_rel.year = -integrate_reltimeexp.org_value_lowerbound;
      reltimeexp.value_upperbound_rel.year = -integrate_reltimeexp.org_value_upperbound;
    } else if (corresponding_time_position == "-m") {
      reltimeexp.value_lowerbound_rel.month = -integrate_reltimeexp.org_value_lowerbound;
      reltimeexp.value_upperbound_rel.month = -integrate_reltimeexp.org_value_upperbound;
    } else if (corresponding_time_position == "-d") {
      reltimeexp.value_lowerbound_rel.day = -integrate_reltimeexp.org_value_lowerbound;
      reltimeexp.value_upperbound_rel.day = -integrate_reltimeexp.org_value_upperbound;
    } else if (corresponding_time_position == "-h") {
      reltimeexp.value_lowerbound_rel.hour = -integrate_reltimeexp.org_value_lowerbound;
      reltimeexp.value_upperbound_rel.hour = -integrate_reltimeexp.org_value_upperbound;
    } else if (corresponding_time_position == "-mn") {
      reltimeexp.value_lowerbound_rel.minute = -integrate_reltimeexp.org_value_lowerbound;
      reltimeexp.value_upperbound_rel.minute = -integrate_reltimeexp.org_value_upperbound;
    } else if (corresponding_time_position == "-s") {
      reltimeexp.value_lowerbound_rel.second = -integrate_reltimeexp.org_value_lowerbound;
      reltimeexp.value_upperbound_rel.second = -integrate_reltimeexp.org_value_upperbound;
    } else if (corresponding_time_position == "-seiki") {
      reltimeexp.value_lowerbound_rel.year = -integrate_reltimeexp.org_value_lowerbound*100;
      reltimeexp.value_upperbound_rel.year = -integrate_reltimeexp.org_value_upperbound*100;
    } else if (corresponding_time_position == "-w") {
      reltimeexp.value_lowerbound_rel.day = -integrate_reltimeexp.org_value_lowerbound*7;
      reltimeexp.value_upperbound_rel.day = -integrate_reltimeexp.org_value_upperbound*7;
    }
  }
  
  void do_option_han(ReltimeExpression& reltimeexp, const std::string& corresponding_time_position){
    //TODO : 「週」、「世紀」に対応していない部分がちらほらある。
    if (corresponding_time_position == "+y") {
      reltimeexp.value_lowerbound_rel.year += 0.5;
      reltimeexp.value_upperbound_rel.year += 0.5;
    } else if (corresponding_time_position == "+m") {
      reltimeexp.value_lowerbound_rel.month += 0.5;
      reltimeexp.value_upperbound_rel.month += 0.5;
    } else if (corresponding_time_position == "+d") {
      reltimeexp.value_lowerbound_rel.day += 0.5;
      reltimeexp.value_upperbound_rel.day += 0.5;
    } else if (corresponding_time_position == "+h") {
      reltimeexp.value_lowerbound_rel.hour += 0.5;
      reltimeexp.value_upperbound_rel.hour += 0.5;
    } else if (corresponding_time_position == "+mn") {
      reltimeexp.value_lowerbound_rel.minute += 0.5;
      reltimeexp.value_upperbound_rel.minute += 0.5;
    } else if (corresponding_time_position == "+s") {
      reltimeexp.value_lowerbound_rel.second += 0.5;
      reltimeexp.value_upperbound_rel.second += 0.5;
    } else if (corresponding_time_position == "+seiki") {
      reltimeexp.value_lowerbound_rel.year += 50;
      reltimeexp.value_upperbound_rel.year += 50;
    } else  if (corresponding_time_position == "-y") {
      reltimeexp.value_lowerbound_rel.year -= 0.5;
      reltimeexp.value_upperbound_rel.year -= 0.5;
    } else if (corresponding_time_position == "-m") {
      reltimeexp.value_lowerbound_rel.month -= 0.5;
      reltimeexp.value_upperbound_rel.month -= 0.5;
    } else if (corresponding_time_position == "-d") {
      reltimeexp.value_lowerbound_rel.day -= 0.5;
      reltimeexp.value_upperbound_rel.day -= 0.5;
    } else if (corresponding_time_position == "-h") {
      reltimeexp.value_lowerbound_rel.hour -= 0.5;
      reltimeexp.value_upperbound_rel.hour -= 0.5;
    } else if (corresponding_time_position == "-mn") {
      reltimeexp.value_lowerbound_rel.minute -= 0.5;
      reltimeexp.value_upperbound_rel.minute -= 0.5;
    } else if (corresponding_time_position == "-s") {
      reltimeexp.value_lowerbound_rel.second -= 0.5;
      reltimeexp.value_upperbound_rel.second -= 0.5;
    } else if (corresponding_time_position == "-seiki") {
      reltimeexp.value_lowerbound_rel.year -= 50;
      reltimeexp.value_upperbound_rel.year -= 50;
    }
  }
  
  void revise_reltimeexp_by_process_type(ReltimeExpression& reltimeexp, std::string process_type, const LimitedReltimeExpression& matching_limited_reltime_expression) {
    if (process_type == "han") {
      if(matching_limited_reltime_expression.corresponding_time_position.empty()) return;
      std::string corresponding_time_position = matching_limited_reltime_expression.corresponding_time_position[matching_limited_reltime_expression.corresponding_time_position.size()-1];
      do_option_han(reltimeexp, corresponding_time_position);
    } else if (process_type == "or_over") {
      reltimeexp.value_upperbound_abs = normalizer_utility::Time(INFINITY);
    } else if (process_type == "or_less") {
      reltimeexp.value_lowerbound_abs = normalizer_utility::Time(-INFINITY);
    } else if (process_type == "over") {
      reltimeexp.value_upperbound_abs = normalizer_utility::Time(INFINITY);
      reltimeexp.include_lowerbound = false;
    } else if (process_type == "less") {
      reltimeexp.value_lowerbound_abs = normalizer_utility::Time(-INFINITY);
      reltimeexp.include_upperbound = false;
    } else if (process_type == "none") {
      ;
    } else if (process_type == "inai") {
      reltimeexp.value_lowerbound_rel = normalizer_utility::Time(0);
    }
    
  }
  
  void ReltimeExpressionNormalizer::revise_any_type_expression_by_matching_limited_expression(std::vector<ReltimeExpression>& reltimeexps, int &expression_id,
                                                                                              const LimitedReltimeExpression matching_limited_reltime_expression) {
    int final_integrated_reltimeexp_id = expression_id + matching_limited_reltime_expression.total_number_of_place_holder;
    reltimeexps[expression_id].position_end = reltimeexps[final_integrated_reltimeexp_id].position_end
    + matching_limited_reltime_expression.length_of_strings_after_final_place_holder;
    for (int i = 0; i < static_cast<int>(matching_limited_reltime_expression.corresponding_time_position.size()); i++) {
      set_time(reltimeexps[expression_id], matching_limited_reltime_expression.corresponding_time_position[i], reltimeexps[expression_id + i]);
    }
    for (int i = 0; i < static_cast<int>(matching_limited_reltime_expression.process_type.size()); i++) {
      revise_reltimeexp_by_process_type(reltimeexps[expression_id], matching_limited_reltime_expression.process_type[i], matching_limited_reltime_expression);
    }
    reltimeexps[expression_id].ordinary = matching_limited_reltime_expression.ordinary;
    
    reltimeexps.erase(reltimeexps.begin() + expression_id + 1,
                      reltimeexps.begin() + expression_id + 1 + matching_limited_reltime_expression.total_number_of_place_holder);
  }
  
  void ReltimeExpressionNormalizer::revise_any_type_expression_by_matching_prefix_counter(ReltimeExpression& reltimeexp, const LimitedReltimeExpression& matching_limited_expression) {
    if(matching_limited_expression.option == "add_relation"){
      int relation_value;
      normalizer_utility::cast(matching_limited_expression.process_type[0], relation_value);
      if (matching_limited_expression.corresponding_time_position[0] == "y") {
        reltimeexp.value_lowerbound_rel.year = relation_value;
        reltimeexp.value_upperbound_rel.year = relation_value;
      } else if (matching_limited_expression.corresponding_time_position[0] == "m") {
        reltimeexp.value_lowerbound_rel.month = relation_value;
        reltimeexp.value_upperbound_rel.month = relation_value;
      } else if (matching_limited_expression.corresponding_time_position[0] == "d") {
        reltimeexp.value_lowerbound_rel.day = relation_value;
        reltimeexp.value_upperbound_rel.day = relation_value;
      } else if (matching_limited_expression.corresponding_time_position[0] == "h") {
        reltimeexp.value_lowerbound_rel.hour = relation_value;
        reltimeexp.value_upperbound_rel.hour = relation_value;
      } else if (matching_limited_expression.corresponding_time_position[0] == "mn") {
        reltimeexp.value_lowerbound_rel.minute = relation_value;
        reltimeexp.value_upperbound_rel.minute = relation_value;
      } else if (matching_limited_expression.corresponding_time_position[0] == "s") {
        reltimeexp.value_lowerbound_rel.second = relation_value;
        reltimeexp.value_upperbound_rel.second = relation_value;
      }
    }
    reltimeexp.position_start -= pfi::data::string::string_to_ustring(matching_limited_expression.pattern).size();
  }
  
  void ReltimeExpressionNormalizer::revise_any_type_expression_by_number_modifier(ReltimeExpression& reltimeexp,
                                                                                  const normalizer_utility::NumberModifier& number_modifier) {
    std::string process_type = number_modifier.process_type;
    //or_overなどのタイプは、reltimeではexpression_jsonの方で記述されており、これはrevise_reltimeexp_by_process_typeで処理されるので、ここでは処理しない。（TODO : 辞書からありえない表現をのぞく）
    reltimeexp.options.push_back(process_type);
  }
  
  void ReltimeExpressionNormalizer::delete_not_any_type_expression(std::vector<ReltimeExpression>& reltimeexps){
    for(int i=0; i<static_cast<int>(reltimeexps.size()); i++){
      if(normalizer_utility::is_null_time(reltimeexps[i].value_lowerbound_rel) && normalizer_utility::is_null_time(reltimeexps[i].value_upperbound_rel)){
        reltimeexps.erase(reltimeexps.begin() + i);
        i--;
      }
    }
  }
  
  void ReltimeExpressionNormalizer::fix_by_range_expression(const pfi::data::string::ustring& utext, std::vector<ReltimeExpression>& reltimeexps) {
    for(int i=0; i<static_cast<int>(reltimeexps.size()-1); i++){
      if(have_kara_suffix(reltimeexps[i].options) && have_kara_prefix(reltimeexps[i+1].options)){
        reltimeexps[i].value_upperbound_rel = reltimeexps[i+1].value_upperbound_rel;
        reltimeexps[i].value_upperbound_abs = reltimeexps[i+1].value_upperbound_abs;
        reltimeexps[i].position_end = reltimeexps[i+1].position_end;
        reltimeexps[i].set_original_expression_from_position(utext);
        merge_options(reltimeexps[i].options, reltimeexps[i+1].options);
        reltimeexps.erase(reltimeexps.begin()+i+1);
      }
    }
  }
} //namespace reltime_expression_normalizer


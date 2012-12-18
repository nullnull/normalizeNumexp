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
	//修飾語でない、パターンに含まれるprocess_typeによる規格化表現の補正処理。
	if (process_type == "gozen") {
		if(abstimeexp.value_lowerbound == INFINITY){
			abstimeexp.value_lowerbound = 0;
			abstimeexp.value_upperbound = 12;
		}else{
			;
		}
	}else if (process_type == "gogo") {
		if(abstimeexp.value_lowerbound == INFINITY){
			abstimeexp.value_lowerbound = 12;
			abstimeexp.value_upperbound = 24;
		}else{
			abstimeexp.value_lowerbound.hour += 12;
			abstimeexp.value_upperbound.hour += 12;
		}
  }else if (process_type == "han") {
    abstimeexp.value_lowerbound.minute = 30;
    abstimeexp.value_upperbound.minute = 30;
  }
}

void AbstimeExpressionNormalizer::revise_any_type_expression_by_matching_limited_expression(std::vector<AbstimeExpression>& abstimeexps, int &expression_id,
    const LimitedAbstimeExpression matching_limited_abstime_expression) {
	//一致したパターンに応じて、規格化を行う
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
	//一致したパターンに応じて、規格化を行う（数字の前側に単位等が来る場合。絶対時間表現の場合「西暦」など）
  if(matching_limited_expression.option == "seireki"){
    int tmp;
    normalizer_utility::cast(matching_limited_expression.process_type[0], tmp);
    any_type_expression.value_lowerbound.year += tmp;
    any_type_expression.value_upperbound.year += tmp;
  }else if(matching_limited_expression.option == "gogo"){
		any_type_expression.value_lowerbound.hour += 12;
    any_type_expression.value_upperbound.hour += 12;
	}else if(matching_limited_expression.option == "gozen"){
	
	}
  any_type_expression.position_start -= pfi::data::string::string_to_ustring(matching_limited_expression.pattern).size();
}





/*
　修飾語による規格化表現の補正処理。
*/
 
void do_time_about(AbstimeExpression& abstimeexp) {
  normalizer_utility::Time &tvl = abstimeexp.value_lowerbound, &tvu = abstimeexp.value_upperbound;
  const std::string target_time_position = normalizer_utility::identify_time_detail(abstimeexp.value_lowerbound);
  if (target_time_position == "year") {
    //TODO : 「1800年ごろ」という指定なら1790~1810くらいだと感じる　「1811年ごろ」という指定なら1810~1812くらいだと感じる　　以下は雑な実装
    tvl.year -= 5;
    tvu.year += 5;
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
void do_time_zenhan(AbstimeExpression& abstimeexp) {
	//「18世紀前半」「1989年前半」「7月前半」「3日朝」など。
	//TODO : 「18世紀はじめ」などもzenhanに括ってしまっている。より細かく分類が行いたい場合は、hajime関数などを書いて処理
	normalizer_utility::Time &tvl = abstimeexp.value_lowerbound, &tvu = abstimeexp.value_upperbound;
  const std::string target_time_position = normalizer_utility::identify_time_detail(abstimeexp.value_lowerbound);
  if (target_time_position == "year") {
    if(tvl.year != tvu.year){
			//「18世紀前半」のとき
			tvu.year = (tvl.year + tvu.year)/2 -0.5;
		} else {
			//「1989年前半」のとき
			tvl.month = 1;
			tvu.month = 6;
		}
  } else if (target_time_position == "month") {
		//「7月前半」のとき
    tvl.day = 1;
		tvl.day = 15;
  } else if (target_time_position == "day") {
		//「3日朝」のとき
    tvl.hour = 5;
		tvu.hour = 12;
  } else {
		//これ以外でzenhanになる場合はない？　処理を行わない。
	}
}


void do_time_kouhan(AbstimeExpression& abstimeexp) {
	//「18世紀後半」「1989年後半」「7月後半」など。
	//TODO : 「18世紀末」「3日夜」などもkouhanに括ってしまっている。
	normalizer_utility::Time &tvl = abstimeexp.value_lowerbound, &tvu = abstimeexp.value_upperbound;
	const std::string target_time_position = normalizer_utility::identify_time_detail(abstimeexp.value_lowerbound);
	if (target_time_position == "year") {
		if(tvl.year != tvu.year){
			//「18世紀後半」のとき
			tvl.year = (tvl.year + tvu.year)/2 +0.5;;
		} else {
			//「1989年後半」のとき
			tvl.month = 7;
			tvu.month = 12;
		}
	} else if (target_time_position == "month") {
		//「7月後半」のとき
		tvl.day = 16;
		tvl.day = 31;
	} else if (target_time_position == "day") {
		//「3日夜」のとき
		tvl.hour = 18;
		tvu.hour = 24;
	} else {
		//これ以外の場合はない？　処理を行わない。
	}
}


void do_time_nakaba(AbstimeExpression& abstimeexp) {
	//「18世紀中盤」「1989年中盤」「7月中盤」など。
	//TODO : 「18世紀なかば」「3日昼」などもnakabaに括ってしまっている。
	normalizer_utility::Time &tvl = abstimeexp.value_lowerbound, &tvu = abstimeexp.value_upperbound;
	const std::string target_time_position = normalizer_utility::identify_time_detail(abstimeexp.value_lowerbound);
	if (target_time_position == "year") {
		if(tvl.year != tvu.year){
			//「18世紀中盤」のとき
			int tmp = (tvu.year - tvl.year)/4;
			tvl.year += tmp;
			tvu.year -= tmp;
		} else {
			//「1989年中盤」のとき
			tvl.month = 4;
			tvu.month = 9;
		}
	} else if (target_time_position == "month") {
		//「7月半ば」のとき
		tvl.day = 10;
		tvl.day = 20;
	} else if (target_time_position == "day") {
		//「3日昼」のとき
		tvl.hour = 10;
		tvu.hour = 15;
	} else {
		//これ以外の場合はない？　処理を行わない。
	}
}


void do_time_joujun(AbstimeExpression& abstimeexp) {
	normalizer_utility::Time &tvl = abstimeexp.value_lowerbound, &tvu = abstimeexp.value_upperbound;
	const std::string target_time_position = normalizer_utility::identify_time_detail(abstimeexp.value_lowerbound);
	if (target_time_position == "month") {
			tvl.day = 1;
			tvu.day = 10;
	} 
}

void do_time_tyujun(AbstimeExpression& abstimeexp) {
	normalizer_utility::Time &tvl = abstimeexp.value_lowerbound, &tvu = abstimeexp.value_upperbound;
	const std::string target_time_position = normalizer_utility::identify_time_detail(abstimeexp.value_lowerbound);
	if (target_time_position == "month") {
			tvl.day = 11;
			tvu.day = 20;
	} 
}

void do_time_gejun(AbstimeExpression& abstimeexp) {
	normalizer_utility::Time &tvl = abstimeexp.value_lowerbound, &tvu = abstimeexp.value_upperbound;
	const std::string target_time_position = normalizer_utility::identify_time_detail(abstimeexp.value_lowerbound);
	if (target_time_position == "month") {
			tvl.day = 21;
			tvu.day = 31;
	} 
} 

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
  } else if (process_type == "about") {
		do_time_about(abstimeexp);
	} else if (process_type == "zenhan") {
		do_time_zenhan(abstimeexp);
	} else if (process_type == "kouhan") {
		do_time_kouhan(abstimeexp);
	} else if (process_type == "nakaba") {
		do_time_nakaba(abstimeexp);
	} else if (process_type == "joujun") {
		do_time_joujun(abstimeexp);
	} else if (process_type == "tyujun") {
		do_time_tyujun(abstimeexp);
	} else if (process_type == "gejun") {
		do_time_gejun(abstimeexp);
	}	else {
    abstimeexp.options.push_back(process_type);
  }
}
  






/*
　その他の処理
*/

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


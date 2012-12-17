#include <string>
#include <math.h>
#include "duration_expression_normalizer.hpp"
#include "digit_utility.hpp"
#include "number_normalizer.hpp"

namespace duration_expression_normalizer {

void DurationExpressionNormalizer::init() {
	load_from_dictionaries("duration_expression_json.txt", "duration_prefix_counter_json.txt", "duration_prefix_json.txt", "duration_suffix_json.txt");
}

void DurationExpressionNormalizer::normalize_number(const std::string& text, std::vector<digit_utility::Number>& numbers) {
	NN.process(text, numbers);
}

void set_time(DurationExpression& durationexp, const std::string& corresponding_time_position, const DurationExpression& integrate_durationexp) {
	if (corresponding_time_position == "y") {
		durationexp.value_lowerbound.year = integrate_durationexp.org_value_lowerbound;
		durationexp.value_upperbound.year = integrate_durationexp.org_value_upperbound;
	} else if (corresponding_time_position == "m") {
		durationexp.value_lowerbound.month = integrate_durationexp.org_value_lowerbound;
		durationexp.value_upperbound.month = integrate_durationexp.org_value_upperbound;
	} else if (corresponding_time_position == "d") {
		durationexp.value_lowerbound.day = integrate_durationexp.org_value_lowerbound;
		durationexp.value_upperbound.day = integrate_durationexp.org_value_upperbound;
	} else if (corresponding_time_position == "h") {
		durationexp.value_lowerbound.hour = integrate_durationexp.org_value_lowerbound;
		durationexp.value_upperbound.hour = integrate_durationexp.org_value_upperbound;
	} else if (corresponding_time_position == "mn") {
		durationexp.value_lowerbound.minute = integrate_durationexp.org_value_lowerbound;
		durationexp.value_upperbound.minute = integrate_durationexp.org_value_upperbound;
	} else if (corresponding_time_position == "s") {
		durationexp.value_lowerbound.second = integrate_durationexp.org_value_lowerbound;
		durationexp.value_upperbound.second = integrate_durationexp.org_value_upperbound;
	} else if (corresponding_time_position == "seiki") {
		durationexp.value_lowerbound.year = integrate_durationexp.org_value_lowerbound*100;
		durationexp.value_upperbound.year = integrate_durationexp.org_value_upperbound*100;
	} else if (corresponding_time_position == "w") {
		durationexp.value_lowerbound.day = integrate_durationexp.org_value_lowerbound*7;
		durationexp.value_upperbound.day = integrate_durationexp.org_value_upperbound*7;
	}
}

void do_option_han(DurationExpression& durationexp, const std::string& corresponding_time_position){
	if (corresponding_time_position == "y") {
		durationexp.value_lowerbound.year += 0.5;
		durationexp.value_upperbound.year += 0.5;
	} else if (corresponding_time_position == "m") {
		durationexp.value_lowerbound.month += 0.5;
		durationexp.value_upperbound.month += 0.5;
	} else if (corresponding_time_position == "d") {
		durationexp.value_lowerbound.day += 0.5;
		durationexp.value_upperbound.day += 0.5;
	} else if (corresponding_time_position == "h") {
		durationexp.value_lowerbound.hour += 0.5;
		durationexp.value_upperbound.hour += 0.5;
	} else if (corresponding_time_position == "mn") {
		durationexp.value_lowerbound.minute += 0.5;
		durationexp.value_upperbound.minute += 0.5;
	} else if (corresponding_time_position == "s") {
		durationexp.value_lowerbound.second += 0.5;
		durationexp.value_upperbound.second += 0.5;
	} else if (corresponding_time_position == "seiki") {
		durationexp.value_lowerbound.year += 50;
		durationexp.value_upperbound.year += 50;
	}
}

void revise_durationexp_by_process_type(DurationExpression& durationexp, std::string process_type, const LimitedDurationExpression& matching_limited_duration_expression) {
	if (process_type == "han") {
		if(matching_limited_duration_expression.corresponding_time_position.empty()) return;
		std::string corresponding_time_position = matching_limited_duration_expression.corresponding_time_position[matching_limited_duration_expression.corresponding_time_position.size()-1];
		do_option_han(durationexp, corresponding_time_position);
	}
}

void DurationExpressionNormalizer::revise_any_type_expression_by_matching_limited_expression(std::vector<DurationExpression>& durationexps, int &expression_id,
																																														const LimitedDurationExpression matching_limited_duration_expression) {
	int final_integrated_durationexp_id = expression_id + matching_limited_duration_expression.total_number_of_place_holder;
	durationexps[expression_id].position_end = durationexps[final_integrated_durationexp_id].position_end
	+ matching_limited_duration_expression.length_of_strings_after_final_place_holder;
	for (int i = 0; i < static_cast<int>(matching_limited_duration_expression.corresponding_time_position.size()); i++) {
		set_time(durationexps[expression_id], matching_limited_duration_expression.corresponding_time_position[i], durationexps[expression_id + i]);
	}
	for (int i = 0; i < static_cast<int>(matching_limited_duration_expression.process_type.size()); i++) {
		revise_durationexp_by_process_type(durationexps[expression_id], matching_limited_duration_expression.process_type[i], matching_limited_duration_expression);
	}
	durationexps[expression_id].ordinary = matching_limited_duration_expression.ordinary;
	
	durationexps.erase(durationexps.begin() + expression_id + 1,
										durationexps.begin() + expression_id + 1 + matching_limited_duration_expression.total_number_of_place_holder);
}

void DurationExpressionNormalizer::revise_any_type_expression_by_matching_prefix_counter(DurationExpression& any_type_expression, const LimitedDurationExpression& matching_limited_expression) {} //持続時間にprefix_counterは存在しない（今のところ）















	



/*
 　修飾語による規格化表現の補正処理。
 */

void do_time_about(DurationExpression& durationexp) {
	normalizer_utility::Time &tvl = durationexp.value_lowerbound, &tvu = durationexp.value_upperbound;
	const std::string target_time_position = normalizer_utility::identify_time_detail(durationexp.value_lowerbound);
	if (target_time_position == "year") {
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


void do_time_kyou(DurationExpression& durationexp) {
	normalizer_utility::Time &tvl = durationexp.value_lowerbound, &tvu = durationexp.value_upperbound;
	const std::string target_time_position = normalizer_utility::identify_time_detail(durationexp.value_lowerbound);
	if (target_time_position == "year") {
		tvu.year += 5;
	} else if (target_time_position == "month") {
		tvu.month += 1;
	} else if (target_time_position == "day") {
		tvu.day += 1;
	} else if (target_time_position == "hour") {
		tvu.hour += 1;
	} else if (target_time_position == "minute") {
		tvu.minute += 5;
	} else if (target_time_position == "second") {
		tvu.second += 5;
	}
}


void do_time_jaku(DurationExpression& durationexp) {
	normalizer_utility::Time &tvl = durationexp.value_lowerbound, &tvu = durationexp.value_upperbound;
	const std::string target_time_position = normalizer_utility::identify_time_detail(durationexp.value_lowerbound);
	if (target_time_position == "year") {
		tvl.year -= 5;
	} else if (target_time_position == "month") {
		tvl.month -= 1;
	} else if (target_time_position == "day") {
		tvl.day -= 1;
	} else if (target_time_position == "hour") {
		tvl.hour -= 1;
	} else if (target_time_position == "minute") {
		tvl.minute -= 5;
	} else if (target_time_position == "second") {
		tvl.second -= 5;
	}
}	


void DurationExpressionNormalizer::revise_any_type_expression_by_number_modifier(DurationExpression& durationexp,
																																								const normalizer_utility::NumberModifier& number_modifier) {
	std::string process_type = number_modifier.process_type;
	if (process_type == "or_over") {
		durationexp.value_upperbound = normalizer_utility::Time(INFINITY);
	} else if (process_type == "or_less") {
		durationexp.value_lowerbound = normalizer_utility::Time(-INFINITY);
	} else if (process_type == "over") {
		durationexp.value_upperbound = normalizer_utility::Time(INFINITY);
		durationexp.include_lowerbound = false;
	} else if (process_type == "less") {
		durationexp.value_lowerbound = normalizer_utility::Time(-INFINITY);
		durationexp.include_upperbound = false;
	} else if (process_type == "ordinary") { //TODO : 序数は絶対時間として扱う？持続時間として扱う？　未定
		durationexp.ordinary = true;
	} else if (process_type == "none") {
		;
	} else if (process_type == "per") {
		// TODO : 「1日毎」など? どんな処理をするか未定。
	} else if (process_type == "dai") {
		// TODO : 「1秒台」など。　どんな処理をするか未定。　これは持続時間？（ではなさそう）
	} else if (process_type == "about") {
		do_time_about(durationexp);
	} else if (process_type == "kyou") {
		do_time_kyou(durationexp);
	} else if (process_type == "jaku") {
		do_time_jaku(durationexp);				
	} else {
		durationexp.options.push_back(process_type);
	}
	
}

void DurationExpressionNormalizer::delete_not_any_type_expression(std::vector<DurationExpression>& durationexps){
	for(int i=0; i<static_cast<int>(durationexps.size()); i++){
		if(normalizer_utility::is_null_time(durationexps[i].value_lowerbound) && normalizer_utility::is_null_time(durationexps[i].value_upperbound)){
			durationexps.erase(durationexps.begin() + i);
			i--;
		}
	}
}

void DurationExpressionNormalizer::fix_by_range_expression(const pfi::data::string::ustring& utext, std::vector<DurationExpression>& durationexps) {
	for(int i=0; i<static_cast<int>(durationexps.size()-1); i++){
		if(have_kara_suffix(durationexps[i].options) && have_kara_prefix(durationexps[i+1].options)){
			durationexps[i].value_upperbound = durationexps[i+1].value_upperbound;
			durationexps[i].position_end = durationexps[i+1].position_end;
			durationexps[i].set_original_expression_from_position(utext);
			merge_options(durationexps[i].options, durationexps[i+1].options);
			durationexps.erase(durationexps.begin()+i+1);
		}
	}
}
} //namespace duration_expression_normalizer


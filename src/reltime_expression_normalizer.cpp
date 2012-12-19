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
			//「去年3月」などの、「相対時間表現」＋「絶対時間表現」からなる処理
			if(normalizer_utility::is_null_time(reltimeexp.value_lowerbound_abs) && normalizer_utility::is_null_time(reltimeexp.value_upperbound_abs)) return; //絶対時間表現が抽出されていなければ、処理を行わない
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
	
	
	
	
	
	
	
	/*
	 　修飾語による規格化表現の補正処理。
	 	絶対表現と同じ処理を念のため書き加えたが、絶対表現と同じ修飾表現 + 相対時間表現は存在しないと考えられるので、恐らく必要ない。
	 */
	
	void do_time_about(ReltimeExpression& reltimeexp) {
		//「およそ1000年前」「2か月前頃」など
		normalizer_utility::Time &tvl = reltimeexp.value_lowerbound_rel, &tvu = reltimeexp.value_upperbound_rel;
		const std::string target_time_position = normalizer_utility::identify_time_detail(reltimeexp.value_lowerbound_rel);
		if (target_time_position == "y") {
			tvl.year -= 5;
			tvu.year += 5;
		} else if (target_time_position == "m") {
			tvl.month -= 1;
			tvu.month += 1;
		} else if (target_time_position == "d") {
			tvl.day -= 1;
			tvu.day += 1;
		} else if (target_time_position == "h") {
			tvl.hour -= 1;
			tvu.hour += 1;
		} else if (target_time_position == "mn") {
			tvl.minute -= 5;
			tvu.minute += 5;
		} else if (target_time_position == "s") {
			tvl.second -= 5;
			tvu.second += 5;
		}
	}
	
	//~~他の処理も同様
	void do_time_zenhan(ReltimeExpression& reltimeexp) {
		//「18世紀前半」「1989年前半」「7月前半」「3日朝」など。
		//TODO : 「18世紀はじめ」などもzenhanに括ってしまっている。より細かく分類が行いたい場合は、hajime関数などを書いて処理
		normalizer_utility::Time &tvl = reltimeexp.value_lowerbound_abs, &tvu = reltimeexp.value_upperbound_abs;
		const std::string target_time_position = normalizer_utility::identify_time_detail(reltimeexp.value_lowerbound_abs);
		if (target_time_position == "y") {
			if(tvl.year != tvu.year){
				//「18世紀前半」のとき
				tvu.year = (tvl.year + tvu.year)/2 -0.5;
			} else {
				//「1989年前半」のとき
				tvl.month = 1;
				tvu.month = 6;
			}
		} else if (target_time_position == "m") {
			//「7月前半」のとき
			tvl.day = 1;
			tvl.day = 15;
		} else if (target_time_position == "d") {
			//「3日朝」のとき
			tvl.hour = 5;
			tvu.hour = 12;
		} else {
			//これ以外でzenhanになる場合はない？　処理を行わない。
		}
	}
	
	
	void do_time_kouhan(ReltimeExpression& reltimeexp) {
		//「18世紀後半」「1989年後半」「7月後半」など。
		//TODO : 「18世紀末」「3日夜」などもkouhanに括ってしまっている。
		normalizer_utility::Time &tvl = reltimeexp.value_lowerbound_abs, &tvu = reltimeexp.value_upperbound_abs;
		const std::string target_time_position = normalizer_utility::identify_time_detail(reltimeexp.value_lowerbound_abs);
		if (target_time_position == "y") {
			if(tvl.year != tvu.year){
				//「18世紀後半」のとき
				tvl.year = (tvl.year + tvu.year)/2 +0.5;;
			} else {
				//「1989年後半」のとき
				tvl.month = 7;
				tvu.month = 12;
			}
		} else if (target_time_position == "m") {
			//「7月後半」のとき
			tvl.day = 16;
			tvl.day = 31;
		} else if (target_time_position == "d") {
			//「3日夜」のとき
			tvl.hour = 18;
			tvu.hour = 24;
		} else {
			//これ以外の場合はない？　処理を行わない。
		}
	}
	
	
	void do_time_nakaba(ReltimeExpression& reltimeexp) {
		//「18世紀中盤」「1989年中盤」「7月中盤」など。
		//TODO : 「18世紀なかば」「3日昼」などもnakabaに括ってしまっている。
		normalizer_utility::Time &tvl = reltimeexp.value_lowerbound_abs, &tvu = reltimeexp.value_upperbound_abs;
		const std::string target_time_position = normalizer_utility::identify_time_detail(reltimeexp.value_lowerbound_abs);
		if (target_time_position == "y") {
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
		} else if (target_time_position == "m") {
			//「7月半ば」のとき
			tvl.day = 10;
			tvl.day = 20;
		} else if (target_time_position == "d") {
			//「3日昼」のとき
			tvl.hour = 10;
			tvu.hour = 15;
		} else {
			//これ以外の場合はない？　処理を行わない。
		}
	}
	
	
	void do_time_joujun(ReltimeExpression& reltimeexp) {
		normalizer_utility::Time &tvl = reltimeexp.value_lowerbound_abs, &tvu = reltimeexp.value_upperbound_abs;
		const std::string target_time_position = normalizer_utility::identify_time_detail(reltimeexp.value_lowerbound_abs);
		if (target_time_position == "m") {
			tvl.day = 1;
			tvu.day = 10;
		} 
	}
	
	void do_time_tyujun(ReltimeExpression& reltimeexp) {
		normalizer_utility::Time &tvl = reltimeexp.value_lowerbound_abs, &tvu = reltimeexp.value_upperbound_abs;
		const std::string target_time_position = normalizer_utility::identify_time_detail(reltimeexp.value_lowerbound_abs);
		if (target_time_position == "m") {
			tvl.day = 11;
			tvu.day = 20;
		} 
	}
	
	void do_time_gejun(ReltimeExpression& reltimeexp) {
		normalizer_utility::Time &tvl = reltimeexp.value_lowerbound_abs, &tvu = reltimeexp.value_upperbound_abs;
		const std::string target_time_position = normalizer_utility::identify_time_detail(reltimeexp.value_lowerbound_abs);
		if (target_time_position == "m") {
			tvl.day = 21;
			tvu.day = 31;
		} 
	} 
  
  void ReltimeExpressionNormalizer::revise_any_type_expression_by_number_modifier(ReltimeExpression& reltimeexp,
                                                                                  const normalizer_utility::NumberModifier& number_modifier) {
    std::string process_type = number_modifier.process_type;
    //or_overなどのタイプは、reltimeではexpression_jsonの方で記述されており、これはrevise_reltimeexp_by_process_typeで処理されるので、ここでは処理しない。（TODO : 辞書からありえない表現をのぞく）
		if (process_type == "none") {
			;
		} else if (process_type == "about") {
			do_time_about(reltimeexp);
		} else if (process_type == "zenhan") {
			do_time_zenhan(reltimeexp);
		} else if (process_type == "kouhan") {
			do_time_kouhan(reltimeexp);
		} else if (process_type == "nakaba") {
			do_time_nakaba(reltimeexp);
		} else if (process_type == "joujun") {
			do_time_joujun(reltimeexp);
		} else if (process_type == "tyujun") {
			do_time_tyujun(reltimeexp);
		} else if (process_type == "gejun") {
			do_time_gejun(reltimeexp);
		}	else {
			reltimeexp.options.push_back(process_type);
		}
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
      if(have_kara_suffix(reltimeexps[i].options) && have_kara_prefix(reltimeexps[i+1].options) && reltimeexps[i].position_end +2 >= reltimeexps[i+1].position_start){
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


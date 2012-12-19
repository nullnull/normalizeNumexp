#include <math.h>
#include "numerical_expression_normalizer.hpp"
#include "digit_utility.hpp"
#include "number_normalizer.hpp"

namespace numerical_expression_normalizer{

void NumericalExpressionNormalizer::init(){
  load_from_dictionaries("num_counter_json.txt", "num_prefix_counter_json.txt", "num_prefix_json.txt", "num_suffix_json.txt");
}
  
void NumericalExpressionNormalizer::normalize_number(const std::string& text, std::vector<digit_utility::Number>& numbers) {
  NN.process(text, numbers);
}

void multiply_numexp_value(NumericalExpression& numexp, double x){
  numexp.value_lowerbound *= x;
  numexp.value_upperbound *= x;
}


void do_option_wari(std::vector<NumericalExpression>& numexps, int expression_id, const Counter matching_limited_expression){
	pfi::data::string::ustring upattern = pfi::data::string::string_to_ustring(matching_limited_expression.pattern);
  numexps[expression_id].position_end += upattern.size();
	numexps[expression_id].counter = pfi::data::string::string_to_ustring("%");
	numexps[expression_id].ordinary = false;
			
	//set_value
	double value = 0;
	for(int i=0; i<static_cast<int>(upattern.size()); i+=2){
		if(upattern[i] == pfi::data::string::string_to_ustring("割")[0]){
			value += numexps[expression_id + i/2].value_lowerbound * 10; 
		}else if(upattern[i] == pfi::data::string::string_to_ustring("分")[0]){
			value += numexps[expression_id + i/2].value_lowerbound * 1;
		}else if(upattern[i] == pfi::data::string::string_to_ustring("厘")[0]){
			value += numexps[expression_id + i/2].value_lowerbound * 0.1;
		}
	}
	numexps[expression_id].value_lowerbound = value;
	numexps[expression_id].value_upperbound = value;
	
	//erase merged numexps
	for(int i=2; i<static_cast<int>(upattern.size()); i+=2){
		numexps.erase(numexps.begin() + expression_id + 1);
	}
}

void NumericalExpressionNormalizer::revise_any_type_expression_by_matching_limited_expression(std::vector<NumericalExpression>& numexps, int& expression_id, const Counter matching_limited_expression){
  //特殊なタイプをここで例外処理
	if(matching_limited_expression.option == "wari"){
		do_option_wari(numexps, expression_id, matching_limited_expression);
		return;
	}
  //TODO : 今のところ特殊なタイプは分数しかないので、とりあえず保留

  numexps[expression_id].position_end += pfi::data::string::string_to_ustring(matching_limited_expression.pattern).size();
  numexps[expression_id].counter = pfi::data::string::string_to_ustring(matching_limited_expression.counter);
  multiply_numexp_value(numexps[expression_id], pow(10, matching_limited_expression.SI_prefix));
  multiply_numexp_value(numexps[expression_id], pow(10, matching_limited_expression.optional_power_of_ten));
  numexps[expression_id].ordinary = matching_limited_expression.ordinary;
}
  
void NumericalExpressionNormalizer::revise_any_type_expression_by_matching_prefix_counter(NumericalExpression& numexp, const Counter& matching_limited_expression){    
  if(matching_limited_expression.option == "counter"){
    numexp.position_start -= pfi::data::string::string_to_ustring(matching_limited_expression.pattern).size();
    numexp.counter = pfi::data::string::string_to_ustring(matching_limited_expression.counter);
    multiply_numexp_value(numexp, pow(10, matching_limited_expression.SI_prefix));
    multiply_numexp_value(numexp, pow(10, matching_limited_expression.optional_power_of_ten));
    numexp.ordinary = matching_limited_expression.ordinary;
  }else if(matching_limited_expression.option == "add_suffix_counter"){
    if(numexp.counter.empty()) return; //TODO : 単位が空の場合、追加は行わない？
    numexp.position_start -= pfi::data::string::string_to_ustring(matching_limited_expression.pattern).size();
    numexp.counter += pfi::data::string::string_to_ustring(matching_limited_expression.counter);
  }
}

void NumericalExpressionNormalizer::revise_any_type_expression_by_number_modifier(NumericalExpression& numexp, const normalizer_utility::NumberModifier& number_modifier){
  std::string process_type = number_modifier.process_type;
  /* 「約」などのNumberModifierの処理を行う。
   */
  if(process_type == "or_over"){
    numexp.value_upperbound = INFINITY;
  }else if(process_type == "or_less"){
    numexp.value_lowerbound = -INFINITY;
  }else if(process_type == "over"){
    numexp.value_upperbound = INFINITY;
    numexp.include_lowerbound = false;
  }else if(process_type == "less"){
    numexp.value_lowerbound = -INFINITY;
    numexp.include_upperbound = false;
  }else if(process_type == "dai"){
    //TODO : どんな処理をするか未定。。　該当する事例は「30代」「9秒台」のみ？
  }else if(process_type == "ordinary"){
    numexp.ordinary = true;
  }else if(process_type == "han"){
    numexp.value_lowerbound += 0.5;
    numexp.value_upperbound += 0.5;
  }else if(process_type[0] == '/'){  // /hour, /minなど
    numexp.counter += pfi::data::string::string_to_ustring(process_type);
  }else if(process_type == "none"){
    ;
  }else if(process_type == "per"){
    // TODO : どんな処理をするか未定。　該当する事例は「1ページ毎」など。
  }else if(process_type == "about"){
    numexp.value_lowerbound *= 0.7;
    numexp.value_upperbound *= 1.3;
	}else if(process_type == "kyou"){
    numexp.value_upperbound *= 1.6;
  }else if(process_type == "jaku"){
    numexp.value_lowerbound *= 0.5;
	}else if(process_type == "made"){
		if(numexp.value_lowerbound == numexp.value_upperbound) {
			numexp.value_lowerbound = -INFINITY;
		}else{
			;
		}
	}else {
    numexp.options.push_back(process_type);
	}
}
  
void NumericalExpressionNormalizer::delete_not_any_type_expression(std::vector<NumericalExpression>& numexps){
  for(int i=0; i<static_cast<int>(numexps.size()); i++){
    if(numexps[i].counter.empty()){
      numexps.erase(numexps.begin() + i);
      i--;
    }
  }
}
  
void delete_after_slash(pfi::data::string::ustring& ustr){
  if(ustr.find(pfi::data::string::string_to_uchar("/")) == pfi::data::string::ustring::npos) return;
  ustr = ustr.substr(0, ustr.find(pfi::data::string::string_to_uchar("/")));
}

bool suffix_match_counter(pfi::data::string::ustring counter1, pfi::data::string::ustring counter2){
  //単位が一致しているかどうかを判断する。
  //「時速50km〜60km」のような事例に対応する（前者は[50km/h], 後者は[60km]と規格化されており、完全一致ではマッチしない）ために、スラッシュより前の単位が一致するかどうかで判断する
  delete_after_slash(counter1);
  delete_after_slash(counter2);
  return counter1 == counter2;
}

void NumericalExpressionNormalizer::fix_by_range_expression(const pfi::data::string::ustring& utext, std::vector<NumericalExpression>& numexps) {
  for(int i=0; i<static_cast<int>(numexps.size()-1); i++){
    if(have_kara_suffix(numexps[i].options) && have_kara_prefix(numexps[i+1].options) && numexps[i].position_end +2 >= numexps[i+1].position_start){
      if(!suffix_match_counter(numexps[i].counter, numexps[i+1].counter)) continue;
      numexps[i].value_upperbound = numexps[i+1].value_upperbound;
      numexps[i].position_end = numexps[i+1].position_end;
      numexps[i].set_original_expression_from_position(utext);
      //memo :単位のマージは、必ずiの方がi+1よりも長いので、する必要なし
      merge_options(numexps[i].options, numexps[i+1].options);
      numexps.erase(numexps.begin()+i+1);
    }
  }
}
  
} //namespace numerical_expression_normalizer



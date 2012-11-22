#include "numerical_expression_extractor.hpp"
#include <sstream>
namespace numerical_expression_extractor{
    
	void NumericalExpressionExtractor::normalize_each_type_expressions(const std::string& text,
																	   std::vector<numerical_expression_normalizer::NumericalExpression>& numexps,
																	   std::vector<abstime_expression_normalizer::AbstimeExpression>& abstimeexps,
																	   std::vector<reltime_expression_normalizer::ReltimeExpression>& reltimeexps,
																	   std::vector<duration_expression_normalizer::DurationExpression>& durationexps){
		NEN.process(text, numexps);
		AEN.process(text, abstimeexps);
		REN.process(text, reltimeexps);
		DEN.process(text, durationexps);
	}
	
	template <class AnyTypeExpression1, class AnyTypeExpression2>
	bool is_covered_by_other_type_expression(const AnyTypeExpression1& any_type_expression1, const AnyTypeExpression2& any_type_expression2){
		return any_type_expression2.position_start <= any_type_expression1.position_start && any_type_expression1.position_end <= any_type_expression2.position_end;
	}
	
	template <class AnyTypeExpression1, class AnyTypeExpression2>
	bool is_covered_by_other_type_expressions(const AnyTypeExpression1& any_type_expression1, const std::vector<AnyTypeExpression2>& any_type_expressions2){
		for(int i=0; i<static_cast<int>(any_type_expressions2.size()); i++){
			if(is_covered_by_other_type_expression(any_type_expression1, any_type_expressions2[i])) return true;
		}
		return false;
	}
	
	void delete_inappropriate_extraction(std::vector<numerical_expression_normalizer::NumericalExpression>& numexps,
										 std::vector<abstime_expression_normalizer::AbstimeExpression>& abstimeexps,
										 std::vector<reltime_expression_normalizer::ReltimeExpression>& reltimeexps,
										 std::vector<duration_expression_normalizer::DurationExpression>& durationexps){
		//TODO : O(N^2)のアルゴリズム。対象となる表現と、その他すべての表現に対して重複をチェックしている。必要に応じて高速化する
		//erase duration
		for(int i=0; i<static_cast<int>(durationexps.size()); i++){
			if(is_covered_by_other_type_expressions(durationexps[0], abstimeexps) || is_covered_by_other_type_expressions(durationexps[0], reltimeexps) || is_covered_by_other_type_expressions(durationexps[0], numexps)){
				durationexps.erase(durationexps.begin() + i);
				i--;
			}
		}
		
		//erase reltime
		for(int i=0; i<static_cast<int>(reltimeexps.size()); i++){
			if(is_covered_by_other_type_expressions(reltimeexps[0], abstimeexps) || is_covered_by_other_type_expressions(reltimeexps[0], numexps) || is_covered_by_other_type_expressions(reltimeexps[0], durationexps)){
				reltimeexps.erase(reltimeexps.begin() + i);
				i--;
			}
		}
		
		//erase numexp
		for(int i=0; i<static_cast<int>(numexps.size()); i++){
			if(is_covered_by_other_type_expressions(numexps[0], abstimeexps) || is_covered_by_other_type_expressions(numexps[0], reltimeexps) || is_covered_by_other_type_expressions(numexps[0], durationexps)){
				numexps.erase(numexps.begin() + i);
				i--;
			}
		}
		
		//erase abstime
		for(int i=0; i<static_cast<int>(abstimeexps.size()); i++){
			if(is_covered_by_other_type_expressions(abstimeexps[0], numexps) || is_covered_by_other_type_expressions(abstimeexps[0], reltimeexps) || is_covered_by_other_type_expressions(abstimeexps[0], durationexps)){
				abstimeexps.erase(abstimeexps.begin() + i);
				i--;
			}
		}
	}
	
	
	 void merge_normalize_expressions_into_result(const std::vector<numerical_expression_normalizer::NumericalExpression> numexps, const std::vector<abstime_expression_normalizer::AbstimeExpression> abstimeexps, const std::vector<reltime_expression_normalizer::ReltimeExpression> reltimeexps, const std::vector<duration_expression_normalizer::DurationExpression> durationexps, std::vector<std::string>& result){
	 //TODO : それぞれの正規形に、toString関数をつける？逆に分かり辛い？　とりあえずここで処理
	 std::string kugiri("*");
	 std::string tmpstr;
	 std::stringstream ss;
	 result.clear();
	 
	 for(int i=0; i<static_cast<int>(numexps.size()); i++){
	 ss.clear(); ss.str("");
	 ss << "numerical" << "*" << numexps[i].original_expression << "*" << numexps[i].position_start << "*" << numexps[i].position_end << "*" << numexps[i].counter << "*" << numexps[i].value_lowerbound;
	 ss >> tmpstr;
	 result.push_back(tmpstr);
	 }
	 
	 for(int i=0; i<static_cast<int>(abstimeexps.size()); i++){
	 ss.clear(); ss.str("");
	 ss << "abstime" << "*" << abstimeexps[i].original_expression << "*" << abstimeexps[i].position_start << "*" << abstimeexps[i].position_end << "*";
	 ss >> tmpstr;
	 result.push_back(tmpstr);
	 }
	 
	 for(int i=0; i<static_cast<int>(reltimeexps.size()); i++){
	 ss.clear(); ss.str("");
	 ss << "reltime" << "*" << reltimeexps[i].original_expression << "*" << reltimeexps[i].position_start << "*" << reltimeexps[i].position_end << "*";
	 ss >> tmpstr;
	 result.push_back(tmpstr);
	 }
	 
	 for(int i=0; i<static_cast<int>(durationexps.size()); i++){
	 ss.clear(); ss.str("");
	 ss << "duration" << "*" << durationexps[i].original_expression << "*" << durationexps[i].position_start << "*" << durationexps[i].position_end << "*";
	 ss >> tmpstr;
	 result.push_back(tmpstr);
	 }
	 }
	 
	
	NumericalExpressionExtractor::NumericalExpressionExtractor(const std::string& language) : NEN(language), AEN(language), REN(language), DEN(language) {}
	
	void NumericalExpressionExtractor::extract_numerical_expression(const std::string& text, std::vector<std::string>& result){
		result.clear(); //TODO : 結果をどのような形で返すかはScrewの実装に合わせて。とりあえず、original_expressionのみを格納したvectorを返す。
		std::vector<numerical_expression_normalizer::NumericalExpression> numexps;
		std::vector<abstime_expression_normalizer::AbstimeExpression> abstimeexps;
		std::vector<reltime_expression_normalizer::ReltimeExpression> reltimeexps;
		std::vector<duration_expression_normalizer::DurationExpression> durationexps;
		//4つにかける
		normalize_each_type_expressions(text, numexps, abstimeexps, reltimeexps, durationexps);
		
		//それぞれの結果より、不適当な抽出を削除
		delete_inappropriate_extraction(numexps, abstimeexps, reltimeexps, durationexps);
		
		//string型にして、resultにまとめる
		merge_normalize_expressions_into_result(numexps, abstimeexps, reltimeexps, durationexps, result);
	}
	
} //namespace numerical_expression_extractor




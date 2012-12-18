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
	
	
	template <class AnyTypeExpression>
	std::string show_options(AnyTypeExpression& any_type_expression){
		std::stringstream ss;
		if(any_type_expression.ordinary) any_type_expression.options.push_back("ordinary");
		int sz = static_cast<int>(any_type_expression.options.size());
		for(int i=0; i<sz; i++){
			ss << any_type_expression.options[i];
			if(i!=sz-1) ss << ",";
		}
		std::string ret;
		ss >> ret;
		return ret;
	}
	
	
	
	//resultの生成
	void merge_normalize_expressions_into_result( std::vector<numerical_expression_normalizer::NumericalExpression> numexps,  std::vector<abstime_expression_normalizer::AbstimeExpression> abstimeexps,  std::vector<reltime_expression_normalizer::ReltimeExpression> reltimeexps,  std::vector<duration_expression_normalizer::DurationExpression> durationexps, std::vector<std::string>& result){

	 //TODO : それぞれの正規形に、toString関数をつける？逆に分かり辛い？　とりあえずここで処理
	 std::string kugiri("*");
	 std::string tmpstr;
	 std::stringstream ss;
	 result.clear();
	 
	 for(int i=0; i<static_cast<int>(numexps.size()); i++){
	 ss.clear(); ss.str("");
	 ss << "numerical" << "*" << numexps[i].original_expression << "*" << numexps[i].position_start << "*" << numexps[i].position_end << "*" << numexps[i].counter << "*" << numexps[i].value_lowerbound << "*" << numexps[i].value_upperbound << "*" << show_options(numexps[i]);
	 ss >> tmpstr;
	 result.push_back(tmpstr);
	 }
	 
	 for(int i=0; i<static_cast<int>(abstimeexps.size()); i++){
	 ss.clear(); ss.str("");
	 ss << "abstime" << "*" << abstimeexps[i].original_expression << "*" << abstimeexps[i].position_start << "*" << abstimeexps[i].position_end << "*" << "none" << "*" << abstimeexps[i].value_lowerbound.to_string(false) << "*" << abstimeexps[i].value_upperbound.to_string(true) << "*" << show_options(abstimeexps[i]);
	 ss >> tmpstr;
	 result.push_back(tmpstr);
	 }
	 
	 for(int i=0; i<static_cast<int>(reltimeexps.size()); i++){
	 ss.clear(); ss.str("");
	 //TODO : 相対時間表現を、どう表示させるか？
	 ss << "reltime" << "*" << reltimeexps[i].original_expression << "*" << reltimeexps[i].position_start << "*" << reltimeexps[i].position_end << "*" << "none" << "*" << reltimeexps[i].value_lowerbound_abs.to_string(false) << "," << reltimeexps[i].value_lowerbound_rel.to_duration_string(false) << "*" << reltimeexps[i].value_upperbound_abs.to_string(true) << "," << reltimeexps[i].value_upperbound_rel.to_duration_string(true) << "*" << show_options(reltimeexps[i]);
	 ss >> tmpstr;
	 result.push_back(tmpstr);
	 }
	 
	 for(int i=0; i<static_cast<int>(durationexps.size()); i++){
	 ss.clear(); ss.str("");
	 ss << "duration" << "*" << durationexps[i].original_expression << "*" << durationexps[i].position_start << "*" << durationexps[i].position_end << "*" << "none" << "*" << durationexps[i].value_lowerbound.to_duration_string(false) << "*" << durationexps[i].value_upperbound.to_duration_string(true) << "*" << show_options(durationexps[i]);
	 ss >> tmpstr;
	 result.push_back(tmpstr);
	 }
	 }


	
	NumericalExpressionExtractor::NumericalExpressionExtractor(const std::string& language) : NEN(language), AEN(language), REN(language), DEN(language), IER(language) {}


	void NumericalExpressionExtractor::extract_numerical_expression(const std::string& text, std::vector<std::string>& result){
		result.clear();
		std::vector<numerical_expression_normalizer::NumericalExpression> numexps;
		std::vector<abstime_expression_normalizer::AbstimeExpression> abstimeexps;
		std::vector<reltime_expression_normalizer::ReltimeExpression> reltimeexps;
		std::vector<duration_expression_normalizer::DurationExpression> durationexps;
		
		//4つのnormalizerで処理を行う
		normalize_each_type_expressions(text, numexps, abstimeexps, reltimeexps, durationexps);
		
		//それぞれの結果より、不適当な抽出を削除
		IER.remove_inappropriate_extraction(text, numexps, abstimeexps, reltimeexps, durationexps);
		
		//string型に変換し、resultにまとめる
		merge_normalize_expressions_into_result(numexps, abstimeexps, reltimeexps, durationexps, result);
	}
	
} //namespace numerical_expression_extractor




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
	
	
	void delete_duplicate_extraction(std::vector<numerical_expression_normalizer::NumericalExpression>& numexps,
																	 std::vector<abstime_expression_normalizer::AbstimeExpression>& abstimeexps,
																	 std::vector<reltime_expression_normalizer::ReltimeExpression>& reltimeexps,
																	 std::vector<duration_expression_normalizer::DurationExpression>& durationexps){
		//TODO : O(N^2)のアルゴリズム。対象となる表現と、その他すべての表現に対して重複をチェックしている。必要に応じて高速化する
		//erase duration
		for(int i=0; i<static_cast<int>(durationexps.size()); i++){
			if(is_covered_by_other_type_expressions(durationexps[i], abstimeexps) || is_covered_by_other_type_expressions(durationexps[i], reltimeexps) || is_covered_by_other_type_expressions(durationexps[i], numexps)){
				durationexps.erase(durationexps.begin() + i);
				i--;
			}
		}
		
		//erase reltime
		for(int i=0; i<static_cast<int>(reltimeexps.size()); i++){
			if(is_covered_by_other_type_expressions(reltimeexps[i], abstimeexps) || is_covered_by_other_type_expressions(reltimeexps[i], numexps) || is_covered_by_other_type_expressions(reltimeexps[i], durationexps)){
				reltimeexps.erase(reltimeexps.begin() + i);
				i--;
			}
		}
		
		//erase numexp
		for(int i=0; i<static_cast<int>(numexps.size()); i++){
			if(is_covered_by_other_type_expressions(numexps[i], abstimeexps) || is_covered_by_other_type_expressions(numexps[i], reltimeexps) || is_covered_by_other_type_expressions(numexps[i], durationexps)){
				numexps.erase(numexps.begin() + i);
				i--;
			}
		}
		
		//erase abstime
		for(int i=0; i<static_cast<int>(abstimeexps.size()); i++){
			if(is_covered_by_other_type_expressions(abstimeexps[i], numexps) || is_covered_by_other_type_expressions(abstimeexps[i], reltimeexps) || is_covered_by_other_type_expressions(abstimeexps[i], durationexps)){
				abstimeexps.erase(abstimeexps.begin() + i);
				i--;
			}
		}															 
	}
	
	
	
	
	template <class AnyTypeExpression>
	void NumericalExpressionExtractor::delete_inappropriate_extraction_using_dictionary_one_type(std::vector<AnyTypeExpression>& any_type_expressions){
		for(int i=0; i<static_cast<int>(any_type_expressions.size()); i++){
			if(inappropriate_strings_to_bool[pfi::data::string::ustring_to_string(any_type_expressions[i].original_expression)]){
				any_type_expressions.erase(any_type_expressions.begin() + i);
				i--;
			}
		}	
		return;
	}
	
	void NumericalExpressionExtractor::delete_inappropriate_extraction_using_dictionary(std::vector<numerical_expression_normalizer::NumericalExpression>& numexps,
																			 std::vector<abstime_expression_normalizer::AbstimeExpression>& abstimeexps,
																			 std::vector<reltime_expression_normalizer::ReltimeExpression>& reltimeexps,
																			 std::vector<duration_expression_normalizer::DurationExpression>& durationexps){
		delete_inappropriate_extraction_using_dictionary_one_type(numexps);
		delete_inappropriate_extraction_using_dictionary_one_type(abstimeexps);
		delete_inappropriate_extraction_using_dictionary_one_type(reltimeexps);
		delete_inappropriate_extraction_using_dictionary_one_type(durationexps);					
	}
	
	
	void NumericalExpressionExtractor::delete_inappropriate_extraction(std::vector<numerical_expression_normalizer::NumericalExpression>& numexps,
										 std::vector<abstime_expression_normalizer::AbstimeExpression>& abstimeexps,
										 std::vector<reltime_expression_normalizer::ReltimeExpression>& reltimeexps,
										 std::vector<duration_expression_normalizer::DurationExpression>& durationexps){
		delete_duplicate_extraction(numexps, abstimeexps, reltimeexps, durationexps);
		delete_inappropriate_extraction_using_dictionary(numexps, abstimeexps, reltimeexps, durationexps);
	}
	
	
	void merge_normalize_expressions_into_result( std::vector<numerical_expression_normalizer::NumericalExpression> numexps,  std::vector<abstime_expression_normalizer::AbstimeExpression> abstimeexps,  std::vector<reltime_expression_normalizer::ReltimeExpression> reltimeexps,  std::vector<duration_expression_normalizer::DurationExpression> durationexps, std::vector<std::string>& result){

	 //TODO : それぞれの正規形に、toString関数をつける？逆に分かり辛い？　とりあえずここで処理
	 std::string kugiri("*");
	 std::string tmpstr;
	 std::stringstream ss;
	 result.clear();
	 
	 for(int i=0; i<static_cast<int>(numexps.size()); i++){
	 ss.clear(); ss.str("");
	 ss << "numerical" << "*" << numexps[i].original_expression << "*" << numexps[i].position_start << "*" << numexps[i].position_end << "*" << numexps[i].counter << "*" << numexps[i].value_lowerbound << "*" << numexps[i].value_upperbound;
	 ss >> tmpstr;
	 result.push_back(tmpstr);
	 }
	 
	 for(int i=0; i<static_cast<int>(abstimeexps.size()); i++){
	 ss.clear(); ss.str("");
	 ss << "abstime" << "*" << abstimeexps[i].original_expression << "*" << abstimeexps[i].position_start << "*" << abstimeexps[i].position_end << "*" << "none" << "*" << abstimeexps[i].value_lowerbound.to_string(false) << "*" << abstimeexps[i].value_upperbound.to_string(true);
	 ss >> tmpstr;
	 result.push_back(tmpstr);
	 }
	 
	 for(int i=0; i<static_cast<int>(reltimeexps.size()); i++){
	 ss.clear(); ss.str("");
	 //TODO : 相対時間表現を、どう表示させるか？
	 ss << "reltime" << "*" << reltimeexps[i].original_expression << "*" << reltimeexps[i].position_start << "*" << reltimeexps[i].position_end << "*" << "none" << "*" << reltimeexps[i].value_lowerbound_abs.to_string(false) << "," << reltimeexps[i].value_lowerbound_rel.to_string(false) << "*" << reltimeexps[i].value_upperbound_abs.to_string(true) << "," << reltimeexps[i].value_upperbound_rel.to_string(true);
	 ss >> tmpstr;
	 result.push_back(tmpstr);
	 }
	 
	 for(int i=0; i<static_cast<int>(durationexps.size()); i++){
	 ss.clear(); ss.str("");
	 ss << "duration" << "*" << durationexps[i].original_expression << "*" << durationexps[i].position_start << "*" << durationexps[i].position_end << "*" << "none" << "*" << durationexps[i].value_lowerbound.to_string(false) << "*" << durationexps[i].value_upperbound.to_string(true);
	 ss >> tmpstr;
	 result.push_back(tmpstr);
	 }
	 }
	 
	
	//TODO : digit_utilityのをそのまま使いたかったが、うまく読み込んでくれないので直コピー
	void load_json_from_file(const std::string& filepath, pfi::text::json::json& js) {
		std::ifstream in(filepath.c_str());
		pfi::text::json::json_parser parser(in);
		try {
			while (true) {
				js.add(parser.parse());
			}
		} catch (const pfi::lang::end_of_data&) {
		}
	}
	
	template <class T>
	void load_from_dictionary(const std::string& dictionary_path, std::vector<T>& load_target) {
		load_target.clear();
		pfi::text::json::json js = pfi::text::json::json(new pfi::text::json::json_array());
		load_json_from_file(dictionary_path, js);
		pfi::text::json::from_json(js, load_target);
	}

	
	void NumericalExpressionExtractor::init_inappropriate_stringss(const std::string& language){
		std::vector<InappropriateStrings> inappropriate_stringss;
		std::string dictionary_path;
		dictionary_path += dictionary_dirpath::get_dictionary_dirpath();
    dictionary_path += language;
		dictionary_path += "/inappropriate_strings_json.txt";
		load_from_dictionary(dictionary_path, inappropriate_stringss); 
		
		for(int i=0; i<static_cast<int>(inappropriate_stringss.size()); i++){
			inappropriate_strings_to_bool[inappropriate_stringss[i].str] = true;
		}
	}
	
	
	NumericalExpressionExtractor::NumericalExpressionExtractor(const std::string& language) : NEN(language), AEN(language), REN(language), DEN(language) {
		std::string dictionary_path;
		init_inappropriate_stringss(language);
	}
	
	void NumericalExpressionExtractor::extract_numerical_expression(const std::string& text, std::vector<std::string>& result){
		result.clear();
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




#include "inappropriate_expression_remover.hpp"
#include <sstream>
namespace inappropriate_expression_remover{
	/*
	重複を削除する。
	*/
	
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
		//表現タイプ毎に重複があるので、これを削除する（例：「300年間」はabstimeexpsでも「300年」として規格化されている）
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







	
	
	/*
	不適切な表現を削除する。
	TODO : とりあえず実装しただけ。局所的な手がかりしか用いない抽出による、不適切な表現を（しっかり）削除していくのは、今後の課題。
	*/
	
	//辞書に記述した表現の削除
	template <class AnyTypeExpression>
	void InappropriateExpressionRemover::delete_inappropriate_extraction_using_dictionary_one_type(std::vector<AnyTypeExpression>& any_type_expressions){
		for(int i=0; i<static_cast<int>(any_type_expressions.size()); i++){
			if(inappropriate_strings_to_bool[pfi::data::string::ustring_to_string(any_type_expressions[i].original_expression)]){
				any_type_expressions.erase(any_type_expressions.begin() + i);
				i--;
			}
		}	
		return;
	}
	
	
	//文字列の前後を参照して表現を削除
	bool is_suffix_match(const pfi::data::string::ustring& text, int position_start, const pfi::data::string::ustring& targ){
		int sz = targ.size();
		for(int i=0; i<sz; i++){
			if(position_start-1-i < 0) return false;
			if(text[position_start-1-i] != targ[sz-1-i]) return false;
		}
		return true;
	} 
	
	template <class AnyTypeExpression>
	bool is_inappropriate_prefix(const std::string& text, const AnyTypeExpression& any_type_expression){
		//TODO : 辞書知識として分離してない & 実装が超雑。
		//現在はverのみを対象。
		std::vector<pfi::data::string::ustring> targs;
		targs.push_back(pfi::data::string::string_to_ustring("ver"));
		targs.push_back(pfi::data::string::string_to_ustring("ｖｅｒ"));
		
		for(int i=0; i<static_cast<int>(targs.size()); i++){
			if(is_suffix_match(pfi::data::string::string_to_ustring(text), any_type_expression.position_start, targs[i]))	return true;
		}
		return false;
	}
	
	template <class AnyTypeExpression>
	void delete_inappropriate_extraction_by_prefix(const std::string& text, std::vector<AnyTypeExpression>& any_type_expressions){
		//文字列全体でなく、特定のprefix（文字列のsuffix）を持つものを削除する
		for(int i=0; i<static_cast<int>(any_type_expressions.size()); i++){
			if(is_inappropriate_prefix(text, any_type_expressions[i])){
				any_type_expressions.erase(any_type_expressions.begin() + i);
				i--;
			}
		}	
		return;
	}
	
	//URLっぽいものを削除
	template <class AnyTypeExpression>
	bool InappropriateExpressionRemover::is_url_strings(const std::string& text, const AnyTypeExpression& any_type_expression){
		//数量表現・時間表現の前後がurlに含まれる文字、かつ自分自身もurlに含まれる文字であればtrue
		pfi::data::string::ustring utext = pfi::data::string::string_to_ustring(text);
		pfi::data::string::uchar a,b;
		if(any_type_expression.position_start	>= 1) a = utext[any_type_expression.position_start-1];
		if(any_type_expression.position_end	< static_cast<int>(utext.size())) b = utext[any_type_expression.position_end];
		if(url_strings_to_bool[pfi::data::string::uchar_to_string(a)[0]] && url_strings_to_bool[pfi::data::string::uchar_to_string(b)[0]]){
			for(int i=0; i<static_cast<int>(any_type_expression.original_expression.size()); i++){
				if(not url_strings_to_bool[pfi::data::string::uchar_to_string(any_type_expression.original_expression[i])[0]]) return false;
			}
			return true;
		}
		return false;
	}
	
	
	template <class AnyTypeExpression>
	void InappropriateExpressionRemover::delete_url_strings(const std::string& text, std::vector<AnyTypeExpression>& any_type_expressions){
		//url、その他英字記号の羅列と思われる文字列中の表現は、削除する　　　例：「http://www.iphone3g.com」など（3gで抽出してしまう）
		for(int i=0; i<static_cast<int>(any_type_expressions.size()); i++){
			if(is_url_strings(text, any_type_expressions[i])){
				any_type_expressions.erase(any_type_expressions.begin() + i);
				i--;
			}
		}	
		return;
	}
		
	
	void InappropriateExpressionRemover::delete_inappropriate_extraction_using_dictionary(const std::string& text,
																			 std::vector<numerical_expression_normalizer::NumericalExpression>& numexps,
																			 std::vector<abstime_expression_normalizer::AbstimeExpression>& abstimeexps,
																			 std::vector<reltime_expression_normalizer::ReltimeExpression>& reltimeexps,
																			 std::vector<duration_expression_normalizer::DurationExpression>& durationexps){
		delete_inappropriate_extraction_using_dictionary_one_type(numexps);
		delete_inappropriate_extraction_using_dictionary_one_type(abstimeexps);
		delete_inappropriate_extraction_using_dictionary_one_type(reltimeexps);
		delete_inappropriate_extraction_using_dictionary_one_type(durationexps);
		
		delete_inappropriate_extraction_by_prefix(text, numexps);
		delete_inappropriate_extraction_by_prefix(text, abstimeexps);
		delete_inappropriate_extraction_by_prefix(text, reltimeexps);
		delete_inappropriate_extraction_by_prefix(text, durationexps);
		
		delete_url_strings(text, numexps);
		delete_url_strings(text, abstimeexps);
		delete_url_strings(text, reltimeexps);
		delete_url_strings(text, durationexps);
	}
	
	
	
	
	
	
	
	/*
	 不適切な時間表現の削除
	 */
	
	void revise_year_do_revise(double& year){
		if(20 < year && year < 100){
			year += 1900;
		}else if(0 <= year && year <= 20){
			year += 2000;
		}
	}
	
	bool is_period_etc(pfi::data::string::uchar uc){
		//TODO : とりあえず実装。。。
		//TODO : digit_utilityとの依存性の問題が解決できず、こんな事態に。。
		return uc == pfi::data::string::string_to_uchar(".") || uc == pfi::data::string::string_to_uchar("・") || uc == pfi::data::string::string_to_uchar("．") || uc == pfi::data::string::string_to_uchar("-") || uc == pfi::data::string::string_to_uchar("−") || uc == pfi::data::string::string_to_uchar("ー") || uc == pfi::data::string::string_to_uchar("―");
	}
	
	void revise_year(abstime_expression_normalizer::AbstimeExpression& abstimeexp){
		//「98年7月21日」などの表記のとき（20 < year < 100のとき）は、1900を加算する
		//「01年7月21日」などの表記のとき（0<= year <= 20のとき）は、2000を加算する
		//「西暦」とついてたら処理を行わない
		if(abstimeexp.original_expression[0] ==  pfi::data::string::string_to_uchar("西")) return;
		revise_year_do_revise(abstimeexp.value_lowerbound.year);
		revise_year_do_revise(abstimeexp.value_upperbound.year);
	}
	
	bool is_out_of_range(double x, double a, double b){
		if(x==INFINITY || x==-INFINITY) return false;
		return (x < a || b < x);
	}
	
	bool is_inappropriate_time_value(normalizer_utility::Time t){
		return (is_out_of_range(t.year, 1, 3000) || is_out_of_range(t.month, 1, 12) || is_out_of_range(t.day, 1, 31) || is_out_of_range(t.hour, 0, 30) || is_out_of_range(t.minute, 0, 59) || is_out_of_range(t.second, 0, 59));
	}
	
	bool is_inappropriate_abstimeexp(abstime_expression_normalizer::AbstimeExpression& abstimeexp){
		//「1.2.3」のような表現かどうか
		if(abstimeexp.original_expression.size() > 1 && is_period_etc(abstimeexp.original_expression[1])) return true;
		
		//時間の範囲が明らかにおかしいかどうか
		return (is_inappropriate_time_value(abstimeexp.value_lowerbound) || is_inappropriate_time_value(abstimeexp.value_upperbound));
		
	}
	
	void revise_or_delete_abstimeexp(std::vector<abstime_expression_normalizer::AbstimeExpression>& abstimeexps, int& i){
		revise_year(abstimeexps[i]);
		if(is_inappropriate_abstimeexp(abstimeexps[i])){
			abstimeexps.erase(abstimeexps.begin() + i);
			i--;
		}
	}
	
	void delete_inappropriate_abstimeexps(std::vector<abstime_expression_normalizer::AbstimeExpression>& abstimeexps){
		for(int i=0; i<static_cast<int>(abstimeexps.size()); i++){
			revise_or_delete_abstimeexp(abstimeexps, i);
		}
	}
	
	
	
	
	
	
	//不適切な表現の削除
	void InappropriateExpressionRemover::remove_inappropriate_extraction(const std::string& text,
																																		 std::vector<numerical_expression_normalizer::NumericalExpression>& numexps,
																																		 std::vector<abstime_expression_normalizer::AbstimeExpression>& abstimeexps,
																																		 std::vector<reltime_expression_normalizer::ReltimeExpression>& reltimeexps,
																																		 std::vector<duration_expression_normalizer::DurationExpression>& durationexps){
		//TODO : この部分を、コンポーネントとして書き出す。辞書で指定できるようにする。　規格化処理は辞書で指定できるが、不適切な表現の処理は辞書でできるようにほとんどなっていない。
		delete_duplicate_extraction(numexps, abstimeexps, reltimeexps, durationexps);
		delete_inappropriate_extraction_using_dictionary(text, numexps, abstimeexps, reltimeexps, durationexps);
		delete_inappropriate_abstimeexps(abstimeexps);
	}
	
	
		 
	 
	 
		 
	/* 
	 初期化処理
	*/
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

	void InappropriateExpressionRemover::init_inappropriate_stringss(const std::string& language){
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
	
	void InappropriateExpressionRemover::init_url_strings(){
		std::string url_strings("1234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ-!#$%&()=~^|¥@`/１２３４５６７８９０ａｂｃｄｅｆｇｈｉｊｋｌｍｎｏｐｑｒｓｔｕｖｗｘｙｚＡＢＣＤＥＦＧＨＩＪＫＭＮＬＰＱＲＳＴＵＶＷＸＹＺ！＃＄％＆（）＝〜｜");
			
		for(int i=0; i<static_cast<int>(url_strings.size()); i++){
			url_strings_to_bool[url_strings[i]] = true;
		}
	}


	
	InappropriateExpressionRemover::InappropriateExpressionRemover(const std::string& language) {
		init_inappropriate_stringss(language);
		init_url_strings();
	}
			
} //namespace inappropriate_expression_remover




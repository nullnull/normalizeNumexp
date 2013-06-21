#include "normalize_numexp.hpp"
#include <sstream>

namespace normalize_numexp{
    
	void NormalizeNumexp::normalize_each_type_expressions(const std::string& text,
																	   std::vector<numerical_expression_normalizer::NumericalExpression>& numexps,
																	   std::vector<abstime_expression_normalizer::AbstimeExpression>& abstimeexps,
																	   std::vector<reltime_expression_normalizer::ReltimeExpression>& reltimeexps,
																	   std::vector<duration_expression_normalizer::DurationExpression>& durationexps){
		NEN.process(text, numexps);
		AEN.process(text, abstimeexps);
		REN.process(text, reltimeexps);
		DEN.process(text, durationexps);
	}
	
	/*
	 utilities
	*/
	
	bool is_startswith(std::string sentence, std::string targ){
		if(sentence.size() < targ.size()) return false;
		return sentence.substr(0, targ.size()) == targ;
	}
	
	std::string show_options(std::vector<std::string> options, bool ordinary){
		std::stringstream ss;
		std::string option;
		if(ordinary) options.push_back("ordinary"); //TODO : ordinaryの扱い
		int sz = static_cast<int>(options.size());
		for(int i=0; i<sz; i++){
			option = options[i];
			if(option == "") continue;
			if(is_startswith(option, "MOD_")) option = option.substr(4, option.size()-4);
			ss << option;
			if(i!=sz-1) ss << ",";
		}
		std::string ret;
		ss >> ret;
		return ret;
	}
	
	bool check_time_type(std::string time_str){
		return time_str[time_str.size()-3] == ':';
	}
	
	bool check_time_type_duration(std::string duration_str){
		for(int i=0; i<static_cast<int>(duration_str.size()); i++){
			if(duration_str[i] == 'T') return true;
		}
		return false;
	}
	
	bool is_underspecified(std::string time_str){
		for(int i=0; i<static_cast<int>(time_str.size()); i++){
			if(time_str[i] == 'X') return true;
		}
		return false;
	}
	
	void insert_text(pfi::data::string::ustring& text, int pos, const std::string insert_strings){
		pfi::data::string::ustring text_first_half, text_second_half;
		text_first_half = text.substr(0, pos);
		text_second_half = text.substr(pos, text.size()-pos);
		text = text_first_half;
		text += pfi::data::string::string_to_ustring(insert_strings);
		text += text_second_half;
		;
	}
	
	
	
	/*
	 output simply
	*/
	void merge_normalize_expressions_into_result( std::vector<numerical_expression_normalizer::NumericalExpression> numexps,  std::vector<abstime_expression_normalizer::AbstimeExpression> abstimeexps,  std::vector<reltime_expression_normalizer::ReltimeExpression> reltimeexps,  std::vector<duration_expression_normalizer::DurationExpression> durationexps, std::vector<std::string>& result){

	 //TODO : それぞれの正規形に、toString関数をつける？逆に分かり辛い？　とりあえずここで処理
	 std::string kugiri("*");
	 std::stringstream ss;
	 result.clear();
	 
	 for(int i=0; i<static_cast<int>(numexps.size()); i++){
	 ss.clear(); ss.str("");
	 ss << "numerical" << "*" << numexps[i].original_expression << "*" << numexps[i].position_start << "*" << numexps[i].position_end << "*" << numexps[i].counter << "*" << numexps[i].value_lowerbound << "*" << numexps[i].value_upperbound << "*" << show_options(numexps[i].options, numexps[i].ordinary);
	 result.push_back(ss.str());
	 }
	 
	 for(int i=0; i<static_cast<int>(abstimeexps.size()); i++){
	 ss.clear(); ss.str("");
	 ss << "abstime" << "*" << abstimeexps[i].original_expression << "*" << abstimeexps[i].position_start << "*" << abstimeexps[i].position_end << "*" << "none" << "*" << abstimeexps[i].value_lowerbound.to_string(false) << "*" << abstimeexps[i].value_upperbound.to_string(true) << "*" << show_options(abstimeexps[i].options, abstimeexps[i].ordinary);
	 result.push_back(ss.str());
	 }
	 
	 for(int i=0; i<static_cast<int>(reltimeexps.size()); i++){
	 ss.clear(); ss.str("");
	 //TODO : 相対時間表現を、どう表示させるか？
	 ss << "reltime" << "*" << reltimeexps[i].original_expression << "*" << reltimeexps[i].position_start << "*" << reltimeexps[i].position_end << "*" << "none" << "*" << reltimeexps[i].value_lowerbound_abs.to_string(false) << "," << reltimeexps[i].value_lowerbound_rel.to_duration_string(false) << "*" << reltimeexps[i].value_upperbound_abs.to_string(true) << "," << reltimeexps[i].value_upperbound_rel.to_duration_string(true) << "*" << show_options(reltimeexps[i].options, reltimeexps[i].ordinary);
	 result.push_back(ss.str());
	 }
	 
	 for(int i=0; i<static_cast<int>(durationexps.size()); i++){
	 ss.clear(); ss.str("");
	 ss << "duration" << "*" << durationexps[i].original_expression << "*" << durationexps[i].position_start << "*" << durationexps[i].position_end << "*" << "none" << "*" << durationexps[i].value_lowerbound.to_duration_string(false) << "*" << durationexps[i].value_upperbound.to_duration_string(true) << "*" << show_options(durationexps[i].options, durationexps[i].ordinary);
	 result.push_back(ss.str());
	 }
	}


	
	NormalizeNumexp::NormalizeNumexp(const std::string& language) : NEN(language), AEN(language), REN(language), DEN(language), IER(language) {}


	void NormalizeNumexp::normalize(const std::string& text, std::vector<std::string>& result){
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
	
	
	
	
	
	/*
	 output by timex_format
	*/
	
	std::string convert_timex_format_numexp(numerical_expression_normalizer::NumericalExpression numexp){
		std::stringstream ss; ss.clear(); ss.str("");
		ss << " valueLowerBound=\"" << numexp.value_lowerbound << "\"";
		ss << " valueUpperBound=\"" << numexp.value_upperbound << "\"";
		ss << " counter=\"" << numexp.counter << "\"";		
		//quantの処理。どうしようか？
		if(!numexp.options.empty()) ss << " mod=\"" << show_options(numexp.options, numexp.ordinary) << "\"";
		ss << ">";
		return ss.str();
	}
	
	void add_token(std::string& time_lower_bound_str, std::string& time_upper_bound_str, std::vector<std::string>& options){
		std::vector<std::string>::iterator itr = options.begin();
		while(itr != options.end()){
			if(!is_startswith(*itr, "MOD_")){
				if(*itr == "BC"){
					time_lower_bound_str = "BC" + time_lower_bound_str;
					time_upper_bound_str = "BC" + time_upper_bound_str;
				}else if(*itr == "SP" || *itr == "SU" || *itr == "FA" || *itr == "WI"){
					*itr = "-" + *itr;
					time_lower_bound_str = time_lower_bound_str + *itr;
					time_upper_bound_str = time_upper_bound_str + *itr;
				}else if(*itr == "DN" || *itr == "MO" || *itr == "MI" || *itr == "DT" || *itr == "AF" || *itr == "EV" || *itr == "NI" || *itr == "MN"){
					*itr = "T" + *itr;
					time_lower_bound_str = time_lower_bound_str + *itr;
					time_upper_bound_str = time_upper_bound_str + *itr;					
				}
				options.erase(itr);
				continue;
			}
			itr++;
		}
	}
	
	std::string convert_timex_format_abstimeexp(abstime_expression_normalizer::AbstimeExpression abstimeexp){
		std::stringstream ss; ss.clear(); ss.str("");
		//typeとvalueFromSurfaceLoewrBound,UpperBoundをどうにかする。それでtemporalFunctionも書ける。
		std::string time_lower_bound_str, time_upper_bound_str;
		time_lower_bound_str = abstimeexp.value_lowerbound.to_string(false);
		time_upper_bound_str = abstimeexp.value_upperbound.to_string(true);
		bool is_time_type = check_time_type(time_lower_bound_str);
		ss << " type=\"" << (is_time_type ? "TIME" : "DATE") << "\"";
		add_token(time_lower_bound_str, time_upper_bound_str, abstimeexp.options);
		ss << " valueFromSurfaceLowerBound=\"" << time_lower_bound_str << "\"";
		ss << " valueFromSurfaceUpperBound=\"" << time_upper_bound_str << "\"";
		if(is_underspecified(time_lower_bound_str)) ss << " temporalFunction=\"true\"";
		if(!abstimeexp.options.empty()) ss << " mod=\"" << show_options(abstimeexp.options, abstimeexp.ordinary) << "\"";
		ss << ">";
		return ss.str();
	}
	
	std::string convert_timex_format_reltimeexp(reltime_expression_normalizer::ReltimeExpression reltimeexp){
		std::stringstream ss; ss.clear(); ss.str("");
		
		/*
		 reltimeのtypeは、
		 relのみのとき……… その時間単位による（最小の時間単位でOK）
		 absもあるとき……　も、同じか。そして、relより必ずabsが小さい時間単位？　明日5時　3日1時間後
		 > relかabsの最小の時間単位（absがあるときは、必ずabsになる）で、TYPEが決定される
		 > というか、relがTIMEをもつか、absがtimeか、のどちらか、か。
		 
		 出力は、現状と同じ感じでいいかなあ。
		*/
		std::string time_lower_bound_str, duration_lower_bound_str;
		time_lower_bound_str = reltimeexp.value_lowerbound_abs.to_string(false);
		duration_lower_bound_str = reltimeexp.value_lowerbound_rel.to_duration_string(false);
		bool is_time_type = (check_time_type(time_lower_bound_str) || check_time_type_duration(duration_lower_bound_str));
		ss << " type=\"" << (is_time_type ? "TIME" : "DATE") << "\"";
		ss << " valueFromSurfaceLowerBound=\"" << time_lower_bound_str << "," << duration_lower_bound_str << "\"";
		ss << " valueFromSurfaceUpperBound=\"" << reltimeexp.value_upperbound_abs.to_string(true) << "," << reltimeexp.value_upperbound_rel.to_duration_string(true) << "\"";
		ss << " temporalFunction=\"true\"";
		if(!reltimeexp.options.empty()) ss << " mod=\"" << show_options(reltimeexp.options, reltimeexp.ordinary) << "\"";
		ss << ">";
		return ss.str();
	}
	
	std::string convert_timex_format_durationexp(duration_expression_normalizer::DurationExpression durationexp){
		std::stringstream ss; ss.clear(); ss.str("");
		ss << " type=\"DURATION\"";
		ss << " valueFromSurfaceLowerBound=\"" << durationexp.value_lowerbound.to_duration_string(false) << "\"";
		ss << " valueFromSurfaceUpperBound=\"" << durationexp.value_upperbound.to_duration_string(true) << "\"";		
		//quantの処理。どうしようか？ (EACH, EVERYの処理）
		if(!durationexp.options.empty()) ss << " mod=\"" << show_options(durationexp.options, durationexp.ordinary) << "\"";
		ss << ">";
		return ss.str();
	}
	
	
	void merge_normalize_expressions_into_result_by_timex_format( std::vector<numerical_expression_normalizer::NumericalExpression> numexps,  std::vector<abstime_expression_normalizer::AbstimeExpression> abstimeexps,  std::vector<reltime_expression_normalizer::ReltimeExpression> reltimeexps,  std::vector<duration_expression_normalizer::DurationExpression> durationexps, std::string& text_annotated){
		pfi::data::string::ustring text_annotated_ustr(pfi::data::string::string_to_ustring(text_annotated));
		std::vector<std::pair<std::pair<int,int>, std::string> > results_numex;
		std::vector<std::pair<std::pair<int,int>, std::string> > results_timex;
		
		for(int i=0; i<static_cast<int>(numexps.size()); i++){
			results_numex.push_back(std::make_pair(std::make_pair(numexps[i].position_start, numexps[i].position_end), convert_timex_format_numexp(numexps[i])));
		}
		
		for(int i=0; i<static_cast<int>(abstimeexps.size()); i++){
			results_timex.push_back(std::make_pair(std::make_pair(abstimeexps[i].position_start, abstimeexps[i].position_end), convert_timex_format_abstimeexp(abstimeexps[i])));
		}
		
		for(int i=0; i<static_cast<int>(reltimeexps.size()); i++){
			results_timex.push_back(std::make_pair(std::make_pair(reltimeexps[i].position_start, reltimeexps[i].position_end), convert_timex_format_reltimeexp(reltimeexps[i])));
		}
		
		for(int i=0; i<static_cast<int>(durationexps.size()); i++){
			results_timex.push_back(std::make_pair(std::make_pair(durationexps[i].position_start, durationexps[i].position_end), convert_timex_format_durationexp(durationexps[i])));
		}
		
		//idを付与。
		sort(results_numex.begin(), results_numex.end());
		sort(results_timex.begin(), results_timex.end());
		std::stringstream ss;
		for(int i=0; i<static_cast<int>(results_numex.size()); i++){
			ss.clear(); ss.str("");
			ss << "<NUMEX nid=\"n" << i << "\"" << results_numex[i].second;
			results_numex[i].second = ss.str();
		}
		for(int i=0; i<static_cast<int>(results_timex.size()); i++){
			ss.clear(); ss.str("");
			ss << "<TIMEX tid=\"t" << i << "\"" << results_timex[i].second;
			results_timex[i].second = ss.str();
		}
		
		//pos_start, pos_endを参照して、テキストにアノテートしていく
		int nid, tid;
		nid = results_numex.size()-1;
		tid = results_timex.size()-1;
		int position_end, position_start;
		while(nid != -1 || tid != -1){
			if(nid == -1 || (tid != -1 && results_numex[nid].first.first < results_timex[tid].first.first)){
				//TIMEXをアノテート
				position_start = results_timex[tid].first.first;
				position_end = results_timex[tid].first.second;
				insert_text(text_annotated_ustr, position_end, "</TIMEX>");
				insert_text(text_annotated_ustr, position_start, results_timex[tid].second);
				tid--;
			}else{
				//NUMEXをアノテート
				position_start = results_numex[nid].first.first;
				position_end = results_numex[nid].first.second;
				insert_text(text_annotated_ustr, position_end, "</NUMEX>");
				insert_text(text_annotated_ustr, position_start, results_numex[nid].second);
				nid--;
			}
		}
		text_annotated = pfi::data::string::ustring_to_string(text_annotated_ustr);
	}
	
	void NormalizeNumexp::normalize_by_timex_format(const std::string& text, std::string& text_annotated){
		//TODO : 途中までnormalizeと同じ。マージしたい。
		std::vector<numerical_expression_normalizer::NumericalExpression> numexps;
		std::vector<abstime_expression_normalizer::AbstimeExpression> abstimeexps;
		std::vector<reltime_expression_normalizer::ReltimeExpression> reltimeexps;
		std::vector<duration_expression_normalizer::DurationExpression> durationexps;
		
		//4つのnormalizerで処理を行う
		normalize_each_type_expressions(text, numexps, abstimeexps, reltimeexps, durationexps);
		
		//それぞれの結果より、不適当な抽出を削除
		IER.remove_inappropriate_extraction(text, numexps, abstimeexps, reltimeexps, durationexps);
		
		//string型に変換し、resultにまとめる
		text_annotated = text;
		merge_normalize_expressions_into_result_by_timex_format(numexps, abstimeexps, reltimeexps, durationexps, text_annotated);
	}
	
} //namespace normalize_numexp




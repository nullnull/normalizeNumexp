/*
 数量表現（「三人」「約1000円」などといった表現）や時間表現（「1989年3月」「3:30」「百年後」などといった表現）は以下のように構成される
 
 【接頭辞 + 前置助数詞 + 数量表現or時間表現の基本パターン + 接尾辞】
　　・接頭辞：「約」「およそ」など
　　・前置助数詞：数量表現における「時速」「￥」や、絶対時間表現における年号など（本来はパターンに含めたいところだが、基本パターンをprefixSearchで探索している都合上、今回は別の構成要素として考える）
　　・基本パターン：「*人」「*円」「*年*月」「*:*」などの正規表現パターン。
　　・接尾辞：「以降」「くらい」など
 
 この構成性に着目し、この規格化モジュールでは、文中の数の周囲を正規表現でマッチングさせ、表現を認識させる。
 （「数」 -> 「数」＋「助数詞」 -> 「前置助数詞」＋「数」＋「助数詞」 -> 「前置助数詞」＋「数」＋「助数詞」＋「接尾辞」 -> 「接頭辞」＋「前置助数詞」＋「数」＋「助数詞」　と認識範囲を増やしていく）
 認識した際には、認識したパターンに対応する処理を、辞書を参照して実行し、規格化表現を作成していく。
 
 この基底クラスでは、上のようにパターンを順番に認識していく処理を書いている。
 派生クラスとなるnumerical_expression_normalizer, abstime_expression_normalizer, reltime_expression_normalizer, duration_expression_normalizerでは、認識したパターンに対応する処理を書く。
 
 */

#ifndef NORMALIZER_TEMPLATE_H_
#define NORMALIZER_TEMPLATE_H_
#include <string>
#include <ux/ux.hpp>
#include "digit_utility.hpp"
#include "number_normalizer.hpp"
#include "normalizer_utility.hpp"
#include "dictionary_dirpath.hpp"
#include <unistd.h>

namespace normalizer_template{

template <class AnyTypeExpression, class AnyTypeLimitedExpression>
class NormalizerTemplate{
public:
  virtual void init() = 0;  
  virtual void normalize_number(const std::string& text, std::vector<digit_utility::Number>& numbers) = 0;
  virtual void revise_any_type_expression_by_matching_limited_expression(std::vector<AnyTypeExpression>& any_type_expressions, int& expression_id, AnyTypeLimitedExpression matching_limited_expression) = 0;
  virtual void revise_any_type_expression_by_matching_prefix_counter(AnyTypeExpression& any_type_expression, const AnyTypeLimitedExpression& matching_limited_expression) = 0;
  virtual void revise_any_type_expression_by_number_modifier(AnyTypeExpression& any_type_expression, const normalizer_utility::NumberModifier& number_modifier) = 0;
  virtual void delete_not_any_type_expression(std::vector<AnyTypeExpression>& any_type_expressions) = 0;
  virtual void fix_by_range_expression(const pfi::data::string::ustring& utext, std::vector<AnyTypeExpression>& any_type_expressions) = 0;
  
  void build_limited_expression_patterns_from_limited_expressions(){
    //limited_expressionのpatternでprefixSearchするために、patternをキーとするトライ木を生成する。
    std::vector<std::pair<std::string, int> > limited_expression_pattern_table;
    for(int i=0; i<static_cast<int>(limited_expressions_.size()); i++){
      limited_expression_pattern_table.push_back(make_pair(limited_expressions_[i].pattern, i));
    }
    limited_expression_patterns_.build(limited_expression_pattern_table);
  }

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
    try {
      pfi::text::json::json js = pfi::text::json::json(new pfi::text::json::json_array());
      load_json_from_file(dictionary_path, js);
      pfi::text::json::from_json(js, load_target);
    } catch( ... ) {
      std::cout << "dictionary load error" << std::endl; //TODO : error処理
      exit(1);
    }
  }
  
  template <class T>
  void build_patterns_rev(const std::vector<T>& originals, ux::Map<int>& patterns) {
    //prefixSearchをつかってsuffixSearchを実現するため、uxに格納するパターンを予め前後逆にしておく
    std::vector<std::pair<std::string, int> > kvs;
    for (int i = 0; i < static_cast<int>(originals.size()); i++) {
      kvs.push_back(std::make_pair(normalizer_utility::reverse_string(originals[i].pattern), i));
    }
    patterns.build(kvs);
  }
  
  template <class T>
  void build_patterns(const std::vector<T>& originals, ux::Map<int>& patterns) {
    std::vector<std::pair<std::string, int> > kvs;
    for (int i = 0; i < static_cast<int>(originals.size()); i++) {
      kvs.push_back(std::make_pair(originals[i].pattern, i));
    }
    patterns.build(kvs);
  }
  
  void load_from_dictionaries(const std::string& limited_expression_dictionary, const std::string& prefix_counter_dictionary, const std::string& prefix_number_modifier_dictionary, const std::string& suffix_number_modifier_dictionary){
	std::string dictionary_path;
	dictionary_path += dictionary_dirpath::get_dictionary_dirpath();
    dictionary_path += language_; 
	dictionary_path += "/";
    load_from_dictionary(dictionary_path+limited_expression_dictionary, limited_expressions_);
    load_from_dictionary(dictionary_path+prefix_counter_dictionary, prefix_counters_);
    load_from_dictionary(dictionary_path+suffix_number_modifier_dictionary, suffix_number_modifier_);
    load_from_dictionary(dictionary_path+prefix_number_modifier_dictionary, prefix_number_modifier_);
    
    build_patterns(limited_expressions_, limited_expression_patterns_);
    build_patterns_rev(prefix_counters_, prefix_counter_patterns_);
    build_patterns_rev(prefix_number_modifier_, prefix_number_modifier_patterns_);
    build_patterns(suffix_number_modifier_, suffix_number_modifier_patterns_);
    
    for(int i=0; i<static_cast<int>(limited_expressions_.size()); i++){
      limited_expressions_[i].set_total_number_of_place_holder();
      limited_expressions_[i].set_length_of_strings_after_final_place_holder();
    }
  }
  
  void search_matching_limited_expression(const pfi::data::string::ustring& utext_replaced, const AnyTypeExpression& any_type_expression, int& matching_pattern_id){
    pfi::data::string::ustring string_after_expression;
    normalizer_utility::extract_after_string(utext_replaced, any_type_expression.position_end, string_after_expression);
    normalizer_utility::prefixSearch(string_after_expression, limited_expression_patterns_, matching_pattern_id);
  }
  
  void search_matching_prefix_counter(const pfi::data::string::ustring& utext_replaced, const AnyTypeExpression& any_type_expression, int& matching_pattern_id){
    pfi::data::string::ustring string_before_expression;
    normalizer_utility::extract_before_string(utext_replaced, any_type_expression.position_start, string_before_expression);
    normalizer_utility::suffixSearch(string_before_expression, prefix_counter_patterns_, matching_pattern_id);
  }
  
  void revise_any_type_expression_by_matching_prefix_number_modifier(AnyTypeExpression& any_type_expression, const normalizer_utility::NumberModifier& number_modifier){
    any_type_expression.position_start -= pfi::data::string::string_to_ustring(number_modifier.pattern).size();
    revise_any_type_expression_by_number_modifier(any_type_expression, number_modifier);
  }
  
  void revise_any_type_expression_by_matching_suffix_number_modifier(AnyTypeExpression& any_type_expression, const normalizer_utility::NumberModifier& number_modifier){
    any_type_expression.position_end += pfi::data::string::string_to_ustring(number_modifier.pattern).size();
    revise_any_type_expression_by_number_modifier(any_type_expression, number_modifier);
  }
  
  bool normalize_limited_expression(const pfi::data::string::ustring& utext_replaced, std::vector<AnyTypeExpression>& any_type_expressions, int &i){
    int matching_pattern_id;
    search_matching_limited_expression(utext_replaced, any_type_expressions[i], matching_pattern_id);
    if(matching_pattern_id == -1) return false;
    revise_any_type_expression_by_matching_limited_expression(any_type_expressions, i, limited_expressions_[matching_pattern_id]);
    return true;
  }
  
  void normalize_prefix_counter(const pfi::data::string::ustring& utext_replaced, AnyTypeExpression& any_type_expression){
    int matching_pattern_id;
    search_matching_prefix_counter(utext_replaced, any_type_expression, matching_pattern_id);
    if(matching_pattern_id == -1) return;
    revise_any_type_expression_by_matching_prefix_counter(any_type_expression, prefix_counters_[matching_pattern_id]);
    return;
  }
  
  void normalize_suffix_number_modifier(const pfi::data::string::ustring& utext_replaced, AnyTypeExpression& any_type_expression){
    int matching_pattern_id;
    normalizer_utility::search_suffix_number_modifier(utext_replaced, any_type_expression.position_end, suffix_number_modifier_patterns_, matching_pattern_id);
    if(matching_pattern_id == -1) return;
    revise_any_type_expression_by_matching_suffix_number_modifier(any_type_expression, suffix_number_modifier_[matching_pattern_id]);
  }
  
  void normalize_prefix_number_modifier(const pfi::data::string::ustring& utext_replaced, AnyTypeExpression& any_type_expression){
    int matching_pattern_id;
    normalizer_utility::search_prefix_number_modifier(utext_replaced, any_type_expression.position_start, prefix_number_modifier_patterns_, matching_pattern_id);
    if(matching_pattern_id == -1) return;
    revise_any_type_expression_by_matching_prefix_number_modifier(any_type_expression, prefix_number_modifier_[matching_pattern_id]);
  }
  
  void convert_numbers_to_any_type_expressions(const std::vector<digit_utility::Number>& numbers, std::vector<AnyTypeExpression>& any_type_expressions){
    for(int i=0; i<static_cast<int>(numbers.size()); i++){
      any_type_expressions.push_back(numbers[i]);
    }
  }
  
  bool have_kara_prefix(const std::vector<std::string>& options){
    return find(options.begin(), options.end(), "kara_prefix") != options.end();
  }
  
  bool have_kara_suffix(const std::vector<std::string>& options){
    return find(options.begin(), options.end(), "kara_suffix") != options.end();
  }
  
  void merge_options(std::vector<std::string>& options1, std::vector<std::string>& options2){
    for(int i=0; i<static_cast<int>(options2.size()); i++){
      options1.push_back(options2[i]);
    }
  }
  
  void process(const std::string& text, std::vector<AnyTypeExpression>& any_type_expressions) {
    any_type_expressions.clear();
    pfi::data::string::ustring utext = pfi::data::string::string_to_ustring(text);
    
    //numbersの作成
    std::vector<digit_utility::Number> numbers;
    normalize_number(text, numbers);
    
    //numbersを変換して、ベースとなるany_type_expressionsを作成
    convert_numbers_to_any_type_expressions(numbers, any_type_expressions);
    
    //searchするために、text中の数を*に置換しておく
    pfi::data::string::ustring utext_replaced;
    normalizer_utility::replace_numbers_in_text(utext, numbers, utext_replaced);
    
    //単位の探索、規格化
    for(int i=0; i<static_cast<int>(any_type_expressions.size()); i++){
      if(!normalize_limited_expression(utext_replaced, any_type_expressions, i)){
        //TODO : 単位が存在しなかった場合の処理をどうするか、相談して決める
      }
      normalize_prefix_counter(utext_replaced, any_type_expressions[i]);
      normalize_suffix_number_modifier(utext_replaced, any_type_expressions[i]); //TODO : 2回以上の接尾辞・接頭辞の繰り返しを許すか？（例：「およそ約3人」）
      normalize_prefix_number_modifier(utext_replaced, any_type_expressions[i]);
      any_type_expressions[i].set_original_expression_from_position(utext);
    }
    
    //TODO : 範囲表現の処理
    fix_by_range_expression(utext, any_type_expressions);
    
    //規格化されなかったnumberを削除
    delete_not_any_type_expression(any_type_expressions);
  }
  
  ux::Map<int> limited_expression_patterns_, prefix_counter_patterns_, suffix_number_modifier_patterns_, prefix_number_modifier_patterns_;
  std::vector<AnyTypeLimitedExpression> limited_expressions_, prefix_counters_;
  std::vector<normalizer_utility::NumberModifier> suffix_number_modifier_, prefix_number_modifier_;
  std::string language_;
};

} //namespace numerical_expression_normalizer

#endif //NORMALIZER_TEMPLATE_H_

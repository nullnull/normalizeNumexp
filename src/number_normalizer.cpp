#include "number_normalizer.hpp"
#include "digit_utility.hpp"
#include <sstream>
#include <math.h>
#include <iostream>

namespace number_normalizer {
using namespace digit_utility;

template<class T1, class T2>
void cast(const T1& a, T2& b) {
  std::stringstream ss;
  ss << a;
  ss >> b;
}

void update_notation_type(const pfi::data::string::uchar uc, int& notation_type) {
  if (digit_utility::is_hankakusuji(uc)) notation_type |= HANKAKU;
  else if (digit_utility::is_zenkakusuji(uc)) notation_type |= ZENKAKU;
  else if (digit_utility::is_kansuji_09(uc)) notation_type |= KANSUJI_09;
  else if (digit_utility::is_kansuji_kurai_sen(uc)) notation_type |= KANSUJI_KURAI_SEN;
  else if (digit_utility::is_kansuji_kurai_man(uc)) notation_type |= KANSUJI_KURAI_MAN;
}

bool include_mask(const int mask, const int submask) {
  return (mask & submask) == submask;
}

bool suffix_is_arabic(const pfi::data::string::ustring& number_string) {
  return number_string.size() > 0 && is_arabic(number_string[number_string.size() - 1]);
}

bool prefix_3digit_is_arabic(const pfi::data::string::ustring& number_string) {
  return number_string.size() > 2 && is_arabic(number_string[0]) && is_arabic(number_string[1]) && is_arabic(number_string[2]);
}

bool is_valid_comma_notation(const pfi::data::string::ustring& number_string1, const pfi::data::string::ustring& number_string2) {
  return suffix_is_arabic(number_string1) && prefix_3digit_is_arabic(number_string2) && (number_string2.size() == 3 || !is_arabic(number_string2[3]));
}

void join_numbers_by_comma(const std::string& text, std::vector<Number>& numbers) {
  //カンマ表記を統合する。カンマは「3,000,000」のように3桁ごとに区切っているカンマしか数のカンマ表記とは認めない（「29,30」のような表記は認めない）
  for (int i = static_cast<int>(numbers.size()) - 1; i > 0; i--) {
    if (numbers[i - 1].position_end != numbers[i].position_start - 1) continue;
    pfi::data::string::uchar uchar_intermediate = pfi::data::string::string_to_ustring(text)[numbers[i - 1].position_end];
    if (!digit_utility::is_comma(uchar_intermediate)) continue;
    if (!is_valid_comma_notation(numbers[i - 1].original_expression, numbers[i].original_expression)) continue;
    numbers[i - 1].position_end = numbers[i].position_end;
    numbers[i - 1].original_expression += uchar_intermediate;
    numbers[i - 1].original_expression += numbers[i].original_expression;
    numbers.erase(numbers.begin() + i);
  }
}
  
void split_by_kansuji_kurai(const pfi::data::string::ustring& number_string,
                            std::vector<std::pair<pfi::data::string::ustring, pfi::data::string::uchar> >& number_string_splited) {
  number_string_splited.clear();
  pfi::data::string::ustring ustr;
  for (int i = 0; i < static_cast<int>(number_string.size()); i++) {
    pfi::data::string::uchar uc = number_string[i];
    if (is_kansuji_kurai_man(uc)) {
      number_string_splited.push_back(make_pair(ustr, uc));
      ustr.clear();
    } else {
      ustr += pfi::data::string::string_to_ustring(pfi::data::string::uchar_to_string(uc));
    }
  }
  number_string_splited.push_back(make_pair(ustr, pfi::data::string::string_to_uchar("　")));
}

bool NumberExtractor::is_invalid_notation_type(const int notation_type) {
  //不適当な数字の表記を、表記タイプから判断する。（「２０００30」や「2000三十」などの、数字の表記が入り乱れているもの）
  return include_mask(notation_type, (HANKAKU | ZENKAKU)) || include_mask(notation_type, (HANKAKU | KANSUJI_09))
      || include_mask(notation_type, (ZENKAKU | KANSUJI_09));
}
  
bool is_invalid_kansuji_kurai_order(const pfi::data::string::uchar uc, pfi::data::string::ustring& kansuji_kurai_strings_for_check_invalid_notation){
  if(kansuji_kurai_strings_for_check_invalid_notation.empty()){
    kansuji_kurai_strings_for_check_invalid_notation += uc;
    return false;
  }
  int prev = convert_kansuji_kurai_to_power_value(kansuji_kurai_strings_for_check_invalid_notation[kansuji_kurai_strings_for_check_invalid_notation.size()-1]);
  int cur = convert_kansuji_kurai_to_power_value(uc);
  if(cur<prev){
    kansuji_kurai_strings_for_check_invalid_notation += uc;
    return false;
  }else{
    return true;
  }
}

bool is_invalid_notation(const pfi::data::string::uchar uc, pfi::data::string::ustring& kansuji_kurai_man_strings_for_check_invalid_notation, pfi::data::string::ustring& kansuji_kurai_sen_strings_for_check_invalid_notation){
  //「百2千」「一万五千七百億」のような不適当な表記（百が千の前にある、万が億の前にある、など）を検出する。 リアルタイム検出するために、変数は呼び出し元と共有し変数をアップデートしていくアルゴリズムになっている（ucは今回対象とする数）
  if(is_kansuji_kurai_man(uc)){
    kansuji_kurai_sen_strings_for_check_invalid_notation.clear();
    return is_invalid_kansuji_kurai_order(uc, kansuji_kurai_man_strings_for_check_invalid_notation);
  } else if (is_kansuji_kurai_sen(uc)){
    return is_invalid_kansuji_kurai_order(uc, kansuji_kurai_sen_strings_for_check_invalid_notation);
  }
}

void NumberExtractor::return_longest_number_strings(const pfi::data::string::ustring& utext, int &i, std::string& numstr) {
  int notation_type = NOT_NUMBER;
  pfi::data::string::ustring kansuji_kurai_sen_strings_for_check_invalid_notation, kansuji_kurai_man_strings_for_check_invalid_notation; // TODO : 変数名これで良い??…。
  numstr.clear();
  for (; i < static_cast<int>(utext.size()); i++) {
    pfi::data::string::uchar uc = utext[i];
    if (not digit_utility::is_number(uc)) return;
    update_notation_type(uc, notation_type);
    if (is_invalid_notation_type(notation_type) || is_invalid_notation(uc, kansuji_kurai_man_strings_for_check_invalid_notation, kansuji_kurai_sen_strings_for_check_invalid_notation)) {
      i--;
      return;
    }
    numstr += pfi::data::string::uchar_to_string(uc);
  }
}

void NumberExtractor::extract_number(const std::string& text, std::vector<Number>& numbers) {
  numbers.clear();
  pfi::data::string::ustring utext = pfi::data::string::string_to_ustring(text);
  std::string numstr;
  for (int i = 0; i < static_cast<int>(utext.size()); i++) {
    pfi::data::string::uchar uc = utext[i];
    if (digit_utility::is_number(uc)) {
      Number number;
      number.position_start = i;
      return_longest_number_strings(utext, i, numstr);
      number.original_expression = pfi::data::string::string_to_ustring(numstr);
      number.position_end = i;
      numbers.push_back(number);
    }
  }
}

void NumberConverterTemplate::delete_comma(const pfi::data::string::ustring& ustr, pfi::data::string::ustring& ret) {
  ret.clear();
  for (int i = 0; i < static_cast<int>(ustr.size()); i++) {
    pfi::data::string::uchar uc = ustr[i];
    if (!digit_utility::is_comma(uc)) {
      ret += uc;
    }
  }
}

void NumberConverterTemplate::identify_notation_type_of_number_string(const pfi::data::string::ustring& number_string, int& notation_type) {
  notation_type = NOT_NUMBER;
  for (int i = 0; i < static_cast<int>(number_string.size()); i++) {
    pfi::data::string::uchar uc = number_string[i];
    update_notation_type(uc, notation_type);
  }
}

void NumberConverterTemplate::convert_arabic_numerals(const pfi::data::string::ustring& number_string, double& value) {
  cast(number_string, value);
}

void NumberConverterTemplate::convert_kansuji_by_positional_notation(const pfi::data::string::ustring& number_string, double& value) {
  std::stringstream ss;
  for (int i = 0; i < static_cast<int>(number_string.size()); i++) {
    pfi::data::string::uchar uc = number_string[i];
    ss << convert_kansuji_09_to_value(uc);
  }
  ss >> value;
}

void NumberConverterTemplate::convert_arabic_kansuji_kurai_man_mixed(const pfi::data::string::ustring& number_string, double& value) {
  value = 0;
  std::vector<std::pair<pfi::data::string::ustring, pfi::data::string::uchar> > number_string_splited;
  split_by_kansuji_kurai(number_string, number_string_splited);
  int number_converted;
  for (int i = 0; i < static_cast<int>(number_string_splited.size()); i++) {
    cast(number_string_splited[i].first, number_converted);
    value += number_converted * pow(10, digit_utility::convert_kansuji_kurai_to_power_value(number_string_splited[i].second));
    number_converted = 0;
  }
}

void JapaneseNumberConverter::convert_arabic_kansuji_mixed_of_4digit(const pfi::data::string::ustring& number_string, int& number_converted) {
  number_converted = 0;
  int tmpnum = -1;
  for (int i = 0; i < static_cast<int>(number_string.size()); i++) {
    pfi::data::string::uchar uc = number_string[i];
    if (is_kansuji_kurai_sen(uc)) {
      if (tmpnum == -1) tmpnum = 1;
      number_converted += tmpnum * pow(10, convert_kansuji_kurai_to_power_value(uc));
      tmpnum = -1;
    } else if (is_kansuji_09(uc)) {
      tmpnum = convert_kansuji_09_to_value(uc);
    } else if (is_hankakusuji(uc)) {
      cast(pfi::data::string::uchar_to_string(uc), tmpnum);
    }
  }
  if (tmpnum != -1) number_converted += tmpnum;
}
  
void ChineseNumberConverter::convert_arabic_kansuji_mixed_of_4digit(const pfi::data::string::ustring& number_string, int& number_converted) {
  /* 基本的に日本語と同じだが、「０」の扱いが異なる。「千二百三」は「1230」となり、「千二百零三」が「1203」となる。
     よって、直前に零が出てきたかどうか、また出てきた場合のために前の桁を覚えておく必要がある。
   */
  number_converted = 0;
  int tmpnum = -1;
  int power_value;
  bool prev_is_zero = false, current_is_zero = false; //chinese specific
  for (int i = 0; i < static_cast<int>(number_string.size()); i++) {
    pfi::data::string::uchar uc = number_string[i];
    if (is_kansuji_kurai_sen(uc)) {
      if (tmpnum == -1) tmpnum = 1;
      power_value = convert_kansuji_kurai_to_power_value(uc);
      number_converted += tmpnum * pow(10, power_value);
      tmpnum = -1;
    } else if (is_kansuji_09(uc)) {
      tmpnum = convert_kansuji_09_to_value(uc);
      prev_is_zero = current_is_zero;
      if(tmpnum == 0) current_is_zero = true;
      else current_is_zero = false;
    } else if (is_hankakusuji(uc)) {
      cast(pfi::data::string::uchar_to_string(uc), tmpnum);
    }
  }
  if (tmpnum != -1) {
    if(prev_is_zero){
      number_converted += tmpnum;
    }else{
      number_converted += tmpnum * pow(10, power_value-1);
    }
  }
}

void ArabicNumberConverter::convert_arabic_kansuji_mixed_of_4digit(const pfi::data::string::ustring& number_string, int& number_converted) {} //日本語、中国語以外では漢数字がでてこないので、実装の必要はない
  
void NumberConverterTemplate::convert_kansuji(const pfi::data::string::ustring& number_string, double& value) {
  value = 0;
  std::vector<std::pair<pfi::data::string::ustring, pfi::data::string::uchar> > number_string_splited;
  split_by_kansuji_kurai(number_string, number_string_splited);
  int number_converted;
  for (int i = 0; i < static_cast<int>(number_string_splited.size()); i++) {
    convert_arabic_kansuji_mixed_of_4digit(number_string_splited[i].first, number_converted);
    value += number_converted * pow(10, digit_utility::convert_kansuji_kurai_to_power_value(number_string_splited[i].second));
    number_converted = 0;
  }
}

void NumberConverterTemplate::convert_arabic_kansuji_mixed(const pfi::data::string::ustring& number_string, double& value) {
  value = 0;
  std::vector<std::pair<pfi::data::string::ustring, pfi::data::string::uchar> > number_string_splited;
  split_by_kansuji_kurai(number_string, number_string_splited);
  int number_converted;
  for (int i = 0; i < static_cast<int>(number_string_splited.size()); i++) {
    convert_arabic_kansuji_mixed_of_4digit(number_string_splited[i].first, number_converted);
    value += number_converted * pow(10, digit_utility::convert_kansuji_kurai_to_power_value(number_string_splited[i].second));
    number_converted = 0;
  }
}

void NumberConverterTemplate::convert_number(const pfi::data::string::ustring& number_string_org, double& value, int& notation_type) {
  notation_type = NOT_NUMBER;
  pfi::data::string::ustring number_string;
  delete_comma(number_string_org, number_string);
  number_string = pfi::data::string::zenkaku_latin_to_basic_latin(number_string);

  identify_notation_type_of_number_string(number_string, notation_type);
  if (notation_type == HANKAKU) convert_arabic_numerals(number_string, value);
  else if (notation_type == KANSUJI_09) convert_kansuji_by_positional_notation(number_string, value);
  else if (notation_type == (HANKAKU | KANSUJI_KURAI_MAN)) convert_arabic_kansuji_kurai_man_mixed(number_string, value);
  else if (notation_type == (KANSUJI_09 | KANSUJI_KURAI_SEN) || notation_type == KANSUJI_KURAI_SEN
      || notation_type == (KANSUJI_09 | KANSUJI_KURAI_MAN) || notation_type == (KANSUJI_09 | KANSUJI_KURAI_SEN | KANSUJI_KURAI_MAN)) convert_kansuji(
      number_string, value);
  else if (notation_type == (HANKAKU | KANSUJI_KURAI_SEN) || notation_type == (HANKAKU | KANSUJI_KURAI_SEN | KANSUJI_KURAI_MAN)) convert_arabic_kansuji_mixed(
      number_string, value);
  else {
    //TODO : error処理はどうする？
  }
}
  
void ArabicNumberConverter::convert_number(const pfi::data::string::ustring& number_string_org, double& value, int& notation_type) {
  //日本語、中国語以外では漢数字がでてこないので、アラビア数字を値として認識する処理のみを行う
  //もし漢数字以外にも、アラビア数字以外の表記を数字として認識させたい場合は、ExtractorとConverterの2つに処理を追記する
  notation_type = NOT_NUMBER;
  pfi::data::string::ustring number_string;
  delete_comma(number_string_org, number_string);
  number_string = pfi::data::string::zenkaku_latin_to_basic_latin(number_string);
  
  identify_notation_type_of_number_string(number_string, notation_type);
  if (notation_type == HANKAKU) convert_arabic_numerals(number_string, value);
  else {
    //TODO : error処理はどうする？
  }
}

bool SymbolFixer::is_plus(const pfi::data::string::ustring& utext, int i, pfi::data::string::ustring& plus_strings) {
  if (i < 0) return false;
  std::string str = pfi::data::string::uchar_to_string(utext[i]);
  if (str == "+" or str == "＋") {
    plus_strings = pfi::data::string::string_to_ustring(str);
    return true;
  }

  if (i < 2) return false;
  str = pfi::data::string::ustring_to_string(utext.substr(i-2, 3));
  if (str == "プラス") {
    plus_strings = pfi::data::string::string_to_ustring(str);
    return true;
  }
  return false;
}

bool SymbolFixer::is_minus(const pfi::data::string::ustring& utext, int i, pfi::data::string::ustring& plus_strings) {
  if (i < 0) return false;
  std::string str = pfi::data::string::uchar_to_string(utext[i]);
  if (str == "-" or str == "ー") {
    plus_strings = pfi::data::string::string_to_ustring(str);
    return true;
  }

  if (i < 3) return false;
  str = pfi::data::string::ustring_to_string(utext.substr(i - 3, 4));
  if (str == "マイナス") {
    plus_strings = pfi::data::string::string_to_ustring(str);
    return true;
  }
  return false;
}

void SymbolFixer::fix_prefix_symbol(const pfi::data::string::ustring& utext, std::vector<Number>& numbers, int i) {
  pfi::data::string::ustring plus_strings, minus_strings;
  if (is_plus(utext, numbers[i].position_start - 1, plus_strings)) {
    numbers[i].original_expression = plus_strings + numbers[i].original_expression;
    numbers[i].position_start--;
  } else if (is_minus(utext, numbers[i].position_start - 1, minus_strings)) {
    numbers[i].value_lowerbound *= -1;
    numbers[i].value_upperbound *= -1;
    numbers[i].original_expression = minus_strings + numbers[i].original_expression;
    numbers[i].position_start--;
  }
  //TODO : 「数」の処理はどうする？　「数十円」など
}

double SymbolFixer::create_decimal_value(const Number& number) {
  double decimal;
  decimal = number.value_lowerbound;
  while (decimal >= 1)
    decimal *= 0.1;
  int place = 0;
  while (1) {
    std::string str = pfi::data::string::uchar_to_string(number.original_expression[place]);
    if (str != "0" and str != "０" and str != "零" and str != "〇") break;
    decimal *= 0.1;
    place++;
  }
  return decimal;
}

void SymbolFixer::fix_decimal_point(std::vector<Number>& numbers, int i, pfi::data::string::ustring decimal_strings) {
  double decimal = create_decimal_value(numbers[i+1]);
  numbers[i].value_lowerbound += decimal;
  numbers[i].value_upperbound = numbers[i].value_lowerbound;
  numbers[i].original_expression += decimal_strings;
  numbers[i].original_expression += numbers[i+1].original_expression;
  numbers[i].position_end = numbers[i+1].position_end;
  numbers.erase(numbers.begin() + (i+1));
}

void SymbolFixer::fix_range_expression(std::vector<Number>& numbers, int i, pfi::data::string::ustring range_strings) {
  numbers[i].value_upperbound = numbers[i+1].value_lowerbound;
  numbers[i].position_end = numbers[i+1].position_end;
  numbers[i].original_expression += range_strings;
  numbers[i].original_expression += numbers[i+1].original_expression;
  numbers[i].position_end = numbers[i+1].position_end;
  numbers.erase(numbers.begin() + (i+1));
}

void SymbolFixer::fix_intermediate_symbol(const pfi::data::string::ustring& utext, std::vector<Number>& numbers, int i) {
  if (static_cast<int>(numbers.size()) - 1 <= i) return;
  pfi::data::string::ustring intermediate = utext.substr(numbers[i].position_end, numbers[i + 1].position_start - numbers[i].position_end);
  pfi::data::string::ustring decimal_strings;
  if(numbers[i].value_lowerbound == INFINITY || numbers[i+1].value_lowerbound == INFINITY) return;
  if (digit_utility::is_decimal_point(intermediate)) {
    fix_decimal_point(numbers, i, intermediate);
  }
  if ((digit_utility::is_range_expression(intermediate)) || (digit_utility::is_comma(intermediate[0]) && intermediate.size() == 1 && (numbers[i].value_lowerbound == numbers[i+1].value_upperbound-1))) { //範囲表現か、コンマの並列表現のとき
    fix_range_expression(numbers, i, intermediate);
  }
  //TODO : 「数」の処理はどうする？　「百数十円」など
}

void SymbolFixer::fix_suffix_symbol(const pfi::data::string::ustring& utext, std::vector<Number>& numbers, int i) {
  //TODO : 「数」の処理はどうする？　「十数円」など
}

void SymbolFixer::fix_numbers_by_symbol(const std::string& text, std::vector<Number>& numbers) {
  pfi::data::string::ustring utext = pfi::data::string::string_to_ustring(text);
  for (int i = 0; i < static_cast<int>(numbers.size()); i++) {
    fix_prefix_symbol(utext, numbers, i);
    fix_intermediate_symbol(utext, numbers, i);
    fix_suffix_symbol(utext, numbers, i);
  }
}

template <class NumberConverter>
void convert_number(NumberConverter& NC, std::vector<Number>& numbers){
  for (int i = 0; i < static_cast<int>(numbers.size()); i++) {
    NC.convert_number(numbers[i].original_expression, numbers[i].value_lowerbound, numbers[i].notation_type);
    numbers[i].value_upperbound = numbers[i].value_lowerbound;
  }
}

void remove_only_kansuji_kurai_man(std::vector<Number>& numbers) {
	for(int i = static_cast<int>(numbers.size()-1); i>=0; i--){
		if(numbers[i].original_expression.size() == 1 && is_kansuji_kurai_man(numbers[i].original_expression[0])){
			numbers.erase(numbers.begin() + i);
		}
	}
}

void NumberNormalizer::process(const std::string& text, std::vector<Number>& numbers) {
  numbers.clear();

  //入力文inputに含まれる数の表記を抽出
  NumberExtractor NE;
  NE.extract_number(text, numbers);

  //カンマの処理を行う
  join_numbers_by_comma(text, numbers);

  //それぞれの数の表記を、数に変換
  if(language_ == "ja") {
    JapaneseNumberConverter NC;
    convert_number(NC, numbers);
  }
  else if(language_ == "zh") {
    ChineseNumberConverter NC;
    convert_number(NC, numbers);
  }else{
    ArabicNumberConverter NC;
    convert_number(NC, numbers);
  }

  //記号の処理を行う
  SymbolFixer SF;
  SF.fix_numbers_by_symbol(text, numbers);
  
  //「京」「万」など単独のものを削除する（ここで処理を行わないと、「数万」などに対応できない）
  remove_only_kansuji_kurai_man(numbers);
}
  
void NumberNormalizer::process_dont_fix_by_symbol(const std::string& text, std::vector<Number>& numbers) {
  numbers.clear();
  
  //入力文inputに含まれる数の表記を抽出
  NumberExtractor NE;
  NE.extract_number(text, numbers);
  
  //それぞれの数の表記を、数に変換
  if(language_ == "ja") {
    JapaneseNumberConverter NC;
    convert_number(NC, numbers);
  }
  else if(language_ == "zh") {
    ChineseNumberConverter NC;
    convert_number(NC, numbers);
  }else{
    ArabicNumberConverter NC;
    convert_number(NC, numbers);
  }
	
	//「京」「万」など単独のものを削除する（ここで処理を行わないと、「数万」などに対応できない）
	remove_only_kansuji_kurai_man(numbers);

}
  
} //namespace number_normalizer

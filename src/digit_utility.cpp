#include "digit_utility.hpp"
#include <pficommon/text/json.h>
//debug
namespace digit_utility {

std::map<std::string, ENotationType> string_to_notation_type;
std::map<std::string, int> kansuji_09_to_value;
std::map<std::string, int> kansuji_kurai_to_power_value;

struct ChineseCharacter {
  template <class Archive>
  void serialize(Archive &ar){
    ar & MEMBER(character) & MEMBER(NotationType) & MEMBER(value);
  }
  
  std::string character, NotationType;
  int value;
};

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
void load_from_dictionary(const std::string& filepath, std::vector<T>& load_target) {
  load_target.clear();
  pfi::text::json::json js = pfi::text::json::json(new pfi::text::json::json_array());
  load_json_from_file(filepath, js);
  pfi::text::json::from_json(js, load_target);
}

void init_kansuji(const std::string& language){
  std::vector<ChineseCharacter> chinese_characters;
  std::string path("/home/katsuma/src/digit_utils/src/dic/");
  if(language == "ja"){
    path += "ja/chinese_character.txt";
  }else if (language == "zh"){
    path += "zh/chinese_character.txt";
  }else {
    return;
  }
  load_from_dictionary(path, chinese_characters);
  for(int i=0; i<chinese_characters.size(); i++){
    ENotationType notation_type;
    if(chinese_characters[i].NotationType == "09") notation_type = KANSUJI_09;
    else if(chinese_characters[i].NotationType == "sen") notation_type = KANSUJI_KURAI_SEN;
    else if(chinese_characters[i].NotationType == "man") notation_type = KANSUJI_KURAI_MAN;
    string_to_notation_type[chinese_characters[i].character] = notation_type;
    if(notation_type == KANSUJI_09) kansuji_09_to_value[chinese_characters[i].character] = chinese_characters[i].value;
    else if(notation_type == KANSUJI_KURAI_MAN || notation_type == KANSUJI_KURAI_SEN) kansuji_kurai_to_power_value[chinese_characters[i].character] = chinese_characters[i].value;
  }
  kansuji_kurai_to_power_value["　"] = 0;
}
  
bool is_hankakusuji(const pfi::data::string::uchar uc) {
  return (pfi::data::string::string_to_uchar("0") <= uc
      && uc <= pfi::data::string::string_to_uchar("9"));
}

bool is_zenkakusuji(const pfi::data::string::uchar uc) {
  return (pfi::data::string::string_to_uchar("０") <= uc
      && uc <= pfi::data::string::string_to_uchar("９"));
}

bool is_arabic(const pfi::data::string::uchar uc) {
  return (is_hankakusuji(uc) || is_zenkakusuji(uc));
}

bool is_notation_type(const pfi::data::string::uchar uc, ENotationType NOTATION_TYPE) {
  std::map<std::string, ENotationType>::const_iterator itr =
      string_to_notation_type.find(pfi::data::string::uchar_to_string(uc));
  if (itr == string_to_notation_type.end())
    return 0;
  return (itr->second) & NOTATION_TYPE;
}

bool is_kansuji(const pfi::data::string::uchar uc) {
  return is_notation_type(uc, KANSUJI);
}

bool is_kansuji_09(const pfi::data::string::uchar uc) {
  return is_notation_type(uc, KANSUJI_09);
}

bool is_kansuji_kurai_sen(const pfi::data::string::uchar uc) {
  return is_notation_type(uc, KANSUJI_KURAI_SEN);
}

bool is_kansuji_kurai_man(const pfi::data::string::uchar uc) {
  return is_notation_type(uc, KANSUJI_KURAI_MAN);
}

bool is_kansuji_kurai(const pfi::data::string::uchar uc) {
  return is_notation_type(uc, KANSUJI_KURAI);
}

bool is_comma(const pfi::data::string::uchar uc) {
  std::string str = pfi::data::string::uchar_to_string(uc);
  return (str == "," || str == "、" || str == "，");
}

bool is_decimal_point(const pfi::data::string::ustring& ustr) {
  std::string str = pfi::data::string::ustring_to_string(ustr);
  return (str == "." || str == "・" || str == "．");
}

bool is_range_expression(const pfi::data::string::ustring& ustr) {
  std::string str = pfi::data::string::ustring_to_string(ustr);
  return (str == "~" || str == "〜" || str == "～" || str == "-" || str == "−" || str == "ー" || str == "―" || str == "から");
}

bool is_number(const pfi::data::string::uchar uc) {
  return is_hankakusuji(uc) or is_zenkakusuji(uc) or is_kansuji(uc);
}

int convert_kansuji_09_to_value(const pfi::data::string::uchar uc) {
  std::string str = pfi::data::string::uchar_to_string(uc);
  std::map<std::string, int>::const_iterator itr = kansuji_09_to_value.find(str);
  if (itr == kansuji_09_to_value.end()) {
    //例外処理。どうする？
    throw "Exception : is not kansuji09";
  }
  return (itr->second);
}

int convert_kansuji_kurai_to_power_value(const pfi::data::string::uchar uc) {
  std::string str = pfi::data::string::uchar_to_string(uc);
  std::map<std::string, int>::const_iterator itr = kansuji_kurai_to_power_value.find(str);
  if (itr == kansuji_kurai_to_power_value.end()) {
    //例外処理。どうする？
    throw "Exception : is not kansuji_kurai";
  }
  return (itr->second);
}
} //namespace digit_utility

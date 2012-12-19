#include <gtest/gtest.h>
#include <string>
#include <complex>
#include "normalizer_utility.hpp"

#include <pficommon/data/string/ustring.h>
#include <ux/ux.hpp>

using namespace normalizer_utility;
using namespace std;
using namespace pfi::data::string;
class NormalizerUtilityTest: public testing::Test {
public:
  ux::Map<int> uxm, uxm_rev;
  ustring rev(ustring str) {
    return ustring(str.rbegin(), str.rend());
  }

  void SetUp() {
    vector<pair<string, int> > kvs, kvs_rev;
    kvs.push_back(make_pair("あ", 1));
    kvs.push_back(make_pair("あい", 2));
    kvs.push_back(make_pair("あいう", 3));
    kvs.push_back(make_pair("いう", 4));
    kvs.push_back(make_pair("うえ", 5));
    kvs.push_back(make_pair("うえお", 6));
    kvs.push_back(make_pair("えお", 7));
    kvs.push_back(make_pair("いうえおあ", 8));
    uxm.build(kvs);

    for (int i = 0; i < static_cast<int>(kvs.size()); i++) {
      string str_rev = ustring_to_string(rev(string_to_ustring(kvs[i].first)));
      kvs_rev.push_back(make_pair(str_rev, kvs[i].second));
    }
    uxm_rev.build(kvs_rev);
  }

  void TearDown() {
  }
};

TEST_F(NormalizerUtilityTest, prefixSearch) {
  ustring ustr(string_to_ustring("あいうえお"));
  int matching_pattern_id;
  prefixSearch(ustr, uxm, matching_pattern_id);
  EXPECT_EQ(3, matching_pattern_id); // ("あいう", 3)
}

TEST_F(NormalizerUtilityTest, prefixSearch2) {
  ustring ustr(string_to_ustring("いうえおあいうえお"));
  int matching_pattern_id;
  prefixSearch(ustr, uxm, matching_pattern_id);
  EXPECT_EQ(8, matching_pattern_id); // ("いうえおあ", 8)
}

TEST_F(NormalizerUtilityTest, suffixSearch) {
  ustring ustr(string_to_ustring("あいうえお"));
  int matching_pattern_id;
  suffixSearch(ustr, uxm_rev, matching_pattern_id);
  EXPECT_EQ(6, matching_pattern_id); // ("うえお", 6)
}

TEST_F(NormalizerUtilityTest, suffixSearch2) {
  ustring ustr(string_to_ustring("あいうえおあ"));
  int matching_pattern_id;
  suffixSearch(ustr, uxm_rev, matching_pattern_id);
  EXPECT_EQ(8, matching_pattern_id); // ("いうえおあ", 8)
}

TEST_F(NormalizerUtilityTest, extract_after_string) {
  ustring text(string_to_ustring("それは秒速5センチメートルくらいで進む"));
  ustring str;
  extract_after_string(text, 6, str);
  EXPECT_EQ("センチメートルくらいで進む", ustring_to_string(str));
}

TEST_F(NormalizerUtilityTest, extract_before_string) {
  ustring text(string_to_ustring("それは秒速5センチメートルくらいで進む"));
  ustring str;
  extract_before_string(text, 5, str);
  EXPECT_EQ("それは秒速", ustring_to_string(str));
}

TEST_F(NormalizerUtilityTest, seach_suffix) {
  ustring text(string_to_ustring("あいうえおあ5あいうえおごごごごご"));
  int matching_pattern_id;
  search_suffix_number_modifier(text, 7, uxm, matching_pattern_id);
  EXPECT_EQ(3, matching_pattern_id);
}

TEST_F(NormalizerUtilityTest, search_prefix) {
  ustring text(string_to_ustring("あいうえおあ5あいうえおごごごごご"));
  int matching_pattern_id;
  search_prefix_number_modifier(text, 6, uxm_rev, matching_pattern_id);
  EXPECT_EQ(8, matching_pattern_id);
}

TEST_F(NormalizerUtilityTest, replace_numbers_in_text) {
  ustring text(string_to_ustring("その30人がそれは三十五人でボボボ")), text_replaced;
  vector<Number> numbers;
  ustring exp1(string_to_ustring("30人")), exp2(string_to_ustring("三十五人"));
  numbers.push_back(Number(exp1, 2, 4));
  numbers.push_back(Number(exp2, 9, 12));
  replace_numbers_in_text(text, numbers, text_replaced);
  EXPECT_EQ(string_to_ustring("そのǂǂ人がそれはǂǂǂ人でボボボ") , text_replaced);
}

TEST_F(NormalizerUtilityTest, shorten_place_holder_in_text) {
  ustring text(string_to_ustring("そのǂǂ人がそれはǂǂǂǂǂǂ人でボボボǂǂǂ")), text_shortened;
  shorten_place_holder_in_text(text, text_shortened);
  EXPECT_EQ(string_to_ustring("そのǂ人がそれはǂ人でボボボǂ") , text_shortened);
}

TEST_F(NormalizerUtilityTest, is_place_holder_true) {
  EXPECT_TRUE(is_place_holder(string_to_uchar("ǂ")));
}

TEST_F(NormalizerUtilityTest, is_place_holder_false) {
  EXPECT_FALSE(is_place_holder(string_to_uchar("あ")));
}

TEST_F(NormalizerUtilityTest, is_finite_false) {
  EXPECT_FALSE(is_finite(INFINITY));
}

TEST_F(NormalizerUtilityTest, is_finite_true) {
  EXPECT_TRUE(is_finite(99999.0));
}

TEST_F(NormalizerUtilityTest, is_null_time_true) {
  Time t(INFINITY);
  EXPECT_TRUE(is_null_time(t));
}

TEST_F(NormalizerUtilityTest, is_null_time_false) {
  Time t(1);
  EXPECT_FALSE(is_null_time(t));
}

TEST_F(NormalizerUtilityTest, identify_time_detail) {
  Time t(1,1,1,1,1,INFINITY);
  EXPECT_EQ(identify_time_detail(t), "mn");
}

TEST_F(NormalizerUtilityTest, reverse_string) {
  string str("aiueo");
  EXPECT_EQ(reverse_string(str), "oeuia");
}
#include <gtest/gtest.h>
#include <string>
#include "normalizer_utility.hpp"
#include "reltime_expression_normalizer.hpp"

#include <pficommon/data/string/ustring.h>
#include <ux/ux.hpp>

using namespace normalizer_utility;
using namespace std;
using namespace pfi::data::string;
using namespace reltime_expression_normalizer;
class ReltimeexpNormalizerTest : public testing::Test {
public:
  void SetUp() {}
  void TearDown() {}
};

bool is_same_time(const Time& a, const Time& b){
  return
  a.year == b.year &&
  a.month == b.month &&
  a.day == b.day &&
  a.hour == b.hour &&
  a.minute == b.minute &&
  a.second == b.second;
}

TEST_F(ReltimeexpNormalizerTest, simple1) {
  ReltimeExpressionNormalizer REN("ja");
  std::string text("あの人は三時間前に生まれた");
  std::vector<ReltimeExpression> reltimeexps;
  REN.process(text, reltimeexps);
  ASSERT_EQ(1u, reltimeexps.size());
  
  Time ex1_lower_abs(INFINITY, INFINITY, INFINITY, INFINITY, INFINITY, INFINITY);
  Time ex1_upper_abs(-INFINITY, -INFINITY, -INFINITY, -INFINITY, -INFINITY, -INFINITY);
  Time ex1_lower_rel(INFINITY, INFINITY, INFINITY, -3, INFINITY, INFINITY);
  Time ex1_upper_rel(-INFINITY, -INFINITY, -INFINITY, -3, -INFINITY, -INFINITY);
  
  EXPECT_EQ("三時間前", ustring_to_string(reltimeexps[0].original_expression));
  EXPECT_TRUE(is_same_time(ex1_lower_abs, reltimeexps[0].value_lowerbound_abs));
  EXPECT_TRUE(is_same_time(ex1_upper_abs, reltimeexps[0].value_upperbound_abs));
  EXPECT_TRUE(is_same_time(ex1_lower_rel, reltimeexps[0].value_lowerbound_rel));
  EXPECT_TRUE(is_same_time(ex1_upper_rel, reltimeexps[0].value_upperbound_rel));
}

TEST_F(ReltimeexpNormalizerTest, simple2) {
  ReltimeExpressionNormalizer REN("ja");
  std::string text("それは3年5ヶ月後の出来事");
  std::vector<ReltimeExpression> reltimeexps;
  REN.process(text, reltimeexps);
  ASSERT_EQ(1u, reltimeexps.size());
  
  Time ex1_lower_abs(INFINITY, INFINITY, INFINITY, INFINITY, INFINITY, INFINITY);
  Time ex1_upper_abs(-INFINITY, -INFINITY, -INFINITY, -INFINITY, -INFINITY, -INFINITY);
  Time ex1_lower_rel(3, 5, INFINITY, INFINITY, INFINITY, INFINITY);
  Time ex1_upper_rel(3, 5, -INFINITY, -INFINITY, -INFINITY, -INFINITY);
  
  EXPECT_EQ("3年5ヶ月後", ustring_to_string(reltimeexps[0].original_expression));
  EXPECT_TRUE(is_same_time(ex1_lower_abs, reltimeexps[0].value_lowerbound_abs));
  EXPECT_TRUE(is_same_time(ex1_upper_abs, reltimeexps[0].value_upperbound_abs));
  EXPECT_TRUE(is_same_time(ex1_lower_rel, reltimeexps[0].value_lowerbound_rel));
  EXPECT_TRUE(is_same_time(ex1_upper_rel, reltimeexps[0].value_upperbound_rel));
}

TEST_F(ReltimeexpNormalizerTest, plural1) {
  ReltimeExpressionNormalizer REN("ja");
  std::string text("話をしよう。あれは今から36万年前………いや、1万4000年前だったか。");
  std::vector<ReltimeExpression> reltimeexps;
  REN.process(text, reltimeexps);
  ASSERT_EQ(2u, reltimeexps.size());
  
  Time ex1_lower_abs(INFINITY, INFINITY, INFINITY, INFINITY, INFINITY, INFINITY);
  Time ex1_upper_abs(-INFINITY, -INFINITY, -INFINITY, -INFINITY, -INFINITY, -INFINITY);
  Time ex1_lower_rel(-360000, INFINITY, INFINITY, INFINITY, INFINITY, INFINITY);
  Time ex1_upper_rel(-360000, -INFINITY, -INFINITY, -INFINITY, -INFINITY, -INFINITY);
  
  Time ex2_lower_abs(INFINITY, INFINITY, INFINITY, INFINITY, INFINITY, INFINITY);
  Time ex2_upper_abs(-INFINITY, -INFINITY, -INFINITY, -INFINITY, -INFINITY, -INFINITY);
  Time ex2_lower_rel(-14000, INFINITY, INFINITY, INFINITY, INFINITY, INFINITY);
  Time ex2_upper_rel(-14000, -INFINITY, -INFINITY, -INFINITY, -INFINITY, -INFINITY);
  
  EXPECT_EQ("から36万年前", ustring_to_string(reltimeexps[0].original_expression)); //TODO : 「から」を範囲表現として辞書にいれると、こうなってしまう。今後の課題
  EXPECT_TRUE(is_same_time(ex1_lower_abs, reltimeexps[0].value_lowerbound_abs));
  EXPECT_TRUE(is_same_time(ex1_upper_abs, reltimeexps[0].value_upperbound_abs));
  EXPECT_TRUE(is_same_time(ex1_lower_rel, reltimeexps[0].value_lowerbound_rel));
  EXPECT_TRUE(is_same_time(ex1_upper_rel, reltimeexps[0].value_upperbound_rel));
  
  EXPECT_EQ("1万4000年前", ustring_to_string(reltimeexps[1].original_expression));
  EXPECT_TRUE(is_same_time(ex2_lower_abs, reltimeexps[1].value_lowerbound_abs));
  EXPECT_TRUE(is_same_time(ex2_upper_abs, reltimeexps[1].value_upperbound_abs));
  EXPECT_TRUE(is_same_time(ex2_lower_rel, reltimeexps[1].value_lowerbound_rel));
  EXPECT_TRUE(is_same_time(ex2_upper_rel, reltimeexps[1].value_upperbound_rel));
}

TEST_F(ReltimeexpNormalizerTest, suffix_range1) {
  //以下のような「から」の扱いは、正直微妙なところ。
  //「から」は時間の起点を表しているだけで、時間の範囲を指定しているわけではなく、時間表現の一部とみなす必要はないかもしれない。
  //が、起点が示される＝範囲っぽい(?)と捉えることもできるので、ひとまず認識させて、この後の処理で扱い方を委ねることにする。
  ReltimeExpressionNormalizer REN("ja");
  std::string text("1万年と2千年前から愛してる");
  std::vector<ReltimeExpression> reltimeexps;
  REN.process(text, reltimeexps);
  ASSERT_EQ(1u, reltimeexps.size());
  
  //「一万年」は相対時間表現ではない。
  
  Time ex2_lower_abs(INFINITY, INFINITY, INFINITY, INFINITY, INFINITY, INFINITY);
  Time ex2_upper_abs(-INFINITY, -INFINITY, -INFINITY, -INFINITY, -INFINITY, -INFINITY);
  Time ex2_lower_rel(-2000, INFINITY, INFINITY, INFINITY, INFINITY, INFINITY);
  Time ex2_upper_rel(-2000, -INFINITY, -INFINITY, -INFINITY, -INFINITY, -INFINITY);
  
  EXPECT_EQ("2千年前から", ustring_to_string(reltimeexps[0].original_expression));
  EXPECT_TRUE(is_same_time(ex2_lower_abs, reltimeexps[0].value_lowerbound_abs));
  EXPECT_TRUE(is_same_time(ex2_upper_abs, reltimeexps[0].value_upperbound_abs));
  EXPECT_TRUE(is_same_time(ex2_lower_rel, reltimeexps[0].value_lowerbound_rel));
  EXPECT_TRUE(is_same_time(ex2_upper_rel, reltimeexps[0].value_upperbound_rel));
  ASSERT_EQ(1u, reltimeexps[0].options.size());
  EXPECT_EQ(reltimeexps[0].options[0], "kara_suffix");
}

TEST_F(ReltimeexpNormalizerTest, range1) {
  ReltimeExpressionNormalizer REN("ja");
  std::string text("1万2千年前から1億2千年後まで愛してる");
  std::vector<ReltimeExpression> reltimeexps;
  REN.process(text, reltimeexps);
  ASSERT_EQ(1u, reltimeexps.size());
  
  Time ex_lower_abs(INFINITY, INFINITY, INFINITY, INFINITY, INFINITY, INFINITY);
  Time ex_upper_abs(-INFINITY, -INFINITY, -INFINITY, -INFINITY, -INFINITY, -INFINITY);
  Time ex_lower_rel(-12000, INFINITY, INFINITY, INFINITY, INFINITY, INFINITY);
  Time ex_upper_rel(100002000.0, -INFINITY, -INFINITY, -INFINITY, -INFINITY, -INFINITY);
  EXPECT_EQ("1万2千年前から1億2千年後まで", ustring_to_string(reltimeexps[0].original_expression));
  EXPECT_TRUE(is_same_time(ex_lower_abs, reltimeexps[0].value_lowerbound_abs));
  EXPECT_TRUE(is_same_time(ex_upper_abs, reltimeexps[0].value_upperbound_abs));
  EXPECT_TRUE(is_same_time(ex_lower_rel, reltimeexps[0].value_lowerbound_rel));
  EXPECT_TRUE(is_same_time(ex_upper_rel, reltimeexps[0].value_upperbound_rel));
}

TEST_F(ReltimeexpNormalizerTest, seiki1) {
  ReltimeExpressionNormalizer REN("ja");
  std::string text("二世紀前に起こった悲劇");
  std::vector<ReltimeExpression> reltimeexps;
  REN.process(text, reltimeexps);
  ASSERT_EQ(1u, reltimeexps.size());
  
  Time ex1_lower_abs(INFINITY, INFINITY, INFINITY, INFINITY, INFINITY, INFINITY);
  Time ex1_upper_abs(-INFINITY, -INFINITY, -INFINITY, -INFINITY, -INFINITY, -INFINITY);
  Time ex1_lower_rel(-200, INFINITY, INFINITY, INFINITY, INFINITY, INFINITY);
  Time ex1_upper_rel(-200, -INFINITY, -INFINITY, -INFINITY, -INFINITY, -INFINITY);
  
  EXPECT_EQ("二世紀前", ustring_to_string(reltimeexps[0].original_expression));
  EXPECT_TRUE(is_same_time(ex1_lower_abs, reltimeexps[0].value_lowerbound_abs));
  EXPECT_TRUE(is_same_time(ex1_upper_abs, reltimeexps[0].value_upperbound_abs));
  EXPECT_TRUE(is_same_time(ex1_lower_rel, reltimeexps[0].value_lowerbound_rel));
  EXPECT_TRUE(is_same_time(ex1_upper_rel, reltimeexps[0].value_upperbound_rel));
}

TEST_F(ReltimeexpNormalizerTest, han1) {
  ReltimeExpressionNormalizer REN("ja");
  std::string text("二世紀半前に起こった悲劇");
  std::vector<ReltimeExpression> reltimeexps;
  REN.process(text, reltimeexps);
  ASSERT_EQ(1u, reltimeexps.size());
  
  Time ex1_lower_abs(INFINITY, INFINITY, INFINITY, INFINITY, INFINITY, INFINITY);
  Time ex1_upper_abs(-INFINITY, -INFINITY, -INFINITY, -INFINITY, -INFINITY, -INFINITY);
  Time ex1_lower_rel(-250, INFINITY, INFINITY, INFINITY, INFINITY, INFINITY);
  Time ex1_upper_rel(-250, -INFINITY, -INFINITY, -INFINITY, -INFINITY, -INFINITY);
  
  EXPECT_EQ("二世紀半前", ustring_to_string(reltimeexps[0].original_expression));
  EXPECT_TRUE(is_same_time(ex1_lower_abs, reltimeexps[0].value_lowerbound_abs));
  EXPECT_TRUE(is_same_time(ex1_upper_abs, reltimeexps[0].value_upperbound_abs));
  EXPECT_TRUE(is_same_time(ex1_lower_rel, reltimeexps[0].value_lowerbound_rel));
  EXPECT_TRUE(is_same_time(ex1_upper_rel, reltimeexps[0].value_upperbound_rel));
}

TEST_F(ReltimeexpNormalizerTest, han2) {
  ReltimeExpressionNormalizer REN("ja");
  std::string text("あの人は三時間半前に生まれた");
  std::vector<ReltimeExpression> reltimeexps;
  REN.process(text, reltimeexps);
  ASSERT_EQ(1u, reltimeexps.size());
  
  Time ex1_lower_abs(INFINITY, INFINITY, INFINITY, INFINITY, INFINITY, INFINITY);
  Time ex1_upper_abs(-INFINITY, -INFINITY, -INFINITY, -INFINITY, -INFINITY, -INFINITY);
  Time ex1_lower_rel(INFINITY, INFINITY, INFINITY, -3.5, INFINITY, INFINITY);
  Time ex1_upper_rel(-INFINITY, -INFINITY, -INFINITY, -3.5, -INFINITY, -INFINITY);
  
  EXPECT_EQ("三時間半前", ustring_to_string(reltimeexps[0].original_expression));
  EXPECT_TRUE(is_same_time(ex1_lower_abs, reltimeexps[0].value_lowerbound_abs));
  EXPECT_TRUE(is_same_time(ex1_upper_abs, reltimeexps[0].value_upperbound_abs));
  EXPECT_TRUE(is_same_time(ex1_lower_rel, reltimeexps[0].value_lowerbound_rel));
  EXPECT_TRUE(is_same_time(ex1_upper_rel, reltimeexps[0].value_upperbound_rel));
}

TEST_F(ReltimeexpNormalizerTest, prefix_counter1) {
  ReltimeExpressionNormalizer REN("ja");
  std::string text("それは昨年4月の出来事");
  std::vector<ReltimeExpression> reltimeexps;
  REN.process(text, reltimeexps);
  ASSERT_EQ(1u, reltimeexps.size());
  Time ex1_lower_abs(INFINITY, 4, INFINITY, INFINITY, INFINITY, INFINITY);
  Time ex1_upper_abs(-INFINITY, 4, -INFINITY, -INFINITY, -INFINITY, -INFINITY);
  Time ex1_lower_rel(-1, INFINITY, INFINITY, INFINITY, INFINITY, INFINITY);
  Time ex1_upper_rel(-1, -INFINITY, -INFINITY, -INFINITY, -INFINITY, -INFINITY);
  
  EXPECT_EQ("昨年4月", ustring_to_string(reltimeexps[0].original_expression));
  EXPECT_TRUE(is_same_time(ex1_lower_abs, reltimeexps[0].value_lowerbound_abs));
  EXPECT_TRUE(is_same_time(ex1_upper_abs, reltimeexps[0].value_upperbound_abs));
  EXPECT_TRUE(is_same_time(ex1_lower_rel, reltimeexps[0].value_lowerbound_rel));
  EXPECT_TRUE(is_same_time(ex1_upper_rel, reltimeexps[0].value_upperbound_rel));
}

TEST_F(ReltimeexpNormalizerTest, prefix_counter2) {
  ReltimeExpressionNormalizer REN("ja");
  std::string text("来月三日にはできます");
  std::vector<ReltimeExpression> reltimeexps;
  REN.process(text, reltimeexps);
  ASSERT_EQ(1u, reltimeexps.size());
  Time ex1_lower_abs(INFINITY, INFINITY, 3, INFINITY, INFINITY, INFINITY);
  Time ex1_upper_abs(-INFINITY, -INFINITY, 3, -INFINITY, -INFINITY, -INFINITY);
  Time ex1_lower_rel(INFINITY, 1, INFINITY, INFINITY, INFINITY, INFINITY);
  Time ex1_upper_rel(-INFINITY, 1, -INFINITY, -INFINITY, -INFINITY, -INFINITY);
  
  EXPECT_EQ("来月三日", ustring_to_string(reltimeexps[0].original_expression));
  EXPECT_TRUE(is_same_time(ex1_lower_abs, reltimeexps[0].value_lowerbound_abs));
  EXPECT_TRUE(is_same_time(ex1_upper_abs, reltimeexps[0].value_upperbound_abs));
  EXPECT_TRUE(is_same_time(ex1_lower_rel, reltimeexps[0].value_lowerbound_rel));
  EXPECT_TRUE(is_same_time(ex1_upper_rel, reltimeexps[0].value_upperbound_rel));
}

/*
//これは持続時間表現とした。
TEST_F(ReltimeexpNormalizerTest, inai1) {
  ReltimeExpressionNormalizer REN("ja");
  std::string text("あの人なら三時間以内で仕上げるでしょう。");
  std::vector<ReltimeExpression> reltimeexps;
  REN.process(text, reltimeexps);
  ASSERT_EQ(1u, reltimeexps.size());
  
  Time ex1_lower_abs(INFINITY, INFINITY, INFINITY, INFINITY, INFINITY, INFINITY);
  Time ex1_upper_abs(-INFINITY, -INFINITY, -INFINITY, -INFINITY, -INFINITY, -INFINITY);
  Time ex1_lower_rel(0, 0, 0, 0, 0, 0); // TODO : 「以内」のときの表記法を定める。
  Time ex1_upper_rel(-INFINITY, -INFINITY, -INFINITY, 3, -INFINITY, -INFINITY);
  
  EXPECT_EQ("三時間以内", ustring_to_string(reltimeexps[0].original_expression));
  EXPECT_TRUE(is_same_time(ex1_lower_abs, reltimeexps[0].value_lowerbound_abs));
  EXPECT_TRUE(is_same_time(ex1_upper_abs, reltimeexps[0].value_upperbound_abs));
  EXPECT_TRUE(is_same_time(ex1_lower_rel, reltimeexps[0].value_lowerbound_rel));
  EXPECT_TRUE(is_same_time(ex1_upper_rel, reltimeexps[0].value_upperbound_rel));
}
*/

TEST_F(ReltimeexpNormalizerTest, abstime_expression) {
  ReltimeExpressionNormalizer REN("ja");
  std::string text("それは4月の出来事");
  std::vector<ReltimeExpression> reltimeexps;
  REN.process(text, reltimeexps);
  ASSERT_EQ(0u, reltimeexps.size());
}

TEST_F(ReltimeexpNormalizerTest, about1) {
  ReltimeExpressionNormalizer REN("ja");
  std::string text("およそ1000年前");
  std::vector<ReltimeExpression> reltimeexps;
  REN.process(text, reltimeexps);
  ASSERT_EQ(1u, reltimeexps.size());
  Time ex1_lower_abs(INFINITY, INFINITY, INFINITY, INFINITY, INFINITY, INFINITY);
  Time ex1_upper_abs(-INFINITY, -INFINITY, -INFINITY, -INFINITY, -INFINITY, -INFINITY);
  //Time ex1_lower_rel(-1005, INFINITY, INFINITY, INFINITY, INFINITY, INFINITY);
  //Time ex1_upper_rel(-995, -INFINITY, -INFINITY, -INFINITY, -INFINITY, -INFINITY);
  Time ex1_lower_rel(-1000, INFINITY, INFINITY, INFINITY, INFINITY, INFINITY);
  Time ex1_upper_rel(-1000, -INFINITY, -INFINITY, -INFINITY, -INFINITY, -INFINITY);
  EXPECT_EQ("およそ1000年前", ustring_to_string(reltimeexps[0].original_expression));
  EXPECT_TRUE(is_same_time(ex1_lower_abs, reltimeexps[0].value_lowerbound_abs));
  EXPECT_TRUE(is_same_time(ex1_upper_abs, reltimeexps[0].value_upperbound_abs));
  EXPECT_TRUE(is_same_time(ex1_lower_rel, reltimeexps[0].value_lowerbound_rel));
  EXPECT_TRUE(is_same_time(ex1_upper_rel, reltimeexps[0].value_upperbound_rel));
}

TEST_F(ReltimeexpNormalizerTest, about2) {
  ReltimeExpressionNormalizer REN("ja");
  std::string text("約3ヶ月後");
  std::vector<ReltimeExpression> reltimeexps;
  REN.process(text, reltimeexps);
  ASSERT_EQ(1u, reltimeexps.size());
  Time ex1_lower_abs(INFINITY, INFINITY, INFINITY, INFINITY, INFINITY, INFINITY);
  Time ex1_upper_abs(-INFINITY, -INFINITY, -INFINITY, -INFINITY, -INFINITY, -INFINITY);
  //Time ex1_lower_rel(INFINITY, 2, INFINITY, INFINITY, INFINITY, INFINITY);
  //Time ex1_upper_rel(-INFINITY, 4, -INFINITY, -INFINITY, -INFINITY, -INFINITY);
  Time ex1_lower_rel(INFINITY, 3, INFINITY, INFINITY, INFINITY, INFINITY);
  Time ex1_upper_rel(-INFINITY, 3, -INFINITY, -INFINITY, -INFINITY, -INFINITY);
  EXPECT_EQ("約3ヶ月後", ustring_to_string(reltimeexps[0].original_expression));
  EXPECT_TRUE(is_same_time(ex1_lower_abs, reltimeexps[0].value_lowerbound_abs));
  EXPECT_TRUE(is_same_time(ex1_upper_abs, reltimeexps[0].value_upperbound_abs));
  EXPECT_TRUE(is_same_time(ex1_lower_rel, reltimeexps[0].value_lowerbound_rel));
  EXPECT_TRUE(is_same_time(ex1_upper_rel, reltimeexps[0].value_upperbound_rel));
}
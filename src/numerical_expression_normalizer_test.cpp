#include <gtest/gtest.h>
#include <string>
#include <fstream>
#include "normalizer_utility.hpp"
#include "numerical_expression_normalizer.hpp"

#include <pficommon/data/string/ustring.h>
#include <ux/ux.hpp>

using namespace normalizer_utility;
using namespace std;
using namespace pfi::data::string;
using namespace numerical_expression_normalizer;
class NumexpNormalizerTest : public testing::Test {
public:
    void SetUp() {}
    void TearDown() {}
};

bool is_same_numexp(const NumericalExpression &n1, const NumericalExpression &n2){
  return
    n1.original_expression==n2.original_expression &&
    n1.position_start==n2.position_start &&
    n1.position_end==n2.position_end &&
    n1.value_lowerbound==n2.value_lowerbound &&
    n1.value_upperbound==n2.value_upperbound &&
    n1.counter==n2.counter;
}

TEST_F(NumexpNormalizerTest, simple1) {
  NumericalExpressionNormalizer NEN("ja");
  std::string text("その三人が死んだ");
  std::vector<NumericalExpression> numexps;
  NEN.process(text, numexps);
  NumericalExpression ex(string_to_ustring("三人"), 2, 4, 3, 3);
  ex.counter = string_to_ustring("人");
  
  EXPECT_TRUE(is_same_numexp(ex, numexps[0]));
}

TEST_F(NumexpNormalizerTest, simple2) {
  NumericalExpressionNormalizer NEN("ja");
  std::string text("3kgのレッドブルと、2USドルのモンスター");
  std::vector<NumericalExpression> numexps;
  NEN.process(text, numexps);
  NumericalExpression ex1(string_to_ustring("3kg"), 0, 3, 3000, 3000);
  ex1.counter = string_to_ustring("g");
  NumericalExpression ex2(string_to_ustring("2USドル"),11, 16, 2, 2);
  ex2.counter = string_to_ustring("ドル");
  EXPECT_TRUE(is_same_numexp(ex1, numexps[0]));
  EXPECT_TRUE(is_same_numexp(ex2, numexps[1]));
}

TEST_F(NumexpNormalizerTest, about1) {
  NumericalExpressionNormalizer NEN("ja");
  std::string text("その約十人がぼぼぼぼ");
  std::vector<NumericalExpression> numexps;
  NEN.process(text, numexps);
  NumericalExpression ex(string_to_ustring("約十人"), 2, 5, 7, 13.0);
  ex.counter = string_to_ustring("人");
  EXPECT_TRUE(is_same_numexp(ex, numexps[0]));
}

TEST_F(NumexpNormalizerTest, about2) {
  NumericalExpressionNormalizer NEN("ja");
  std::string text("そのおよそ十人がぼぼぼぼ");
  std::vector<NumericalExpression> numexps;
  NEN.process(text, numexps);
  NumericalExpression ex(string_to_ustring("およそ十人"), 2, 7, 7, 13.0);
  ex.counter = string_to_ustring("人");
  EXPECT_TRUE(is_same_numexp(ex, numexps[0]));
}

TEST_F(NumexpNormalizerTest, or_over) {
  NumericalExpressionNormalizer NEN("ja");
  std::string text("その三人以上がぼぼぼぼ");
  std::vector<NumericalExpression> numexps;
  NEN.process(text, numexps);
  NumericalExpression ex(string_to_ustring("三人以上"), 2, 6, 3.0, INFINITY);
  ex.counter = string_to_ustring("人");
  EXPECT_TRUE(is_same_numexp(ex, numexps[0]));
}

TEST_F(NumexpNormalizerTest, about_and_or_over) {
  NumericalExpressionNormalizer NEN("ja");
  std::string text("その約十人以上がぼぼぼぼ");
  std::vector<NumericalExpression> numexps;
  NEN.process(text, numexps);
  NumericalExpression ex(string_to_ustring("約十人以上"), 2, 7, 7.0, INFINITY);
  ex.counter = string_to_ustring("人");
  EXPECT_TRUE(is_same_numexp(ex, numexps[0]));
}

TEST_F(NumexpNormalizerTest, or_less) {
  NumericalExpressionNormalizer NEN("ja");
  std::string text("その三人以下がぼぼぼぼ");
  std::vector<NumericalExpression> numexps;
  NEN.process(text, numexps);
  NumericalExpression ex(string_to_ustring("三人以下"), 2, 6, -INFINITY, 3);
  ex.counter = string_to_ustring("人");
  EXPECT_TRUE(is_same_numexp(ex, numexps[0]));
}

TEST_F(NumexpNormalizerTest, kyou) {
  NumericalExpressionNormalizer NEN("ja");
  std::string text("レッドブルを10本強飲んだ");
  std::vector<NumericalExpression> numexps;
  NEN.process(text, numexps);
  NumericalExpression ex(string_to_ustring("10本強"), 6, 10, 10, 16);
  ex.counter = string_to_ustring("本");
  EXPECT_TRUE(is_same_numexp(ex, numexps[0]));
}

TEST_F(NumexpNormalizerTest, jaku) {
  NumericalExpressionNormalizer NEN("ja");
  std::string text("レッドブルを10本弱飲んだ");
  std::vector<NumericalExpression> numexps;
  NEN.process(text, numexps);
  NumericalExpression ex(string_to_ustring("10本弱"), 6, 10, 5, 10);
  ex.counter = string_to_ustring("本");
  EXPECT_TRUE(is_same_numexp(ex, numexps[0]));
}

TEST_F(NumexpNormalizerTest, ordinary) {
  NumericalExpressionNormalizer NEN("ja");
  std::string text("本日10本目のレッドブル");
  std::vector<NumericalExpression> numexps;
  NEN.process(text, numexps);
  NumericalExpression ex(string_to_ustring("10本目"), 2, 6, 10, 10);
  ex.counter = string_to_ustring("本");
  ex.ordinary = true;
  EXPECT_TRUE(is_same_numexp(ex, numexps[0]));
}

TEST_F(NumexpNormalizerTest, han) {
  NumericalExpressionNormalizer NEN("ja");
  std::string text("レッドブルを1本半飲んだ");
  std::vector<NumericalExpression> numexps;
  NEN.process(text, numexps);
  NumericalExpression ex(string_to_ustring("1本半"), 6, 9, 1.5, 1.5);
  ex.counter = string_to_ustring("本");
  EXPECT_TRUE(is_same_numexp(ex, numexps[0]));
}

TEST_F(NumexpNormalizerTest, per) {
  NumericalExpressionNormalizer NEN("ja");
  std::string text("１キロメートル／時");
  std::vector<NumericalExpression> numexps;
  NEN.process(text, numexps);
  NumericalExpression ex(string_to_ustring("１キロメートル／時"), 0, 9, 1000, 1000);
  ex.counter = string_to_ustring("m/h");
  EXPECT_TRUE(is_same_numexp(ex, numexps[0]));
}

TEST_F(NumexpNormalizerTest, prefix_counter1) {
  NumericalExpressionNormalizer NEN("ja");
  std::string text("それは¥100だ");
  std::vector<NumericalExpression> numexps;
  NEN.process(text, numexps);
  NumericalExpression ex(string_to_ustring("¥100"), 3, 7, 100, 100);
  ex.counter = string_to_ustring("円");
  ASSERT_EQ(1u, numexps.size());

  EXPECT_TRUE(is_same_numexp(ex, numexps[0]));
}

TEST_F(NumexpNormalizerTest, prefix_counter2) {
  NumericalExpressionNormalizer NEN("ja");
  std::string text("それは時速40キロメートルだ");
  std::vector<NumericalExpression> numexps;
  NEN.process(text, numexps);
  NumericalExpression ex(string_to_ustring("時速40キロメートル"), 3, 13, 40000, 40000);
  ex.counter = string_to_ustring("m/h");
  ASSERT_EQ(1u, numexps.size());
  
  EXPECT_TRUE(is_same_numexp(ex, numexps[0]));
}

TEST_F(NumexpNormalizerTest, range1) {
  NumericalExpressionNormalizer NEN("ja");
  std::string text("このアトラクションは3人〜の運用になります");
  std::vector<NumericalExpression> numexps;
  NEN.process(text, numexps);
  NumericalExpression ex(string_to_ustring("3人〜"), 10, 13, 3, 3);
  ex.counter = string_to_ustring("人");
  ASSERT_EQ(1u, numexps.size());
  EXPECT_TRUE(is_same_numexp(ex, numexps[0]));
  ASSERT_EQ(1u, numexps[0].options.size());
  EXPECT_EQ(numexps[0].options[0], "kara_suffix");
}

TEST_F(NumexpNormalizerTest, range2) {
  NumericalExpressionNormalizer NEN("ja");
  std::string text("遊び方の欄には「〜8人」と書いてある");
  std::vector<NumericalExpression> numexps;
  NEN.process(text, numexps);
  NumericalExpression ex(string_to_ustring("〜8人"), 8, 11, 8, 8);
  ex.counter = string_to_ustring("人");
  ASSERT_EQ(1u, numexps.size());
  EXPECT_TRUE(is_same_numexp(ex, numexps[0]));
  ASSERT_EQ(1u, numexps[0].options.size());
  EXPECT_EQ(numexps[0].options[0], "kara_prefix");
}

TEST_F(NumexpNormalizerTest, range3) {
  NumericalExpressionNormalizer NEN("ja");
  std::string text("遊び方の欄には「5〜8人」と書いてある");
  std::vector<NumericalExpression> numexps;
  NEN.process(text, numexps);
  NumericalExpression ex(string_to_ustring("5〜8人"), 8, 12, 5, 8);
  ex.counter = string_to_ustring("人");
  ASSERT_EQ(1u, numexps.size());
  EXPECT_TRUE(is_same_numexp(ex, numexps[0]));
}

TEST_F(NumexpNormalizerTest, range4) {
  NumericalExpressionNormalizer NEN("ja");
  std::string text("遊び方の欄には「5人〜8人」と書いてある");
  std::vector<NumericalExpression> numexps;
  NEN.process(text, numexps);
  NumericalExpression ex(string_to_ustring("5人〜8人"), 8, 13, 5, 8);
  ex.counter = string_to_ustring("人");
  ASSERT_EQ(1u, numexps.size());
  
  EXPECT_TRUE(is_same_numexp(ex, numexps[0]));
}

TEST_F(NumexpNormalizerTest, range5) {
  NumericalExpressionNormalizer NEN("ja");
  std::string text("時速50km〜60km");
  std::vector<NumericalExpression> numexps;
  NEN.process(text, numexps);
  NumericalExpression ex(string_to_ustring("時速50km〜60km"), 0, 11, 50000, 60000);
  ex.counter = string_to_ustring("m/h");
  ASSERT_EQ(1u, numexps.size());
  
  EXPECT_TRUE(is_same_numexp(ex, numexps[0]));
}

TEST_F(NumexpNormalizerTest, range6) {
  NumericalExpressionNormalizer NEN("ja");
  std::string text("時速50kmから時速60km");
  std::vector<NumericalExpression> numexps;
  NEN.process(text, numexps);
  NumericalExpression ex(string_to_ustring("時速50kmから時速60km"), 0, 14, 50000, 60000);
  ex.counter = string_to_ustring("m/h");
  ASSERT_EQ(1u, numexps.size());
  
  EXPECT_TRUE(is_same_numexp(ex, numexps[0]));
}

TEST_F(NumexpNormalizerTest, range7) {
  NumericalExpressionNormalizer NEN("ja");
  std::string text("時速50〜60km");
  std::vector<NumericalExpression> numexps;
  NEN.process(text, numexps);
  NumericalExpression ex(string_to_ustring("時速50〜60km"), 0, 9, 50000, 60000);
  ex.counter = string_to_ustring("m/h");
  ASSERT_EQ(1u, numexps.size());
  
  EXPECT_TRUE(is_same_numexp(ex, numexps[0]));
}

TEST_F(NumexpNormalizerTest, range8) {
  NumericalExpressionNormalizer NEN("ja");
  std::string text("世界50カ国から3000人が出席予定だ");
  std::vector<NumericalExpression> numexps;
  NEN.process(text, numexps);
  ASSERT_EQ(2u, numexps.size()); //単位が違うので、マージされない
}

TEST_F(NumexpNormalizerTest, range9) {
  NumericalExpressionNormalizer NEN("ja");
  std::string text("およそ時速50km〜60kmくらい");
  std::vector<NumericalExpression> numexps;
  NEN.process(text, numexps);
  NumericalExpression ex(string_to_ustring("およそ時速50km〜60kmくらい"), 0, 17, 35000, 78000);
  ex.counter = string_to_ustring("m/h");
  ASSERT_EQ(1u, numexps.size());
  EXPECT_TRUE(is_same_numexp(ex, numexps[0]));
}

TEST_F(NumexpNormalizerTest, chinese1) {
  NumericalExpressionNormalizer NEN("zh");
  std::string text("日本政府受清廷壓力，以千二百三元請孫中山離開日本。");
  std::vector<NumericalExpression> numexps;
  NEN.process(text, numexps);
  ASSERT_EQ(1u, numexps.size());
  NumericalExpression ex(string_to_ustring("千二百三元"), 11, 16, 1230, 1230);
  ex.counter = string_to_ustring("元");
  EXPECT_TRUE(is_same_numexp(ex, numexps[0]));

}
//"東京支部の三人"



/*
tests for English
*/
TEST_F(NumexpNormalizerTest, english1) {
	NumericalExpressionNormalizer NEN("en");
	std::string text("He gave $30,000 to his friend at the bank");
	std::vector<NumericalExpression> numexps;
	NEN.process(text, numexps);
	ASSERT_EQ(1u, numexps.size());
	NumericalExpression ex(string_to_ustring("$30,000"), 8, 15, 30000, 30000);
	ex.counter = string_to_ustring("$");
	EXPECT_TRUE(is_same_numexp(ex, numexps[0]));
}

TEST_F(NumexpNormalizerTest, english2) {
	NumericalExpressionNormalizer NEN("en");
	std::string text("This is 30,000,000 kg and about 1,000,000 mm²");
	std::vector<NumericalExpression> numexps;
	NEN.process(text, numexps);
	ASSERT_EQ(2u, numexps.size());
	NumericalExpression ex(string_to_ustring("30,000,000 kg"), 8, 21, 30000000000, 30000000000);
	ex.counter = string_to_ustring("g");
	EXPECT_TRUE(is_same_numexp(ex, numexps[0]));
	NumericalExpression ex2(string_to_ustring("about 1,000,000 mm²"), 26, 45, 0.7, 1.3);
	ex2.counter = string_to_ustring("m2");
	EXPECT_TRUE(is_same_numexp(ex2, numexps[1]));
}
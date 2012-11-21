#include <gtest/gtest.h>
#include <string>
#include "numerical_expression_extractor.hpp"

using namespace numerical_expression_extractor;
using namespace std;

class NumexpExtractorTest : public testing::Test {
public:
    void SetUp() {}
    void TearDown() {}
};

/*
bool is_same_numexp(const NumericalExpression &n1, const NumericalExpression &n2){
  return
    n1.original_expression==n2.original_expression &&
    n1.position_start==n2.position_start &&
    n1.position_end==n2.position_end &&
    n1.value_lowerbound==n2.value_lowerbound &&
    n1.value_upperbound==n2.value_upperbound &&
    n1.counter==n2.counter;
}
*/

TEST_F(NumexpExtractorTest, simple1) {
  vector<string> result; //※結果をどのような形で出力するかは、screwに実装する際に決める（今回は、もともとの表記を出力するだけ）
  string language("ja");
  string text("その100年間において");
  string extract_expression_type("duration");
  NumericalExpressionExtractor NEE(language);
  NEE.extract_numerical_expression(text, extract_expression_type, result);
  ASSERT_EQ(1u, result.size());
  EXPECT_EQ("100年間", result[0]);
}

TEST_F(NumexpExtractorTest, simple2) {
  vector<string> result;
  string language("ja");
  string text("その100年間において");
  string extract_expression_type("absolute_time"); //abstime_normalizerでは、これを「(西暦)100年」として認識してしまう
  NumericalExpressionExtractor NEE(language);
  NEE.extract_numerical_expression(text, extract_expression_type, result);
  EXPECT_EQ(0u, result.size());
}

TEST_F(NumexpExtractorTest, simple3) {
  vector<string> result;
  string language("ja");
  string text("100年前には");
  string extract_expression_type("related_time");
  NumericalExpressionExtractor NEE(language);
  NEE.extract_numerical_expression(text, extract_expression_type, result);
  ASSERT_EQ(1u, result.size());
  EXPECT_EQ("100年前", result[0]);
}

TEST_F(NumexpExtractorTest, simple4) {
  vector<string> result;
  string language("ja");
  string text("100年前には");
  string extract_expression_type("absolute_time"); //abstime_normalizerでは、これを「(西暦)100年」として認識してしまう
  NumericalExpressionExtractor NEE(language);
  NEE.extract_numerical_expression(text, extract_expression_type, result);
  EXPECT_EQ(0u, result.size());
}

TEST_F(NumexpExtractorTest, simple5) {
  vector<string> result;
  string language("ja");
  string text("131.1ポイントというスコアを叩き出した");
  string extract_expression_type("numerical");
  NumericalExpressionExtractor NEE(language);
  NEE.extract_numerical_expression(text, extract_expression_type, result);
  ASSERT_EQ(1u, result.size());
  EXPECT_EQ("131.1ポイント*0*9*ポイント*132", result[0]); //TODO 現在はnumexpのみ、これで返している
}

TEST_F(NumexpExtractorTest, simple6) {
  vector<string> result;
  string language("ja");
  string text("131.1ポイントというスコアを叩き出した");
  string extract_expression_type("absolute_time"); //abstime_normalizerでは、これを「(西暦)131年1月」として認識してしまう（2012.3のような表記方法と勘違いする）
  NumericalExpressionExtractor NEE(language);
  NEE.extract_numerical_expression(text, extract_expression_type, result);
  EXPECT_EQ(0u, result.size());
}

/*
TEST_F(NumexpExtractorTest, real_example1) {
	vector<string> result;
	string language("ja");
	string text("【今日から開催】The Fruits of Adventures @ ZEIT-FOTO SALON(東京・京橋)  4/26(Tue)まで");
	string extract_expression_type("absolute_time");
	NumericalExpressionExtractor NEE(language);
	NEE.extract_numerical_expression(text, extract_expression_type, result);
	EXPECT_EQ(1u, result.size());
}
*/

/*
TEST_F(NumexpExtractorTest, real_example2) {
	vector<string> result;
	string language("ja");
	string text("【今日から開催】The Fruits of Adventures @ ZEIT-FOTO SALON(東京・京橋)  4/26(Tue)まで");
	string extract_expression_type("numerical");
	NumericalExpressionExtractor NEE(language);
	NEE.extract_numerical_expression(text, extract_expression_type, result);
	EXPECT_EQ(0u, result.size());
}
*/





//【今日から開催】The Fruits of Adventures @ ZEIT-FOTO SALON(東京・京橋)  4/26(Tue)まで
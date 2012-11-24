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

TEST_F(NumexpExtractorTest, simple1) {
  vector<string> result;
  string language("ja");
  string text("1911年から2011年の間、その100年間において、1人も死傷者はでなかった。");
  NumericalExpressionExtractor NEE(language);
  NEE.extract_numerical_expression(text, result);
  ASSERT_EQ(3u, result.size());
  EXPECT_EQ("numerical*1人*27*29*人*1*1", result[0]);
  EXPECT_EQ("abstime*1911年から2011年*0*12*none*1911/MM/DD/hh/mm/ss*2011/MM/DD/hh/mm/ss", result[1]);
  EXPECT_EQ("duration*100年間*17*22*none*100/MM/DD/hh/mm/ss*100/MM/DD/hh/mm/ss", result[2]);
  for(int i=0; i<result.size(); i++){
	  cout << result[i] << endl;
  }
}

TEST_F(NumexpExtractorTest, simple2) {
	vector<string> result;
	string language("ja");
	string text("15年前、戦争があった");
	NumericalExpressionExtractor NEE(language);
	NEE.extract_numerical_expression(text, result);
	ASSERT_EQ(1u, result.size());
	EXPECT_EQ("reltime*15年前*0*4*none*YY/MM/DD/hh/mm/ss,-15/MM/DD/hh/mm/ss*YY/MM/DD/hh/mm/ss,-15/MM/DD/hh/mm/ss", result[0]);
	for(int i=0; i<result.size(); i++){
		cout << result[i] << endl;
	}
}

TEST_F(NumexpExtractorTest, simple3) {
	vector<string> result;
	string language("ja");
	string text("昨年3月、僕たち２人は結婚した");
	NumericalExpressionExtractor NEE(language);
	NEE.extract_numerical_expression(text, result);
	ASSERT_EQ(2u, result.size());
	EXPECT_EQ("numerical*２人*8*10*人*2*2", result[0]);
	EXPECT_EQ("reltime*昨年3月*0*4*none*YY/3/DD/hh/mm/ss,-1/MM/DD/hh/mm/ss*YY/3/DD/hh/mm/ss,-1/MM/DD/hh/mm/ss", result[1]);
	for(int i=0; i<result.size(); i++){
		cout << result[i] << endl;
	}
}

TEST_F(NumexpExtractorTest, simple4) {
	vector<string> result;
	string language("ja");
	string text("131.1ポイントというスコアを叩き出した");
	NumericalExpressionExtractor NEE(language);
	NEE.extract_numerical_expression(text, result);
	ASSERT_EQ(1u, result.size());
	EXPECT_EQ("numerical*131.1ポイント*0*9*ポイント*131.1*131.1", result[0]);
	for(int i=0; i<result.size(); i++){
		cout << result[i] << endl;
	}
}

TEST_F(NumexpExtractorTest, real_example1) {
	vector<string> result;
	string language("ja");
	string text("【今日から開催】The Fruits of Adventures @ ZEIT-FOTO SALON(東京・京橋)  4/26(Tue)まで");
	NumericalExpressionExtractor NEE(language);
	NEE.extract_numerical_expression(text, result);
	ASSERT_EQ(1u, result.size());
	EXPECT_EQ("abstime*4/26*59*63*none*YY/4/26/hh/mm/ss*YY/4/26/hh/mm/ss", result[0]);
	for(int i=0; i<result.size(); i++){
		cout << result[i] << endl;
	}
}
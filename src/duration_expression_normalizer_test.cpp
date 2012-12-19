#include <gtest/gtest.h>
#include <string>
#include "normalizer_utility.hpp"
#include "duration_expression_normalizer.hpp"

#include <pficommon/data/string/ustring.h>
#include <ux/ux.hpp>

using namespace normalizer_utility;
using namespace std;
using namespace pfi::data::string;
using namespace duration_expression_normalizer;

class DurationexpNormalizerTest : public testing::Test {
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

TEST_F(DurationexpNormalizerTest, simple1) {
  DurationExpressionNormalizer DEN("ja");
  std::string text("あの人は三時間も耐えた");
  std::vector<DurationExpression> durationexps;
  DEN.process(text, durationexps);
  ASSERT_EQ(1u, durationexps.size());
  
  Time ex1_lower(INFINITY, INFINITY, INFINITY, 3, INFINITY, INFINITY);
  Time ex1_upper(-INFINITY, -INFINITY, -INFINITY, 3, -INFINITY, -INFINITY);
  
  EXPECT_EQ("三時間", ustring_to_string(durationexps[0].original_expression));
  EXPECT_TRUE(is_same_time(ex1_lower, durationexps[0].value_lowerbound));
  EXPECT_TRUE(is_same_time(ex1_upper, durationexps[0].value_upperbound));
}

TEST_F(DurationexpNormalizerTest, simple2) {
  DurationExpressionNormalizer DEN("ja");
  std::string text("それは3年5ヶ月の間にも");
  std::vector<DurationExpression> durationexps;
  DEN.process(text, durationexps);
  ASSERT_EQ(1u, durationexps.size());
  Time ex1_lower(3, 5, INFINITY, INFINITY, INFINITY, INFINITY);
  Time ex1_upper(3, 5, -INFINITY, -INFINITY, -INFINITY, -INFINITY);
  
  EXPECT_EQ("3年5ヶ月", ustring_to_string(durationexps[0].original_expression));
  EXPECT_TRUE(is_same_time(ex1_lower, durationexps[0].value_lowerbound));
  EXPECT_TRUE(is_same_time(ex1_upper, durationexps[0].value_upperbound));
}

TEST_F(DurationexpNormalizerTest, seiki1) {
  DurationExpressionNormalizer DEN("ja");
  std::string text("あの人は三世紀も耐えた");
  std::vector<DurationExpression> durationexps;
  DEN.process(text, durationexps);
  ASSERT_EQ(1u, durationexps.size());
  
  Time ex1_lower(300, INFINITY, INFINITY, INFINITY, INFINITY, INFINITY);
  Time ex1_upper(300, -INFINITY, -INFINITY, -INFINITY, -INFINITY, -INFINITY);
  
  EXPECT_EQ("三世紀", ustring_to_string(durationexps[0].original_expression));
  EXPECT_TRUE(is_same_time(ex1_lower, durationexps[0].value_lowerbound));
  EXPECT_TRUE(is_same_time(ex1_upper, durationexps[0].value_upperbound));
}

TEST_F(DurationexpNormalizerTest, han1) {
  DurationExpressionNormalizer DEN("ja");
  std::string text("あの人は三世紀半も耐えた");
  std::vector<DurationExpression> durationexps;
  DEN.process(text, durationexps);
  ASSERT_EQ(1u, durationexps.size());
  
  Time ex1_lower(350, INFINITY, INFINITY, INFINITY, INFINITY, INFINITY);
  Time ex1_upper(350, -INFINITY, -INFINITY, -INFINITY, -INFINITY, -INFINITY);
  
  EXPECT_EQ("三世紀半", ustring_to_string(durationexps[0].original_expression));
  EXPECT_TRUE(is_same_time(ex1_lower, durationexps[0].value_lowerbound));
  EXPECT_TRUE(is_same_time(ex1_upper, durationexps[0].value_upperbound));
}

TEST_F(DurationexpNormalizerTest, han2) {
  DurationExpressionNormalizer DEN("ja");
  std::string text("あの人は三時間半も耐えた");
  std::vector<DurationExpression> durationexps;
  DEN.process(text, durationexps);
  ASSERT_EQ(1u, durationexps.size());
  
  Time ex1_lower(INFINITY, INFINITY, INFINITY, 3.5, INFINITY, INFINITY);
  Time ex1_upper(-INFINITY, -INFINITY, -INFINITY, 3.5, -INFINITY, -INFINITY);
  
  EXPECT_EQ("三時間半", ustring_to_string(durationexps[0].original_expression));
  EXPECT_TRUE(is_same_time(ex1_lower, durationexps[0].value_lowerbound));
  EXPECT_TRUE(is_same_time(ex1_upper, durationexps[0].value_upperbound));
}

TEST_F(DurationexpNormalizerTest, plural1) {
  DurationExpressionNormalizer DEN("ja");
  std::string text("三年間と五ヶ月の間");
  std::vector<DurationExpression> durationexps;
  DEN.process(text, durationexps);
  ASSERT_EQ(2u, durationexps.size());
  
  Time ex1_lower(3, INFINITY, INFINITY, INFINITY, INFINITY, INFINITY);
  Time ex1_upper(3, -INFINITY, -INFINITY, -INFINITY, -INFINITY, -INFINITY);
  
  EXPECT_EQ("三年間", ustring_to_string(durationexps[0].original_expression));
  EXPECT_TRUE(is_same_time(ex1_lower, durationexps[0].value_lowerbound));
  EXPECT_TRUE(is_same_time(ex1_upper, durationexps[0].value_upperbound));
  
  Time ex2_lower(INFINITY, 5, INFINITY, INFINITY, INFINITY, INFINITY);
  Time ex2_upper(-INFINITY, 5, -INFINITY, -INFINITY, -INFINITY, -INFINITY);
  
  EXPECT_EQ("五ヶ月", ustring_to_string(durationexps[1].original_expression));
  EXPECT_TRUE(is_same_time(ex2_lower, durationexps[1].value_lowerbound));
  EXPECT_TRUE(is_same_time(ex2_upper, durationexps[1].value_upperbound));
}

TEST_F(DurationexpNormalizerTest, or_over1) {
  DurationExpressionNormalizer DEN("ja");
  std::string text("あの人は三時間以上も耐えた");
  std::vector<DurationExpression> durationexps;
  DEN.process(text, durationexps);
  ASSERT_EQ(1u, durationexps.size());
  
  Time ex1_lower(INFINITY, INFINITY, INFINITY, 3, INFINITY, INFINITY);
  Time ex1_upper(INFINITY, INFINITY, INFINITY, INFINITY, INFINITY, INFINITY);
  
  EXPECT_EQ("三時間以上", ustring_to_string(durationexps[0].original_expression));
  EXPECT_TRUE(is_same_time(ex1_lower, durationexps[0].value_lowerbound));
  EXPECT_TRUE(is_same_time(ex1_upper, durationexps[0].value_upperbound));
}

TEST_F(DurationexpNormalizerTest, about_suffix) {
  DurationExpressionNormalizer DEN("ja");
  std::string text("あの人は三時間くらいは耐えた");
  std::vector<DurationExpression> durationexps;
  DEN.process(text, durationexps);
  ASSERT_EQ(1u, durationexps.size());
  
  Time ex1_lower(INFINITY, INFINITY, INFINITY, 2, INFINITY, INFINITY);
  Time ex1_upper(-INFINITY, -INFINITY, -INFINITY, 4, -INFINITY, -INFINITY);
  
  EXPECT_EQ("三時間くらい", ustring_to_string(durationexps[0].original_expression));
  EXPECT_TRUE(is_same_time(ex1_lower, durationexps[0].value_lowerbound));
  EXPECT_TRUE(is_same_time(ex1_upper, durationexps[0].value_upperbound));
}


TEST_F(DurationexpNormalizerTest, about_prefix) {
  DurationExpressionNormalizer DEN("ja");
  std::string text("あの人はほぼ三時間は耐えた");
  std::vector<DurationExpression> durationexps;
  DEN.process(text, durationexps);
  ASSERT_EQ(1u, durationexps.size());
  
  Time ex1_lower(INFINITY, INFINITY, INFINITY, 2, INFINITY, INFINITY);
  Time ex1_upper(-INFINITY, -INFINITY, -INFINITY, 4, -INFINITY, -INFINITY);
  
  EXPECT_EQ("ほぼ三時間", ustring_to_string(durationexps[0].original_expression));
  EXPECT_TRUE(is_same_time(ex1_lower, durationexps[0].value_lowerbound));
  EXPECT_TRUE(is_same_time(ex1_upper, durationexps[0].value_upperbound));
}


TEST_F(DurationexpNormalizerTest, kyou) {
  DurationExpressionNormalizer DEN("ja");
  std::string text("あの人は三時間強は耐えた");
  std::vector<DurationExpression> durationexps;
  DEN.process(text, durationexps);
  ASSERT_EQ(1u, durationexps.size());
  
  Time ex1_lower(INFINITY, INFINITY, INFINITY, 3, INFINITY, INFINITY);
  Time ex1_upper(-INFINITY, -INFINITY, -INFINITY, 4, -INFINITY, -INFINITY);
  
  EXPECT_EQ("三時間強", ustring_to_string(durationexps[0].original_expression));
  EXPECT_TRUE(is_same_time(ex1_lower, durationexps[0].value_lowerbound));
  EXPECT_TRUE(is_same_time(ex1_upper, durationexps[0].value_upperbound));
}


TEST_F(DurationexpNormalizerTest, jaku) {
  DurationExpressionNormalizer DEN("ja");
  std::string text("あの人は三時間弱は耐えた");
  std::vector<DurationExpression> durationexps;
  DEN.process(text, durationexps);
  ASSERT_EQ(1u, durationexps.size());
  
  Time ex1_lower(INFINITY, INFINITY, INFINITY, 2, INFINITY, INFINITY);
  Time ex1_upper(-INFINITY, -INFINITY, -INFINITY, 3, -INFINITY, -INFINITY);
  
  EXPECT_EQ("三時間弱", ustring_to_string(durationexps[0].original_expression));
  EXPECT_TRUE(is_same_time(ex1_lower, durationexps[0].value_lowerbound));
  EXPECT_TRUE(is_same_time(ex1_upper, durationexps[0].value_upperbound));
}
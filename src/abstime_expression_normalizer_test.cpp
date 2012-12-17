#include <gtest/gtest.h>
#include <string>
#include "normalizer_utility.hpp"
#include "abstime_expression_normalizer.hpp"

#include <pficommon/data/string/ustring.h>
#include <ux/ux.hpp>

using namespace normalizer_utility;
using namespace std;
using namespace pfi::data::string;
using namespace abstime_expression_normalizer;
class AbstimeexpNormalizerTest : public testing::Test {
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

TEST_F(AbstimeexpNormalizerTest, simple1) {
  AbstimeExpressionNormalizer AEN("ja");
  std::string text("あの人は1989年7月21日午前3時に生まれた");
  std::vector<AbstimeExpression> abstimeexps;
  AEN.process(text, abstimeexps);
  ASSERT_EQ(1u, abstimeexps.size());
    
  Time ex1_lower(1989,7,21,3, INFINITY, INFINITY);
  Time ex1_upper(1989,7,21,3, -INFINITY, -INFINITY);
  EXPECT_EQ("1989年7月21日午前3時", ustring_to_string(abstimeexps[0].original_expression));
  EXPECT_TRUE(is_same_time(ex1_lower, abstimeexps[0].value_lowerbound));
  EXPECT_TRUE(is_same_time(ex1_upper, abstimeexps[0].value_upperbound));
}

TEST_F(AbstimeexpNormalizerTest, simple2) {
  AbstimeExpressionNormalizer AEN("ja");
  std::string text("1989-7-21　1989.7.21　1989/7/21 １９８９．７．２１");
  std::vector<AbstimeExpression> abstimeexps;
  AEN.process(text, abstimeexps);
  ASSERT_EQ(4u, abstimeexps.size());
  
  Time ex_lower(1989,7,21, INFINITY, INFINITY, INFINITY);
  Time ex_upper(1989,7,21, -INFINITY, -INFINITY, -INFINITY);
  EXPECT_EQ("1989-7-21", ustring_to_string(abstimeexps[0].original_expression));
  EXPECT_TRUE(is_same_time(ex_lower, abstimeexps[0].value_lowerbound));
  EXPECT_TRUE(is_same_time(ex_upper, abstimeexps[0].value_upperbound));
  
  EXPECT_EQ("1989.7.21", ustring_to_string(abstimeexps[1].original_expression));
  EXPECT_TRUE(is_same_time(ex_lower, abstimeexps[1].value_lowerbound));
  EXPECT_TRUE(is_same_time(ex_upper, abstimeexps[1].value_upperbound));
  
  EXPECT_EQ("1989/7/21", ustring_to_string(abstimeexps[2].original_expression));
  EXPECT_TRUE(is_same_time(ex_lower, abstimeexps[2].value_lowerbound));
  EXPECT_TRUE(is_same_time(ex_upper, abstimeexps[2].value_upperbound));

  EXPECT_EQ("１９８９．７．２１", ustring_to_string(abstimeexps[3].original_expression));
  EXPECT_TRUE(is_same_time(ex_lower, abstimeexps[3].value_lowerbound));
  EXPECT_TRUE(is_same_time(ex_upper, abstimeexps[3].value_upperbound));
}

TEST_F(AbstimeexpNormalizerTest, gogo) {
  AbstimeExpressionNormalizer AEN("ja");
  std::string text("あの人は1989年7月21日午後3時に生まれた。");
  std::vector<AbstimeExpression> abstimeexps;
  AEN.process(text, abstimeexps);
  
  Time ex1_lower(1989,7,21,15, INFINITY, INFINITY);
  Time ex1_upper(1989,7,21,15, -INFINITY, -INFINITY);
  EXPECT_EQ("1989年7月21日午後3時", ustring_to_string(abstimeexps[0].original_expression));
  EXPECT_TRUE(is_same_time(ex1_lower, abstimeexps[0].value_lowerbound));
  EXPECT_TRUE(is_same_time(ex1_upper, abstimeexps[0].value_upperbound));
}

TEST_F(AbstimeexpNormalizerTest, gogo_han) {
  AbstimeExpressionNormalizer AEN("ja");
  std::string text("あの人は1989年7月21日午後3時半に生まれた。");
  std::vector<AbstimeExpression> abstimeexps;
  AEN.process(text, abstimeexps);
  
  Time ex1_lower(1989,7,21,15, 30, INFINITY);
  Time ex1_upper(1989,7,21,15, 30, -INFINITY);
  EXPECT_EQ("1989年7月21日午後3時半", ustring_to_string(abstimeexps[0].original_expression));
  EXPECT_TRUE(is_same_time(ex1_lower, abstimeexps[0].value_lowerbound));
  EXPECT_TRUE(is_same_time(ex1_upper, abstimeexps[0].value_upperbound));
}

TEST_F(AbstimeexpNormalizerTest, seiki) {
  AbstimeExpressionNormalizer AEN("ja");
  std::string text("あの人は18世紀に生まれた。");
  std::vector<AbstimeExpression> abstimeexps;
  AEN.process(text, abstimeexps);

  Time ex1_lower(1701, INFINITY, INFINITY, INFINITY, INFINITY, INFINITY);
  Time ex1_upper(1800, -INFINITY, -INFINITY, -INFINITY, -INFINITY, -INFINITY);
  EXPECT_EQ("18世紀", ustring_to_string(abstimeexps[0].original_expression));
  EXPECT_TRUE(is_same_time(ex1_lower, abstimeexps[0].value_lowerbound));
  EXPECT_TRUE(is_same_time(ex1_upper, abstimeexps[0].value_upperbound));
}

TEST_F(AbstimeexpNormalizerTest, about1) {
  AbstimeExpressionNormalizer AEN("ja");
  std::string text("あの人は1989年7月21日午前3時ごろに生まれた");
  std::vector<AbstimeExpression> abstimeexps;
  AEN.process(text, abstimeexps);
  ASSERT_EQ(1u, abstimeexps.size());
  
  Time ex1_lower(1989,7,21,2, INFINITY, INFINITY);
  Time ex1_upper(1989,7,21,4, -INFINITY, -INFINITY);
  EXPECT_EQ("1989年7月21日午前3時ごろ", ustring_to_string(abstimeexps[0].original_expression));
  EXPECT_TRUE(is_same_time(ex1_lower, abstimeexps[0].value_lowerbound));
  EXPECT_TRUE(is_same_time(ex1_upper, abstimeexps[0].value_upperbound));
}

TEST_F(AbstimeexpNormalizerTest, prefix_counter1) {
  AbstimeExpressionNormalizer AEN("ja");
  std::string text("平成1年7月21日、私は生まれた");
  std::vector<AbstimeExpression> abstimeexps;
  AEN.process(text, abstimeexps);
  
  Time ex1_lower(1989, 7, 21, INFINITY, INFINITY, INFINITY);
  Time ex1_upper(1989, 7, 21, -INFINITY, -INFINITY, -INFINITY);
  EXPECT_TRUE(is_same_time(ex1_lower, abstimeexps[0].value_lowerbound));
  EXPECT_TRUE(is_same_time(ex1_upper, abstimeexps[0].value_upperbound));
}

TEST_F(AbstimeexpNormalizerTest, zenhan1) {
  AbstimeExpressionNormalizer AEN("ja");
  std::string text("あの人は18世紀前半に生まれた。");
  std::vector<AbstimeExpression> abstimeexps;
  AEN.process(text, abstimeexps);
  Time ex1_lower(1701, INFINITY, INFINITY, INFINITY, INFINITY, INFINITY);
  Time ex1_upper(1750, -INFINITY, -INFINITY, -INFINITY, -INFINITY, -INFINITY);
  EXPECT_EQ("18世紀前半", ustring_to_string(abstimeexps[0].original_expression));
  EXPECT_TRUE(is_same_time(ex1_lower, abstimeexps[0].value_lowerbound));
  EXPECT_TRUE(is_same_time(ex1_upper, abstimeexps[0].value_upperbound));
}

TEST_F(AbstimeexpNormalizerTest, kouhan1) {
  AbstimeExpressionNormalizer AEN("ja");
  std::string text("あの人は18世紀後半に生まれた。");
  std::vector<AbstimeExpression> abstimeexps;
  AEN.process(text, abstimeexps);
  
  Time ex1_lower(1751, INFINITY, INFINITY, INFINITY, INFINITY, INFINITY);
  Time ex1_upper(1800, -INFINITY, -INFINITY, -INFINITY, -INFINITY, -INFINITY);
  EXPECT_EQ("18世紀後半", ustring_to_string(abstimeexps[0].original_expression));
  EXPECT_TRUE(is_same_time(ex1_lower, abstimeexps[0].value_lowerbound));
  EXPECT_TRUE(is_same_time(ex1_upper, abstimeexps[0].value_upperbound));
}

TEST_F(AbstimeexpNormalizerTest, nakaba1) {
  AbstimeExpressionNormalizer AEN("ja");
  std::string text("あの人は18世紀半ばに生まれた。");
  std::vector<AbstimeExpression> abstimeexps;
  AEN.process(text, abstimeexps);
  Time ex1_lower(1725, INFINITY, INFINITY, INFINITY, INFINITY, INFINITY);
  Time ex1_upper(1776, -INFINITY, -INFINITY, -INFINITY, -INFINITY, -INFINITY);
  EXPECT_EQ("18世紀半ば", ustring_to_string(abstimeexps[0].original_expression));
  EXPECT_TRUE(is_same_time(ex1_lower, abstimeexps[0].value_lowerbound));
  EXPECT_TRUE(is_same_time(ex1_upper, abstimeexps[0].value_upperbound));
}

TEST_F(AbstimeexpNormalizerTest, zenhan2) {
  AbstimeExpressionNormalizer AEN("ja");
  std::string text("あの人は7月3日朝に生まれた。");
  std::vector<AbstimeExpression> abstimeexps;
  AEN.process(text, abstimeexps);
  
  Time ex1_lower(INFINITY, 7, 3, 5, INFINITY, INFINITY);
  Time ex1_upper(-INFINITY, 7, 3, 12, -INFINITY, -INFINITY);
  EXPECT_EQ("7月3日朝", ustring_to_string(abstimeexps[0].original_expression));
  EXPECT_TRUE(is_same_time(ex1_lower, abstimeexps[0].value_lowerbound));
  EXPECT_TRUE(is_same_time(ex1_upper, abstimeexps[0].value_upperbound));
}

TEST_F(AbstimeexpNormalizerTest, joujun) {
  AbstimeExpressionNormalizer AEN("ja");
  std::string text("あの人は７月上旬に生まれた。");
  std::vector<AbstimeExpression> abstimeexps;
  AEN.process(text, abstimeexps);
  std::cout << abstimeexps[0].value_upperbound.day << std::endl;
  Time ex1_lower(INFINITY, 7, 1, INFINITY, INFINITY, INFINITY);
  Time ex1_upper(-INFINITY, 7, 10, -INFINITY, -INFINITY, -INFINITY);
  EXPECT_EQ("７月上旬", ustring_to_string(abstimeexps[0].original_expression));
  EXPECT_TRUE(is_same_time(ex1_lower, abstimeexps[0].value_lowerbound));
  EXPECT_TRUE(is_same_time(ex1_upper, abstimeexps[0].value_upperbound));
}

TEST_F(AbstimeexpNormalizerTest, or_less) {
  AbstimeExpressionNormalizer AEN("ja");
  std::string text("3月11日以前に、");
  std::vector<AbstimeExpression> abstimeexps;
  AEN.process(text, abstimeexps);
  ASSERT_EQ(1u, abstimeexps.size());
  
  Time ex1_lower(-INFINITY, -INFINITY, -INFINITY, -INFINITY, -INFINITY, -INFINITY);
  Time ex1_upper(-INFINITY, 3, 11, -INFINITY, -INFINITY, -INFINITY);
  EXPECT_EQ("3月11日以前", ustring_to_string(abstimeexps[0].original_expression));
  EXPECT_TRUE(is_same_time(ex1_lower, abstimeexps[0].value_lowerbound));
  EXPECT_TRUE(is_same_time(ex1_upper, abstimeexps[0].value_upperbound));
}

TEST_F(AbstimeexpNormalizerTest, or_over) {
  AbstimeExpressionNormalizer AEN("ja");
  std::string text("3月11日以降に、");
  std::vector<AbstimeExpression> abstimeexps;
  AEN.process(text, abstimeexps);
  ASSERT_EQ(1u, abstimeexps.size());
  
  Time ex1_lower(INFINITY, 3, 11, INFINITY, INFINITY, INFINITY);
  Time ex1_upper(INFINITY);
  EXPECT_EQ("3月11日以降", ustring_to_string(abstimeexps[0].original_expression));
  EXPECT_TRUE(is_same_time(ex1_lower, abstimeexps[0].value_lowerbound));
  EXPECT_TRUE(is_same_time(ex1_upper, abstimeexps[0].value_upperbound));
}

TEST_F(AbstimeexpNormalizerTest, range1) {
  AbstimeExpressionNormalizer AEN("ja");
  std::string text("15時〜18時の間に、");
  std::vector<AbstimeExpression> abstimeexps;
  AEN.process(text, abstimeexps);
  ASSERT_EQ(1u, abstimeexps.size());
  
  Time ex1_lower(INFINITY, INFINITY, INFINITY, 15, INFINITY, INFINITY);
  Time ex1_upper(-INFINITY, -INFINITY, -INFINITY, 18, -INFINITY, -INFINITY);
  EXPECT_EQ("15時〜18時", ustring_to_string(abstimeexps[0].original_expression));
  EXPECT_TRUE(is_same_time(ex1_lower, abstimeexps[0].value_lowerbound));
  EXPECT_TRUE(is_same_time(ex1_upper, abstimeexps[0].value_upperbound));
}

TEST_F(AbstimeexpNormalizerTest, range2) {
  AbstimeExpressionNormalizer AEN("ja");
  std::string text("15:00から18:00の間に、");
  std::vector<AbstimeExpression> abstimeexps;
  AEN.process(text, abstimeexps);
  ASSERT_EQ(1u, abstimeexps.size());
  
  Time ex1_lower(INFINITY, INFINITY, INFINITY, 15, 0, INFINITY);
  Time ex1_upper(-INFINITY, -INFINITY, -INFINITY, 18, 0, -INFINITY);
  EXPECT_EQ("15:00から18:00", ustring_to_string(abstimeexps[0].original_expression));
  EXPECT_TRUE(is_same_time(ex1_lower, abstimeexps[0].value_lowerbound));
  EXPECT_TRUE(is_same_time(ex1_upper, abstimeexps[0].value_upperbound));
}

TEST_F(AbstimeexpNormalizerTest, ambiguous1) {
  AbstimeExpressionNormalizer AEN("ja");
  std::string text("2011.3 その日、");
  std::vector<AbstimeExpression> abstimeexps;
  AEN.process(text, abstimeexps);
  ASSERT_EQ(1u, abstimeexps.size());
  
  Time ex1_lower(2011,3, INFINITY, INFINITY, INFINITY, INFINITY);
  Time ex1_upper(2011,3, -INFINITY, -INFINITY, -INFINITY, -INFINITY);
  EXPECT_EQ("2011.3", ustring_to_string(abstimeexps[0].original_expression));
  EXPECT_TRUE(is_same_time(ex1_lower, abstimeexps[0].value_lowerbound));
  EXPECT_TRUE(is_same_time(ex1_upper, abstimeexps[0].value_upperbound));
}

TEST_F(AbstimeexpNormalizerTest, chinese1) {
  AbstimeExpressionNormalizer AEN("zh");
  std::string text("我生于1989年7月21日");
  std::vector<AbstimeExpression> abstimeexps;
  AEN.process(text, abstimeexps);
  ASSERT_EQ(1u, abstimeexps.size());
  
  Time ex1_lower(1989,7,21, INFINITY, INFINITY, INFINITY);
  Time ex1_upper(1989,7,21, -INFINITY, -INFINITY, -INFINITY);
  EXPECT_EQ("1989年7月21日", ustring_to_string(abstimeexps[0].original_expression));
  EXPECT_TRUE(is_same_time(ex1_lower, abstimeexps[0].value_lowerbound));
  EXPECT_TRUE(is_same_time(ex1_upper, abstimeexps[0].value_upperbound));
}

/* 
 //未対応。曖昧性への対応はできていない。
TEST_F(AbstimeexpNormalizerTest, ambiguous2) {
  AbstimeExpressionNormalizer AEN("ja");
  std::string text("3.11 その日、");
  std::vector<AbstimeExpression> abstimeexps;
  AEN.process(text, abstimeexps);
  ASSERT_EQ(1u, abstimeexps.size());
  
  Time ex1_lower(INFINITY, 3, 11, INFINITY, INFINITY, INFINITY);
  Time ex1_upper(-INFINITY, 3, 11, -INFINITY, -INFINITY, -INFINITY);
  EXPECT_EQ("3.11", ustring_to_string(abstimeexps[0].original_expression));
  EXPECT_TRUE(is_same_time(ex1_lower, abstimeexps[0].value_lowerbound));
  EXPECT_TRUE(is_same_time(ex1_upper, abstimeexps[0].value_upperbound));
}
 */

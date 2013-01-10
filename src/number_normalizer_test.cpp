#include <gtest/gtest.h>
#include <string>
#include "number_normalizer.hpp"

using namespace number_normalizer;
using namespace pfi::data::string;
using namespace std;

class NumberNormalizerTest: public testing::Test {
public:
  void SetUp() {
    std::string language("ja");
    digit_utility::init_kansuji(language);
  }
  void TearDown() {
  }
};


TEST_F(NumberNormalizerTest, extract_one_number_test) {
  const string input("それは三十二年前の出来事");
  NumberExtractor ne;
  vector<Number> result;
  ne.extract_number(input, result);
  ASSERT_EQ(1u, result.size());
  EXPECT_EQ(string_to_ustring("三十二"), result[0].original_expression);
}

TEST_F(NumberNormalizerTest, extract_some_number_test) {
  const string input("三年前に１２３４５円が奪われたのは123,456円の");
  NumberExtractor ne;
  vector<Number> result;
  ne.extract_number(input, result);
  ASSERT_EQ(4u, result.size());
  EXPECT_EQ(string_to_ustring("三"), result[0].original_expression);
  EXPECT_EQ(string_to_ustring("１２３４５"), result[1].original_expression);
  EXPECT_EQ(string_to_ustring("123"), result[2].original_expression);
  EXPECT_EQ(string_to_ustring("456"), result[3].original_expression);

}

TEST_F(NumberNormalizerTest, extract_type_mixed_number) {
  const string input("1989三年前におきたその事件。");
  NumberExtractor ne;
  vector<Number> result;
  ne.extract_number(input, result);
  ASSERT_EQ(2u, result.size());
  EXPECT_NE(string_to_ustring("1989三"), result[0].original_expression);
  EXPECT_EQ(string_to_ustring("1989"), result[0].original_expression);
  EXPECT_EQ(string_to_ustring("三"), result[1].original_expression);
}

TEST_F(NumberNormalizerTest, convert_test1) {
  const ustring input(string_to_ustring("１，２３４"));
  JapaneseNumberConverter nc;
  double value;
  int number_type = 0;
  nc.convert_number(input, value, number_type);
  EXPECT_DOUBLE_EQ(1234.0, value);
}

TEST_F(NumberNormalizerTest, convert_test2) {
  const ustring input(string_to_ustring("１，２３４，５６７"));
  JapaneseNumberConverter nc;
  double value;
  int number_type = 0;
  nc.convert_number(input, value, number_type);
  EXPECT_DOUBLE_EQ(1234567.0, value);
}

TEST_F(NumberNormalizerTest, convert_test3) {
  const ustring input(string_to_ustring("一二三四五六七"));
  JapaneseNumberConverter nc;
  double value;
  int number_type = 0;
  nc.convert_number(input, value, number_type);
  EXPECT_DOUBLE_EQ(1234567.0, value);
}

TEST_F(NumberNormalizerTest, convert_test4) {
  const ustring input(string_to_ustring("123万4567"));
  JapaneseNumberConverter nc;
  double value;
  int number_type = 0;
  nc.convert_number(input, value, number_type);
  EXPECT_DOUBLE_EQ(1234567.0, value);
}

TEST_F(NumberNormalizerTest, convert_test5) {
  const ustring input(string_to_ustring("百二十三万四千五百六十七"));
  JapaneseNumberConverter nc;
  double value;
  int number_type = 0;
  nc.convert_number(input, value, number_type);
  EXPECT_DOUBLE_EQ(1234567.0, value);
}

TEST_F(NumberNormalizerTest, convert_test6) {
  const ustring input(string_to_ustring("百2十3万4千5百6十7"));
  JapaneseNumberConverter nc;
  double value;
  int number_type = 0;
  nc.convert_number(input, value, number_type);
  EXPECT_DOUBLE_EQ(1234567.0, value);
}

TEST_F(NumberNormalizerTest, process_test1) {
  const string text("その3,244人が３，４５６，７８９円で百二十三万四千五百六十七円");
  vector<Number> result;
  std::string language("ja");
  NumberNormalizer nn(language);
  nn.process(text, result);
  ASSERT_EQ(3u, result.size());
  EXPECT_DOUBLE_EQ(3244.0, result[0].value_lowerbound);
  EXPECT_DOUBLE_EQ(3456789.0, result[1].value_lowerbound);
  EXPECT_DOUBLE_EQ(1234567.0, result[2].value_lowerbound);
}

TEST_F(NumberNormalizerTest, process_test2_decimal_point) {
  const string text("その3,244.15人が３，４５６，７８９．４５６円");
  vector<Number> result;
  std::string language("ja");
  NumberNormalizer nn(language);
  nn.process(text, result);
  ASSERT_EQ(2u, result.size());
  EXPECT_DOUBLE_EQ(3244.15, result[0].value_lowerbound);
  EXPECT_DOUBLE_EQ(3456789.456, result[1].value_lowerbound);
}

TEST_F(NumberNormalizerTest, process_test2_decimal_point2) {
	const string text("131.1ポイントというスコアを叩き出した");
	vector<Number> result;
	std::string language("ja");
	NumberNormalizer nn(language);
	nn.process(text, result);
	ASSERT_EQ(1u, result.size());
	EXPECT_DOUBLE_EQ(131.1, result[0].value_lowerbound);
	EXPECT_DOUBLE_EQ(131.1, result[0].value_upperbound);
}

TEST_F(NumberNormalizerTest, process_test2_decimal_point3) {
	const string text("9.3万円も損した");
	vector<Number> result;
	std::string language("ja");
	NumberNormalizer nn(language);
	nn.process(text, result);
	ASSERT_EQ(1u, result.size());
	EXPECT_DOUBLE_EQ(93000, result[0].value_lowerbound);
	EXPECT_DOUBLE_EQ(93000, result[0].value_upperbound);
}

TEST_F(NumberNormalizerTest, process_test3_plus) {
  const string text("その+3,244人が＋３，４５６，７８９円でプラス百二十三万四千五百六十七円");
  vector<Number> result;
  std::string language("ja");
  NumberNormalizer nn(language);
  nn.process(text, result);
  ASSERT_EQ(3u, result.size());
  EXPECT_EQ("+3,244", ustring_to_string(result[0].original_expression));
  EXPECT_EQ("＋３，４５６，７８９", ustring_to_string(result[1].original_expression));
  EXPECT_EQ("プラス百二十三万四千五百六十七", ustring_to_string(result[2].original_expression));
}

TEST_F(NumberNormalizerTest, process_test3_minus) {
  const string text("その-3,244人がー３，４５６，７８９円でマイナス百二十三万四千五百六十七円");
  vector<Number> result;
  std::string language("ja");
  NumberNormalizer nn(language);
  nn.process(text, result);
  ASSERT_EQ(3u, result.size());
  EXPECT_DOUBLE_EQ(-3244.0, result[0].value_lowerbound);
  EXPECT_DOUBLE_EQ(-3456789.0, result[1].value_lowerbound);
  EXPECT_DOUBLE_EQ(-1234567.0, result[2].value_lowerbound);
}

TEST_F(NumberNormalizerTest, process_test4_range) {
  const string text("その10~20人が、１００〜２００円で");
  vector<Number> result;
  std::string language("ja");
  NumberNormalizer nn(language);
  nn.process(text, result);
  ASSERT_EQ(2u, result.size());
  EXPECT_DOUBLE_EQ(10.0, result[0].value_lowerbound);
  EXPECT_DOUBLE_EQ(20.0, result[0].value_upperbound);
  EXPECT_EQ(2, result[0].position_start);
  EXPECT_EQ(7, result[0].position_end);
  EXPECT_DOUBLE_EQ(100.0, result[1].value_lowerbound);
  EXPECT_DOUBLE_EQ(200.0, result[1].value_upperbound);
  EXPECT_EQ(10, result[1].position_start);
  EXPECT_EQ(17, result[1].position_end);
}

TEST_F(NumberNormalizerTest, donnot_have_number1) {
  const string text("メロスは激怒した。必ず、かの邪智暴虐の王を除かなければならぬと決意した。メロスには政治がわからぬ。メロスは、村の牧人である。笛を吹き、羊と遊んで暮して来た。");
  vector<Number> result;
  std::string language("ja");
  NumberNormalizer nn(language);
  nn.process(text, result);
  EXPECT_DOUBLE_EQ(0, result.size());
}

TEST_F(NumberNormalizerTest, process_test_corner1) {
  const string text("千円札");
  vector<Number> result;
  std::string language("ja");
  NumberNormalizer nn(language);
  nn.process(text, result);
  ASSERT_EQ(1u, result.size());
  EXPECT_DOUBLE_EQ(1000.0, result[0].value_lowerbound);
}

TEST_F(NumberNormalizerTest, plural1) {
  const string text("話をしよう。あれは今から36万年前………いや、1万4000年前だったか。");
  vector<Number> result;
  std::string language("ja");
  NumberNormalizer nn(language);
  nn.process(text, result);
  ASSERT_EQ(2u, result.size());
  EXPECT_DOUBLE_EQ(360000.0, result[0].value_lowerbound);
  EXPECT_DOUBLE_EQ(14000.0, result[1].value_lowerbound);
}



TEST_F(NumberNormalizerTest, invalid_notation1) {
  const string text("1千1千1千");
  vector<Number> result;
  std::string language("ja");
  NumberNormalizer nn(language);
  nn.process(text, result);
  ASSERT_EQ(3u, result.size());
  EXPECT_EQ("1千1", ustring_to_string(result[0].original_expression));
  EXPECT_EQ("千1", ustring_to_string(result[1].original_expression));
  EXPECT_EQ("千", ustring_to_string(result[2].original_expression));
}

TEST_F(NumberNormalizerTest, invalid_notation2) {
  const string text("三十七万五千億");
  vector<Number> result;
  std::string language("ja");
  NumberNormalizer nn(language);
  nn.process(text, result);
  ASSERT_EQ(1u, result.size());
  EXPECT_EQ("三十七万五千", ustring_to_string(result[0].original_expression));
}

TEST_F(NumberNormalizerTest, invalid_notation_type1) {
  const string text("２００７20人がきた");
  vector<Number> result;
  std::string language("ja");
  NumberNormalizer nn(language);
  nn.process(text, result);
  ASSERT_EQ(2u, result.size());
  EXPECT_EQ("２００７", ustring_to_string(result[0].original_expression));
  EXPECT_EQ("20", ustring_to_string(result[1].original_expression));
}

TEST_F(NumberNormalizerTest, invalid_notation_type2) {
  const string text("２００７二十人がきた");
  vector<Number> result;
  std::string language("ja");
  NumberNormalizer nn(language);
  nn.process(text, result);
  ASSERT_EQ(2u, result.size());
  EXPECT_EQ("２００７", ustring_to_string(result[0].original_expression));
  EXPECT_EQ("二十", ustring_to_string(result[1].original_expression));
}

TEST_F(NumberNormalizerTest, commma_range1) {
  const string text("1,2個");
  vector<Number> result;
  std::string language("ja");
  NumberNormalizer nn(language);
  nn.process(text, result);
  ASSERT_EQ(1u, result.size());
  EXPECT_EQ("1,2", ustring_to_string(result[0].original_expression));
}


TEST_F(NumberNormalizerTest, real_example1) {
	const string text("京・京");
	vector<Number> result;
	std::string language("ja");
	NumberNormalizer nn(language);
	nn.process(text, result);
	ASSERT_EQ(0u, result.size());
} 
TEST_F(NumberNormalizerTest, real_example2) {
	const string text("七〇〇万");
	vector<Number> result;
	std::string language("ja");
	NumberNormalizer nn(language);
	nn.process(text, result);
	ASSERT_EQ(1u, result.size());
	EXPECT_EQ("七〇〇万", ustring_to_string(result[0].original_expression));
  EXPECT_DOUBLE_EQ(7000000.0, result[0].value_lowerbound);
}

TEST_F(NumberNormalizerTest, real_example3) {
	const string text("7000千人");
	vector<Number> result;
	std::string language("ja");
	NumberNormalizer nn(language);
	nn.process(text, result);
	ASSERT_EQ(1u, result.size());
	EXPECT_EQ("7000千", ustring_to_string(result[0].original_expression));
  EXPECT_DOUBLE_EQ(7000000.0, result[0].value_lowerbound);
}

TEST_F(NumberNormalizerTest, real_example4) {
	const string text("京京億億万万京億万");
	vector<Number> result;
	std::string language("ja");
	NumberNormalizer nn(language);
	nn.process(text, result);
	ASSERT_EQ(0u, result.size());
}

TEST_F(NumberNormalizerTest, real_example5) {
	const string text("そうだ、京都いこう");
	vector<Number> result;
	std::string language("ja");
	NumberNormalizer nn(language);
	nn.process(text, result);
	ASSERT_EQ(0u, result.size());
}

TEST_F(NumberNormalizerTest, process_test5_suu) {
	const string text("その数十人が、数万人で、十数人で、百数十人で、一万数千人で、十数万人で、");
	std::vector<Number> result;
	std::string language("ja");
	NumberNormalizer nn(language);
	nn.process(text, result);
	EXPECT_EQ("数十", ustring_to_string(result[0].original_expression));
	EXPECT_EQ("数万", ustring_to_string(result[1].original_expression));
	EXPECT_EQ("十数", ustring_to_string(result[2].original_expression));
	EXPECT_EQ("百数十", ustring_to_string(result[3].original_expression));
	EXPECT_EQ("一万数千", ustring_to_string(result[4].original_expression));
	EXPECT_EQ("十数万", ustring_to_string(result[5].original_expression));
	EXPECT_EQ(10.0, result[0].value_lowerbound);
	EXPECT_EQ(90.0, result[0].value_upperbound);
	EXPECT_EQ(10000.0, result[1].value_lowerbound);
	EXPECT_EQ(90000.0, result[1].value_upperbound);
	EXPECT_EQ(11.0, result[2].value_lowerbound);
	EXPECT_EQ(19.0, result[2].value_upperbound);
	EXPECT_EQ(110.0, result[3].value_lowerbound);
	EXPECT_EQ(190.0, result[3].value_upperbound);
	EXPECT_EQ(11000.0, result[4].value_lowerbound);
	EXPECT_EQ(19000.0, result[4].value_upperbound);
	EXPECT_EQ(110000.0, result[5].value_lowerbound);
	EXPECT_EQ(190000.0, result[5].value_upperbound);
}


TEST_F(NumberNormalizerTest, process_test5_suu2) {
	const string text("0数万人"); //２〇万人
	std::vector<Number> result;
	std::string language("ja");
	NumberNormalizer nn(language);
	nn.process(text, result);
	EXPECT_EQ("0数", ustring_to_string(result[0].original_expression));
	EXPECT_EQ(1.0, result[0].value_lowerbound);
	EXPECT_EQ(9.0, result[0].value_upperbound);
}




/*
TEST_F(NumberNormalizerTest, process_test5_suu3) {
	const string text("２〇数万人"); //２〇万人
	std::vector<Number> result;
	std::string language("ja");
	NumberNormalizer nn(language);
	nn.process(text, result);
	EXPECT_EQ("２〇数万", ustring_to_string(result[0].original_expression));
	EXPECT_EQ(210000.0, result[0].value_lowerbound);
	EXPECT_EQ(290000.0, result[0].value_upperbound);
}
*/



/*
中国語のテスト
*/

TEST_F(NumberNormalizerTest, chinese1) {
  const string text("千二百三");
  vector<Number> result;
  std::string language("zh");
  NumberNormalizer nn(language);
  nn.process(text, result);
  ASSERT_EQ(1u, result.size());
  EXPECT_EQ("千二百三", ustring_to_string(result[0].original_expression));
	EXPECT_EQ(1230, result[0].value_lowerbound);
}

TEST_F(NumberNormalizerTest, chinese2) {
  const string text("1230");
  vector<Number> result;
  std::string language("zh");
  NumberNormalizer nn(language);
  nn.process(text, result);
  ASSERT_EQ(1u, result.size());
  EXPECT_EQ("1230", ustring_to_string(result[0].original_expression));
	EXPECT_EQ(1230, result[0].value_lowerbound);
}

TEST_F(NumberNormalizerTest, chinese3) {
  const string text("1万2300");
  vector<Number> result;
  std::string language("zh");
  NumberNormalizer nn(language);
  nn.process(text, result);
  ASSERT_EQ(1u, result.size());
  EXPECT_EQ("1万2300", ustring_to_string(result[0].original_expression));
	EXPECT_EQ(12300, result[0].value_lowerbound);
}

TEST_F(NumberNormalizerTest, chinese4) {
  const string text("一百一");
  vector<Number> result;
  std::string language("zh");
  NumberNormalizer nn(language);
  nn.process(text, result);
  ASSERT_EQ(1u, result.size());
  EXPECT_EQ(text, ustring_to_string(result[0].original_expression));
	EXPECT_EQ(110, result[0].value_lowerbound);
}

TEST_F(NumberNormalizerTest, chinese5) {
  const string text("一千零一十");
  vector<Number> result;
  std::string language("zh");
  NumberNormalizer nn(language);
  nn.process(text, result);
  ASSERT_EQ(1u, result.size());
  EXPECT_EQ(text, ustring_to_string(result[0].original_expression));
	EXPECT_EQ(1010, result[0].value_lowerbound);
}

TEST_F(NumberNormalizerTest, chinese6) {
  const string text("两千");
  vector<Number> result;
  std::string language("zh");
  NumberNormalizer nn(language);
  nn.process(text, result);
  ASSERT_EQ(1u, result.size());
  EXPECT_EQ(text, ustring_to_string(result[0].original_expression));
	EXPECT_EQ(2000, result[0].value_lowerbound);
}

TEST_F(NumberNormalizerTest, chinese7) {
  const string text("一亿");
  vector<Number> result;
  std::string language("zh");
  NumberNormalizer nn(language);
  nn.process(text, result);
  ASSERT_EQ(1u, result.size());
  EXPECT_EQ(text, ustring_to_string(result[0].original_expression));
	EXPECT_EQ(100000000, result[0].value_lowerbound);
}

/*
 invalidな表記。これは扱う？
 TEST_F(NumberNormalizerTest, chinese8) {
 const string text("一万亿");
 vector<Number> result;
 std::string language("zh");
 NumberNormalizer nn(language);
 nn.process(text, result);
 ASSERT_EQ(1u, result.size());
 EXPECT_EQ(text, ustring_to_string(result[0].original_expression));
 EXPECT_EQ(1000000000000, result[0].value_lowerbound);
 }
 */
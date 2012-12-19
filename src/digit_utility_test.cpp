#include <gtest/gtest.h>
#include <string>
#include "digit_utility.hpp"

#include <pficommon/data/string/ustring.h>

using namespace digit_utility;

class DigitUtilityTest: public testing::Test {
public:
  void SetUp() {
    std::string language("ja");
    init_kansuji(language);
  }
  void TearDown() {
  }
};

TEST_F(DigitUtilityTest, isHankaku) {
  EXPECT_TRUE(is_hankakusuji(pfi::data::string::string_to_uchar("1")));
  EXPECT_FALSE(is_hankakusuji(pfi::data::string::string_to_uchar("１")));
  EXPECT_FALSE(is_hankakusuji(pfi::data::string::string_to_uchar("一")));
  EXPECT_FALSE(is_hankakusuji(pfi::data::string::string_to_uchar("あ")));
}

TEST_F(DigitUtilityTest, isZenkaku) {
  EXPECT_FALSE(is_zenkakusuji(pfi::data::string::string_to_uchar("1")));
  EXPECT_TRUE(is_zenkakusuji(pfi::data::string::string_to_uchar("１")));
  EXPECT_FALSE(is_zenkakusuji(pfi::data::string::string_to_uchar("一")));
  EXPECT_FALSE(is_zenkakusuji(pfi::data::string::string_to_uchar("あ")));
}

TEST_F(DigitUtilityTest, isArabic) {
  EXPECT_TRUE(is_arabic(pfi::data::string::string_to_uchar("1")));
  EXPECT_TRUE(is_arabic(pfi::data::string::string_to_uchar("１")));
  EXPECT_FALSE(is_arabic(pfi::data::string::string_to_uchar("一")));
  EXPECT_FALSE(is_arabic(pfi::data::string::string_to_uchar("あ")));
}

TEST_F(DigitUtilityTest, isKansuji) {
  EXPECT_FALSE(is_kansuji(pfi::data::string::string_to_uchar("1")));
  EXPECT_FALSE(is_kansuji(pfi::data::string::string_to_uchar("１")));
  EXPECT_TRUE(is_kansuji(pfi::data::string::string_to_uchar("一")));
  EXPECT_FALSE(is_kansuji(pfi::data::string::string_to_uchar("あ")));
}

TEST_F(DigitUtilityTest, isKansuji09) {
  EXPECT_FALSE(is_kansuji_09(pfi::data::string::string_to_uchar("1")));
  EXPECT_FALSE(is_kansuji_09(pfi::data::string::string_to_uchar("１")));
  EXPECT_TRUE(is_kansuji_09(pfi::data::string::string_to_uchar("一")));
  EXPECT_FALSE(is_kansuji_09(pfi::data::string::string_to_uchar("十")));
  EXPECT_FALSE(is_kansuji_09(pfi::data::string::string_to_uchar("万")));
  EXPECT_FALSE(is_kansuji_09(pfi::data::string::string_to_uchar("あ")));
}

TEST_F(DigitUtilityTest, isKansujiKuraiSen) {
  EXPECT_FALSE(is_kansuji_kurai_sen(pfi::data::string::string_to_uchar("1")));
  EXPECT_FALSE(is_kansuji_kurai_sen(pfi::data::string::string_to_uchar("１")));
  EXPECT_FALSE(is_kansuji_kurai_sen(pfi::data::string::string_to_uchar("一")));
  EXPECT_TRUE(is_kansuji_kurai_sen(pfi::data::string::string_to_uchar("十")));
  EXPECT_TRUE(is_kansuji_kurai_sen(pfi::data::string::string_to_uchar("百")));
  EXPECT_TRUE(is_kansuji_kurai_sen(pfi::data::string::string_to_uchar("千")));
  EXPECT_FALSE(is_kansuji_kurai_sen(pfi::data::string::string_to_uchar("万")));
  EXPECT_FALSE(is_kansuji_kurai_sen(pfi::data::string::string_to_uchar("あ")));
}

TEST_F(DigitUtilityTest, isKansujiKuraiMan) {
  EXPECT_FALSE(is_kansuji_kurai_man(pfi::data::string::string_to_uchar("1")));
  EXPECT_FALSE(is_kansuji_kurai_man(pfi::data::string::string_to_uchar("１")));
  EXPECT_FALSE(is_kansuji_kurai_man(pfi::data::string::string_to_uchar("一")));
  EXPECT_FALSE(is_kansuji_kurai_man(pfi::data::string::string_to_uchar("十")));
  EXPECT_TRUE(is_kansuji_kurai_man(pfi::data::string::string_to_uchar("万")));
  EXPECT_TRUE(is_kansuji_kurai_man(pfi::data::string::string_to_uchar("億")));
  EXPECT_TRUE(is_kansuji_kurai_man(pfi::data::string::string_to_uchar("兆")));
  EXPECT_FALSE(is_kansuji_kurai_man(pfi::data::string::string_to_uchar("あ")));
}

TEST_F(DigitUtilityTest, isKansujiKurai) {
  EXPECT_FALSE(is_kansuji_kurai(pfi::data::string::string_to_uchar("1")));
  EXPECT_FALSE(is_kansuji_kurai(pfi::data::string::string_to_uchar("１")));
  EXPECT_FALSE(is_kansuji_kurai(pfi::data::string::string_to_uchar("一")));
  EXPECT_TRUE(is_kansuji_kurai(pfi::data::string::string_to_uchar("十")));
  EXPECT_TRUE(is_kansuji_kurai(pfi::data::string::string_to_uchar("万")));
  EXPECT_FALSE(is_kansuji_kurai(pfi::data::string::string_to_uchar("あ")));
}

TEST_F(DigitUtilityTest, isNumber) {
  EXPECT_TRUE(is_number(pfi::data::string::string_to_uchar("1")));
  EXPECT_TRUE(is_number(pfi::data::string::string_to_uchar("１")));
  EXPECT_TRUE(is_number(pfi::data::string::string_to_uchar("一")));
  EXPECT_TRUE(is_number(pfi::data::string::string_to_uchar("十")));
  EXPECT_TRUE(is_number(pfi::data::string::string_to_uchar("万")));
  EXPECT_FALSE(is_number(pfi::data::string::string_to_uchar("あ")));
}

#include <gtest/gtest.h>
#include <string>
#include "normalize_numexp.hpp"

using namespace normalize_numexp;
using namespace std;

class NumexpExtractorTest : public testing::Test {
public:
    void SetUp() {}
    void TearDown() {}
};

TEST_F(NumexpExtractorTest, simple1) {
  vector<string> result;
  string language("ja");
  string text("1911年から2011年の間、その100年間において、9.3万人もの死傷者がでた。");
  NormalizeNumexp NN(language);
  NN.normalize(text, result);
	for(int i=0; i<static_cast<int>(result.size()); i++){
	  cout << result[i] << endl;
  }
  ASSERT_EQ(3u, result.size());
  EXPECT_EQ("numerical*9.3万人*27*32*人*93000*93000*", result[0]);
  EXPECT_EQ("abstime*1911年から2011年*0*12*none*1911-XX-XX*2011-XX-XX*", result[1]);
  EXPECT_EQ("duration*100年間*17*22*none*P100Y*P100Y*", result[2]);
}

TEST_F(NumexpExtractorTest, simple2) {
	vector<string> result;
	string language("ja");
	string text("15年前、戦争があった");
	NormalizeNumexp NN(language);
	NN.normalize(text, result);
	ASSERT_EQ(1u, result.size());
	EXPECT_EQ("reltime*15年前*0*4*none*XX:XX:XX,P-15Y*XX:XX:XX,P-15Y*", result[0]);
	for(int i=0; i<static_cast<int>(result.size()); i++){
		cout << result[i] << endl;
	}
}

TEST_F(NumexpExtractorTest, simple3) {
	vector<string> result;
	string language("ja");
	string text("昨年3月、僕たち２人は結婚した");
	NormalizeNumexp NN(language);
	NN.normalize(text, result);
	ASSERT_EQ(2u, result.size());
	EXPECT_EQ("numerical*２人*8*10*人*2*2*", result[0]);
	EXPECT_EQ("reltime*昨年3月*0*4*none*XXXX-03-XX,P-1Y*XXXX-03-XX,P-1Y*", result[1]);
	for(int i=0; i<static_cast<int>(result.size()); i++){
		cout << result[i] << endl;
	}
}

TEST_F(NumexpExtractorTest, simple4) {
	vector<string> result;
	string language("ja");
	string text("131.1ポイントというスコアを叩き出した");
	NormalizeNumexp NN(language);
	NN.normalize(text, result);
	ASSERT_EQ(1u, result.size());
	EXPECT_EQ("numerical*131.1ポイント*0*9*ポイント*131.1*131.1*", result[0]);
	for(int i=0; i<static_cast<int>(result.size()); i++){
		cout << result[i] << endl;
	}
}

TEST_F(NumexpExtractorTest, simple5) {
	vector<string> result;
	string language("ja");
	string text("午後3時45分に待ち合わせ");
	NormalizeNumexp NN(language);
	NN.normalize(text, result);
	ASSERT_EQ(1u, result.size());
	EXPECT_EQ("abstime*午後3時45分*0*7*none*15:45:XX*15:45:XX*", result[0]);
	for(int i=0; i<static_cast<int>(result.size()); i++){
		cout << result[i] << endl;
	}
}

TEST_F(NumexpExtractorTest, day_of_week1) {
	vector<string> result;
	string language("ja");
	string text("5月3日(水)");
	NormalizeNumexp NN(language);
	NN.normalize(text, result);
	ASSERT_EQ(1u, result.size());
	EXPECT_EQ("abstime*5月3日(水)*0*7*none*XXXX-05-03*XXXX-05-03*Wed", result[0]);
	for(int i=0; i<static_cast<int>(result.size()); i++){
		cout << result[i] << endl;
	}
}

/*
//辞書にはあるが認識してくれない。uxが空白を認識してくれていない??
TEST_F(NumexpExtractorTest, day_of_week2) {
	vector<string> result;
	string language("ja");
	string text("2001/3/3 Sat");
	NormalizeNumexp NN(language);
	NN.normalize(text, result);
	ASSERT_EQ(1u, result.size());
	EXPECT_EQ("abstime*2001/3/3*8*16*none*2001-03-3*2001-03-3*Sat", result[0]);
	for(int i=0; i<static_cast<int>(result.size()); i++){
		cout << result[i] << endl;
	}
}
*/

TEST_F(NumexpExtractorTest, real_example1) {
	vector<string> result;
	string language("ja");
	string text("【今日から開催】The Fruits of Adventures @ ZEIT-FOTO SALON(東京・京橋)  4/26(Tue)まで");
	NormalizeNumexp NN(language);
	NN.normalize(text, result);
	for(int i=0; i<static_cast<int>(result.size()); i++){
		cout << result[i] << endl;
	}
	ASSERT_EQ(1u, result.size());
	EXPECT_EQ("abstime*4/26(Tue)まで*59*70*none*XXXX-04-26*XXXX-04-26*Tue", result[0]);
}

TEST_F(NumexpExtractorTest, inappropriate_range1) {
	vector<string> result;
	string language("ja");
	string text("中国から30匹の鳥がきた");
	NormalizeNumexp NN(language);
	NN.normalize(text, result);
	for(int i=0; i<static_cast<int>(result.size()); i++){
		cout << result[i] << endl;
	}
	ASSERT_EQ(1u, result.size());
	EXPECT_EQ("numerical*30匹*4*7*匹*30*30*kara_prefix", result[0]);
}


TEST_F(NumexpExtractorTest, inappropriate_range2) {
	vector<string> result;
	string language("ja");
	string text("30匹からのプレゼント");
	NormalizeNumexp NN(language);
	NN.normalize(text, result);
	for(int i=0; i<static_cast<int>(result.size()); i++){
		cout << result[i] << endl;
	}
	ASSERT_EQ(1u, result.size());
	EXPECT_EQ("numerical*30匹*0*3*匹*30*30*kara_suffix", result[0]);
}

TEST_F(NumexpExtractorTest, inappropriate_range3) {
	vector<string> result;
	string language("ja");
	string text("一万年と二千年前から愛してる");
	NormalizeNumexp NN(language);
	NN.normalize(text, result);
	for(int i=0; i<static_cast<int>(result.size()); i++){
		cout << result[i] << endl;
	}
	ASSERT_EQ(2u, result.size());
	EXPECT_EQ("reltime*二千年前*4*8*none*XX:XX:XX,P-2000Y*XX:XX:XX,P-2000Y*kara_suffix", result[0]);
	EXPECT_EQ("duration*一万年*0*3*none*P10000Y*P10000Y*", result[1]);
}

TEST_F(NumexpExtractorTest, inappropriate_range4) {
	vector<string> result;
	string language("ja");
	string text("話をしよう。あれは今から36万年前………いや、1万4000年前だったか。");
	NormalizeNumexp NN(language);
	NN.normalize(text, result);
	for(int i=0; i<static_cast<int>(result.size()); i++){
		cout << result[i] << endl;
	}
	ASSERT_EQ(2u, result.size());
	EXPECT_EQ("reltime*36万年前*12*17*none*XX:XX:XX,P-360000Y*XX:XX:XX,P-360000Y*kara_prefix", result[0]);
	EXPECT_EQ("reltime*1万4000年前*23*31*none*XX:XX:XX,P-14000Y*XX:XX:XX,P-14000Y*", result[1]);
}

TEST_F(NumexpExtractorTest, inappropriate_strings1) {
	vector<string> result;
	string language("ja");
	string text("一体それがどうしたというのだね。九州。四国。");
	NormalizeNumexp NN(language);
	NN.normalize(text, result);
	for(int i=0; i<static_cast<int>(result.size()); i++){
		cout << result[i] << endl;
	}
	ASSERT_EQ(0u, result.size());
}


TEST_F(NumexpExtractorTest, inappropriate_prefix1) {
	vector<string> result;
	string language("ja");
	string text("ver2.3.4。ver２．３。");
	NormalizeNumexp NN(language);
	NN.normalize(text, result);
	for(int i=0; i<static_cast<int>(result.size()); i++){
		cout << result[i] << endl;
	}
	ASSERT_EQ(0u, result.size());
}


TEST_F(NumexpExtractorTest, inappropriate_abstime1) {
	vector<string> result;
	string language("ja");
	string text("080-6006-4451。ver2.0。");
	NormalizeNumexp NN(language);
	NN.normalize(text, result);
	for(int i=0; i<static_cast<int>(result.size()); i++){
		cout << result[i] << endl;
	}
	ASSERT_EQ(0u, result.size());
}

TEST_F(NumexpExtractorTest, inappropriate_abstime2) {
	vector<string> result;
	string language("ja");
	string text("198999年30月41日。");
	NormalizeNumexp NN(language);
	NN.normalize(text, result);
	for(int i=0; i<static_cast<int>(result.size()); i++){
		cout << result[i] << endl;
	}
	ASSERT_EQ(3u, result.size()); //durationとして認識される
}

TEST_F(NumexpExtractorTest, url1) {
	vector<string> result;
	string language("ja");
	string text("tttp3gl3molggg");
	NormalizeNumexp NN(language);
	NN.normalize(text, result);
	for(int i=0; i<static_cast<int>(result.size()); i++){
		cout << result[i] << endl;
	}
	ASSERT_EQ(0u, result.size());
}

TEST_F(NumexpExtractorTest, revise_abstime1) {
	vector<string> result;
	string language("ja");
	string text("09年5月。99年5月");
	NormalizeNumexp NN(language);
	NN.normalize(text, result);
	for(int i=0; i<static_cast<int>(result.size()); i++){
		cout << result[i] << endl;
	}
	ASSERT_EQ(2u, result.size());
	EXPECT_EQ("abstime*09年5月*0*5*none*2009-05-XX*2009-05-XX*", result[0]);
	EXPECT_EQ("abstime*99年5月*6*11*none*1999-05-XX*1999-05-XX*", result[1]);	
}

TEST_F(NumexpExtractorTest, not_abstime1) {
	vector<string> result;
	string language("ja");
	string text("1.2.2 2-2-2");
	NormalizeNumexp NN(language);
	NN.normalize(text, result);
	for(int i=0; i<static_cast<int>(result.size()); i++){
		cout << result[i] << endl;
	}
	ASSERT_EQ(0u, result.size());
}

TEST_F(NumexpExtractorTest, revise_abstime2) {
	vector<string> result;
	string language("ja");
	string text("西暦99年5月");
	NormalizeNumexp NN(language);
	NN.normalize(text, result);
	for(int i=0; i<static_cast<int>(result.size()); i++){
		cout << result[i] << endl;
	}
	ASSERT_EQ(1u, result.size());
	EXPECT_EQ("abstime*西暦99年5月*0*7*none*0099-05-XX*0099-05-XX*", result[0]);
}

TEST_F(NumexpExtractorTest, su1) {
	vector<string> result;
	string language("ja");
	string text("数十人が十数人と喧嘩して、百数十円落とした");
	NormalizeNumexp NN(language);
	NN.normalize(text, result);
	for(int i=0; i<static_cast<int>(result.size()); i++){
		cout << result[i] << endl;
	}
	ASSERT_EQ(3u, result.size());
	EXPECT_EQ("numerical*数十人*0*3*人*10*90*", result[0]);
	EXPECT_EQ("numerical*十数人*4*7*人*11*19*", result[1]);
	EXPECT_EQ("numerical*百数十円*13*17*円*110*190*", result[2]);
}

TEST_F(NumexpExtractorTest, range1) {
	vector<string> result;
	string language("ja");
	string text("2012/4/3~6に行われる");
	NormalizeNumexp NN(language);
	NN.normalize(text, result);
	for(int i=0; i<static_cast<int>(result.size()); i++){
		cout << result[i] << endl;
	}
	ASSERT_EQ(1u, result.size());
	EXPECT_EQ("abstime*2012/4/3~6*0*10*none*2012-04-03*2012-04-06*", result[0]);
}

TEST_F(NumexpExtractorTest, range2) {
	vector<string> result;
	string language("ja");
	string text("2012/4/3~2012/4/6に行われる");
	NormalizeNumexp NN(language);
	NN.normalize(text, result);
	for(int i=0; i<static_cast<int>(result.size()); i++){
		cout << result[i] << endl;
	}
	ASSERT_EQ(1u, result.size());
	EXPECT_EQ("abstime*2012/4/3~2012/4/6*0*17*none*2012-04-03*2012-04-06*", result[0]);
}

TEST_F(NumexpExtractorTest, wari1) {
	vector<string> result;
	string language("ja");
	string text("彼の打率は3割4分5厘だ");
	NormalizeNumexp NN(language);
	NN.normalize(text, result);
	for(int i=0; i<static_cast<int>(result.size()); i++){
		cout << result[i] << endl;
	}
	ASSERT_EQ(1u, result.size());
	EXPECT_EQ("numerical*3割4分5厘*5*11*%*34.5*34.5*", result[0]);
}
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


/*
tests for Japanese
*/

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
  EXPECT_EQ("abstime*1911年から2011年*0*12*none*1911*2011*", result[1]);
  EXPECT_EQ("duration*100年間*17*22*none*P100Y*P100Y*", result[2]);
}

TEST_F(NumexpExtractorTest, simple2) {
	vector<string> result;
	string language("ja");
	string text("15年前、戦争があった");
	NormalizeNumexp NN(language);
	NN.normalize(text, result);
	ASSERT_EQ(1u, result.size());
	EXPECT_EQ("reltime*15年前*0*4*none*TXX:XX:XX,P-15Y*TXX:XX:XX,P-15Y*", result[0]);
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
	EXPECT_EQ("reltime*昨年3月*0*4*none*XXXX-03,P-1Y*XXXX-03,P-1Y*", result[1]);
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
	EXPECT_EQ("abstime*午後3時45分*0*7*none*T15:45:XX*T15:45:XX*", result[0]);
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
	EXPECT_EQ("reltime*二千年前*4*8*none*TXX:XX:XX,P-2000Y*TXX:XX:XX,P-2000Y*kara_suffix", result[0]);
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
	EXPECT_EQ("reltime*36万年前*12*17*none*TXX:XX:XX,P-360000Y*TXX:XX:XX,P-360000Y*kara_prefix", result[0]);
	EXPECT_EQ("reltime*1万4000年前*23*31*none*TXX:XX:XX,P-14000Y*TXX:XX:XX,P-14000Y*", result[1]);
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
	EXPECT_EQ("abstime*09年5月*0*5*none*2009-05*2009-05*", result[0]);
	EXPECT_EQ("abstime*99年5月*6*11*none*1999-05*1999-05*", result[1]);	
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
	EXPECT_EQ("abstime*西暦99年5月*0*7*none*0099-05*0099-05*", result[0]);
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


/*
tests for MOD_ and special ISO format
*/
TEST_F(NumexpExtractorTest, mod_approx1) {
	vector<string> result;
	string language("ja");
	string text("約3人以上");
	NormalizeNumexp NN(language);
	NN.normalize(text, result);
	for(int i=0; i<static_cast<int>(result.size()); i++){
		cout << result[i] << endl;
	}
	ASSERT_EQ(1u, result.size());
	EXPECT_EQ("numerical*約3人以上*0*5*人*3*3*EQUAL_OR_MORE,APPROX", result[0]);
}

TEST_F(NumexpExtractorTest, bc1) {
	vector<string> result;
	string language("ja");
	string text("紀元前3000年");
	NormalizeNumexp NN(language);
	NN.normalize(text, result);
	for(int i=0; i<static_cast<int>(result.size()); i++){
		cout << result[i] << endl;
	}
	ASSERT_EQ(1u, result.size());
	EXPECT_EQ("abstime*紀元前3000年*0*8*none*3000*3000*BC", result[0]);
}

/*
tests for TIMEX format output
*/

TEST_F(NumexpExtractorTest, timex0) {
	string text_annotated;
	string language("ja");
	string text("紀元前3000年、それは300人の人が30年間に3時間30分にわたって");
	NormalizeNumexp NN(language);
	NN.normalize_by_timex_format(text, text_annotated);
	cout << "debug:" << text_annotated << endl;
	EXPECT_EQ("<TIMEX tid=\"t0\" type=\"DATE\" valueFromSurfaceLowerBound=\"BC3000\" valueFromSurfaceUpperBound=\"BC3000\">紀元前3000年</TIMEX>、それは<NUMEX nid=\"n0\" valueLowerBound=\"300\" valueUpperBound=\"300\" counter=\"人\">300人</NUMEX>の人が<TIMEX tid=\"t1\" type=\"DURATION\" valueFromSurfaceLowerBound=\"P30Y\" valueFromSurfaceUpperBound=\"P30Y\">30年間</TIMEX>に<TIMEX tid=\"t2\" type=\"DURATION\" valueFromSurfaceLowerBound=\"PT3HT30M\" valueFromSurfaceUpperBound=\"PT3HT30M\">3時間30分</TIMEX>にわたって", text_annotated);
}

TEST_F(NumexpExtractorTest, timex1) { //以下、小西ら(2013)の例文より
	string text_annotated;
	string language("ja");
	string text("二〇〇三年十月二十日　月曜日。午後五時傘寿ぷん。ステイシーはだらけた姿勢でモニターの前に陣取り、白黒の画像に見入っていた。彼女は伸びをし、腕時計に目をやった。二時間半で収穫ゼロ");
	NormalizeNumexp NN(language);
	NN.normalize_by_timex_format(text, text_annotated);
	cout << "debug:" << text_annotated << endl;
	EXPECT_EQ("<TIMEX tid=\"t0\" type=\"DATE\" valueFromSurfaceLowerBound=\"2003-10-20\" valueFromSurfaceUpperBound=\"2003-10-20\">二〇〇三年十月二十日　月曜日</TIMEX>。<TIMEX tid=\"t1\" type=\"TIME\" valueFromSurfaceLowerBound=\"T17:XX:XX\" valueFromSurfaceUpperBound=\"T17:XX:XX\" temporalFunction=\"true\">午後五時</TIMEX>傘寿ぷん。ステイシーはだらけた姿勢でモニターの前に陣取り、白黒の画像に見入っていた。彼女は伸びをし、腕時計に目をやった。<TIMEX tid=\"t2\" type=\"DURATION\" valueFromSurfaceLowerBound=\"PT2.5H\" valueFromSurfaceUpperBound=\"PT2.5H\">二時間半</TIMEX>で収穫ゼロ", text_annotated);
}


TEST_F(NumexpExtractorTest, timex2) {
	string text_annotated;
	string language("ja");
	string text("1980年7月7日、水曜日、1999年冬、第一四半期、1998年度、11世紀、紀元前202年、4000年前、2億年前、2005年8月8日午前8時45分30秒、午前8時45分30秒、3日未明、四日朝、四日昼、四日日中、四日午後、四日夕方、四日夜、四日深夜");
	NormalizeNumexp NN(language);
	NN.normalize_by_timex_format(text, text_annotated);
	cout << "debug:" << text_annotated << endl;
	EXPECT_EQ("<TIMEX tid=\"t0\" type=\"DATE\" valueFromSurfaceLowerBound=\"1980-07-07\" valueFromSurfaceUpperBound=\"1980-07-07\">1980年7月7日</TIMEX>、水曜日、<TIMEX tid=\"t1\" type=\"DATE\" valueFromSurfaceLowerBound=\"1999\" valueFromSurfaceUpperBound=\"1999\">1999年</TIMEX>冬、第一四半期、<TIMEX tid=\"t2\" type=\"DATE\" valueFromSurfaceLowerBound=\"1998\" valueFromSurfaceUpperBound=\"1998\">1998年</TIMEX>度、<TIMEX tid=\"t3\" type=\"DATE\" valueFromSurfaceLowerBound=\"1001\" valueFromSurfaceUpperBound=\"1100\">11世紀</TIMEX>、<TIMEX tid=\"t4\" type=\"DATE\" valueFromSurfaceLowerBound=\"BC0202\" valueFromSurfaceUpperBound=\"BC0202\">紀元前202年</TIMEX>、<TIMEX tid=\"t5\" type=\"TIME\" valueFromSurfaceLowerBound=\"TXX:XX:XX,P-4000Y\" valueFromSurfaceUpperBound=\"TXX:XX:XX,P-4000Y\" temporalFunction=\"true\">4000年前</TIMEX>、<TIMEX tid=\"t6\" type=\"TIME\" valueFromSurfaceLowerBound=\"TXX:XX:XX,P-2e+08Y\" valueFromSurfaceUpperBound=\"TXX:XX:XX,P-2e+08Y\" temporalFunction=\"true\">2億年前</TIMEX>、<TIMEX tid=\"t7\" type=\"DATE\" valueFromSurfaceLowerBound=\"2005-08-08\" valueFromSurfaceUpperBound=\"2005-08-08\">2005年8月8日</TIMEX><TIMEX tid=\"t8\" type=\"TIME\" valueFromSurfaceLowerBound=\"T08:45:30\" valueFromSurfaceUpperBound=\"T08:45:30\">午前8時45分30秒</TIMEX>、<TIMEX tid=\"t9\" type=\"TIME\" valueFromSurfaceLowerBound=\"T08:45:30\" valueFromSurfaceUpperBound=\"T08:45:30\">午前8時45分30秒</TIMEX>、<TIMEX tid=\"t10\" type=\"DATE\" valueFromSurfaceLowerBound=\"XXXX-XX-03TDN\" valueFromSurfaceUpperBound=\"XXXX-XX-03TDN\" temporalFunction=\"true\">3日未明</TIMEX>、<TIMEX tid=\"t11\" type=\"DATE\" valueFromSurfaceLowerBound=\"XXXX-XX-04TMO\" valueFromSurfaceUpperBound=\"XXXX-XX-04TMO\" temporalFunction=\"true\">四日朝</TIMEX>、<TIMEX tid=\"t12\" type=\"DATE\" valueFromSurfaceLowerBound=\"XXXX-XX-04TMI\" valueFromSurfaceUpperBound=\"XXXX-XX-04TMI\" temporalFunction=\"true\">四日昼</TIMEX>、<TIMEX tid=\"t13\" type=\"DATE\" valueFromSurfaceLowerBound=\"XXXX-XX-04\" valueFromSurfaceUpperBound=\"XXXX-XX-04\" temporalFunction=\"true\">四日日</TIMEX>中、<TIMEX tid=\"t14\" type=\"DATE\" valueFromSurfaceLowerBound=\"XXXX-XX-04TAF\" valueFromSurfaceUpperBound=\"XXXX-XX-04TAF\" temporalFunction=\"true\">四日午後</TIMEX>、<TIMEX tid=\"t15\" type=\"DATE\" valueFromSurfaceLowerBound=\"XXXX-XX-04TEV\" valueFromSurfaceUpperBound=\"XXXX-XX-04TEV\" temporalFunction=\"true\">四日夕方</TIMEX>、<TIMEX tid=\"t16\" type=\"DATE\" valueFromSurfaceLowerBound=\"XXXX-XX-04TNI\" valueFromSurfaceUpperBound=\"XXXX-XX-04TNI\" temporalFunction=\"true\">四日夜</TIMEX>、<TIMEX tid=\"t17\" type=\"DATE\" valueFromSurfaceLowerBound=\"XXXX-XX-04TMN\" valueFromSurfaceUpperBound=\"XXXX-XX-04TMN\" temporalFunction=\"true\">四日深夜</TIMEX>", text_annotated);
}

TEST_F(NumexpExtractorTest, timex3) {
	string text_annotated;
	string language("ja");
	string text("3年間、2ヶ月、5日, 3時間、30分、9秒80、1週間");
	NormalizeNumexp NN(language);
	NN.normalize_by_timex_format(text, text_annotated);
	cout << "debug:" << text_annotated << endl;
	EXPECT_EQ("<TIMEX tid=\"t0\" type=\"DURATION\" valueFromSurfaceLowerBound=\"P3Y\" valueFromSurfaceUpperBound=\"P3Y\">3年間</TIMEX>、<TIMEX tid=\"t1\" type=\"DURATION\" valueFromSurfaceLowerBound=\"P2M\" valueFromSurfaceUpperBound=\"P2M\">2ヶ月</TIMEX>、<TIMEX tid=\"t2\" type=\"DATE\" valueFromSurfaceLowerBound=\"XXXX-XX-05\" valueFromSurfaceUpperBound=\"XXXX-XX-05\" temporalFunction=\"true\">5日</TIMEX>, <TIMEX tid=\"t3\" type=\"DURATION\" valueFromSurfaceLowerBound=\"PT3H\" valueFromSurfaceUpperBound=\"PT3H\">3時間</TIMEX>、<TIMEX tid=\"t4\" type=\"DURATION\" valueFromSurfaceLowerBound=\"PT30M\" valueFromSurfaceUpperBound=\"PT30M\">30分</TIMEX>、<TIMEX tid=\"t5\" type=\"DURATION\" valueFromSurfaceLowerBound=\"PT9S\" valueFromSurfaceUpperBound=\"PT9S\">9秒</TIMEX>80、<TIMEX tid=\"t6\" type=\"DURATION\" valueFromSurfaceLowerBound=\"P7D\" valueFromSurfaceUpperBound=\"P7D\">1週間</TIMEX>", text_annotated);
}

TEST_F(NumexpExtractorTest, timex4) {
	string text_annotated;
	string language("ja");
	string text("週に2回、毎日、毎10月、10日おき");
	NormalizeNumexp NN(language);
	NN.normalize_by_timex_format(text, text_annotated);
	cout << "debug:" << text_annotated << endl;
	EXPECT_EQ("週に<NUMEX nid=\"n0\" valueLowerBound=\"2\" valueUpperBound=\"2\" counter=\"回\">2回</NUMEX>、毎日、毎<TIMEX tid=\"t0\" type=\"DATE\" valueFromSurfaceLowerBound=\"XXXX-10\" valueFromSurfaceUpperBound=\"XXXX-10\" temporalFunction=\"true\">10月</TIMEX>、<TIMEX tid=\"t1\" type=\"DATE\" valueFromSurfaceLowerBound=\"XXXX-XX-10\" valueFromSurfaceUpperBound=\"XXXX-XX-10\" temporalFunction=\"true\">10日</TIMEX>おき", text_annotated);
}

TEST_F(NumexpExtractorTest, timex5) {
	string text_annotated;
	string language("ja");
	string text("二十世紀初頭、二十世紀半ば、二十世紀末、二十世紀ごろ、2003年4月以前、2003年4月以降、3日以内、3日以上、10個未満、10個余り、ちょうど10個");
	NormalizeNumexp NN(language);
	NN.normalize_by_timex_format(text, text_annotated);
	cout << "debug:" << text_annotated << endl;
	EXPECT_EQ("<TIMEX tid=\"t0\" type=\"DATE\" valueFromSurfaceLowerBound=\"1901\" valueFromSurfaceUpperBound=\"2000\" mod=\"START\">二十世紀初頭</TIMEX>、<TIMEX tid=\"t1\" type=\"DATE\" valueFromSurfaceLowerBound=\"1901\" valueFromSurfaceUpperBound=\"2000\" mod=\"MID\">二十世紀半ば</TIMEX>、<TIMEX tid=\"t2\" type=\"DATE\" valueFromSurfaceLowerBound=\"1901\" valueFromSurfaceUpperBound=\"2000\" mod=\"END\">二十世紀末</TIMEX>、<TIMEX tid=\"t3\" type=\"DATE\" valueFromSurfaceLowerBound=\"1901\" valueFromSurfaceUpperBound=\"2000\" mod=\"APPROX\">二十世紀ごろ</TIMEX>、<TIMEX tid=\"t4\" type=\"DATE\" valueFromSurfaceLowerBound=\"2003-04\" valueFromSurfaceUpperBound=\"2003-04\" mod=\"ON_OR_BEFORE\">2003年4月以前</TIMEX>、<TIMEX tid=\"t5\" type=\"DATE\" valueFromSurfaceLowerBound=\"2003-04\" valueFromSurfaceUpperBound=\"2003-04\" mod=\"ON_OR_AFTER\">2003年4月以降</TIMEX>、<TIMEX tid=\"t6\" type=\"DURATION\" valueFromSurfaceLowerBound=\"P3D\" valueFromSurfaceUpperBound=\"P3D\" mod=\"EQUAL_OR_LESS\">3日以内</TIMEX>、<TIMEX tid=\"t7\" type=\"DURATION\" valueFromSurfaceLowerBound=\"P3D\" valueFromSurfaceUpperBound=\"P3D\" mod=\"EQUAL_OR_MORE\">3日以上</TIMEX>、<NUMEX nid=\"n0\" valueLowerBound=\"10\" valueUpperBound=\"10\" counter=\"個\" mod=\"LESS_THAN\">10個未満</NUMEX>、<NUMEX nid=\"n1\" valueLowerBound=\"10\" valueUpperBound=\"10\" counter=\"個\" mod=\"KYOU\">10個余り</NUMEX>、<NUMEX nid=\"n2\" valueLowerBound=\"10\" valueUpperBound=\"10\" counter=\"個\" mod=\"JUST\">ちょうど10個</NUMEX>", text_annotated);
}

TEST_F(NumexpExtractorTest, timex6) {
	string text_annotated;
	string language("ja");
	string text("三年前、三日前、三時間前、昨年四月、先月四日夜、明日五時");
	NormalizeNumexp NN(language);
	NN.normalize_by_timex_format(text, text_annotated);
	cout << "debug:" << text_annotated << endl;
	EXPECT_EQ("<TIMEX tid=\"t0\" type=\"TIME\" valueFromSurfaceLowerBound=\"TXX:XX:XX,P-3Y\" valueFromSurfaceUpperBound=\"TXX:XX:XX,P-3Y\" temporalFunction=\"true\">三年前</TIMEX>、<TIMEX tid=\"t1\" type=\"TIME\" valueFromSurfaceLowerBound=\"TXX:XX:XX,P-3D\" valueFromSurfaceUpperBound=\"TXX:XX:XX,P-3D\" temporalFunction=\"true\">三日前</TIMEX>、<TIMEX tid=\"t2\" type=\"TIME\" valueFromSurfaceLowerBound=\"TXX:XX:XX,PT-3H\" valueFromSurfaceUpperBound=\"TXX:XX:XX,PT-3H\" temporalFunction=\"true\">三時間前</TIMEX>、<TIMEX tid=\"t3\" type=\"DATE\" valueFromSurfaceLowerBound=\"XXXX-04,P-1Y\" valueFromSurfaceUpperBound=\"XXXX-04,P-1Y\" temporalFunction=\"true\">昨年四月</TIMEX>、<TIMEX tid=\"t4\" type=\"DATE\" valueFromSurfaceLowerBound=\"XXXX-XX-04,P-1M\" valueFromSurfaceUpperBound=\"XXXX-XX-04,P-1M\" temporalFunction=\"true\" mod=\"NI\">先月四日夜</TIMEX>、<TIMEX tid=\"t5\" type=\"TIME\" valueFromSurfaceLowerBound=\"T05:XX:XX,P1D\" valueFromSurfaceUpperBound=\"T05:XX:XX,P1D\" temporalFunction=\"true\">明日五時</TIMEX>", text_annotated);
}

/*
tests for English
*/

TEST_F(NumexpExtractorTest, english1) {
	vector<string> result;
	string language("en");
	string text("He gave $30,000 to his friend at the bank");
	NormalizeNumexp NN(language);
	NN.normalize(text, result);
	ASSERT_EQ(1u, result.size());
	EXPECT_EQ("numerical*$30,000*8*15*$*30000*30000*", result[0]);
}

TEST_F(NumexpExtractorTest, english2) {
	vector<string> result;
	string language("en");
	string text("This is 30,000,000 kg and about 1,000 mm².");
	NormalizeNumexp NN(language);
	NN.normalize(text, result);
	ASSERT_EQ(2u, result.size());
	EXPECT_EQ("numerical*30,000,000 kg *8*22*g*3e+10*3e+10*", result[0]);
	EXPECT_EQ("numerical*about 1,000 mm\xC2\xB2.*26*42*m2*0.001*0.001*APPROX", result[1]);	
}

#include "numerical_expression_extractor.hpp"
#include <vector>
#include <string>
#include <iostream>
using namespace numerical_expression_extractor;
using namespace std;

int main(){
    NumericalExpressionExtractor nee("ja");
    int i = 0;
	string sentence("私は三人の");
	sentence = "【今日から開催】The Fruits of Adventures @ ZEIT-FOTO SALON(東京・京橋)  4/26(Tue)まで";
    vector<string> result;
	nee.extract_numerical_expression(sentence, "numerical", result);
	cout << result.size() << endl;
	return 0;
	
    while(1) {
        i++;
        if(i>10000) break;
        cout << i << endl;
        nee.extract_numerical_expression("私は3人の三匹の貝になりたい", "numerical", result);
		//nee.extract_numerical_expression("私は貝になりたい", "numerical", result);
    }
}

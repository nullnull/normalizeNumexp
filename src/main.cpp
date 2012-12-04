#include "numerical_expression_extractor.hpp"
#include <vector>
#include <string>
#include <iostream>
using namespace numerical_expression_extractor;
using namespace std;

int main(){
  NumericalExpressionExtractor nee("ja");
  string sentence;
  vector<string> result;
	
  while(1) {
    sentence = "";
    cin >> sentence;
    if(sentence.empty()) break;
    nee.extract_numerical_expression(sentence, result);

    //show results
    for(int i=0; i<result.size(); i++){
      cout << result[i] << endl;
    }	
    cout << "END" << endl;
  }
}

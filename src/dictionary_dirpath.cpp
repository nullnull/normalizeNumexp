//TODO : 辞書のパスの設定がわからない。こんな方法しかない？

#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include "dictionary_dirpath.hpp"

namespace dictionary_dirpath {
  void split_by_colon(std::string str, std::vector<std::string>& ret){
	  ret.clear();
	  std::string tmp;
	  for(int i=0; i<static_cast<int>(str.size()); i++){
		  if(str[i] == ':'){
			  ret.push_back(tmp);
			  tmp.clear();
		  }else{
			  tmp += str[i];
		  }
	  }
  }
  
  std::string get_dictionary_dirpath(){
    std::vector<std::string> dictionary_dirpaths;
    std::string envpath_org = std::getenv("PATH");
	std::vector<std::string> envpaths;
	split_by_colon(envpath_org, envpaths);
	for(int i=0; i<static_cast<int>(envpaths.size()); i++){
		dictionary_dirpaths.push_back(envpaths[i] + "/../lib/normalizeNumexp/dic/");	
	}
    dictionary_dirpaths.push_back("../../src/dic/");

    for(int i=0; i<static_cast<int>(dictionary_dirpaths.size()); i++){
      std::string filepath = dictionary_dirpaths[i] + "dic.txt";
      std::ifstream in(filepath.c_str());
      if(in){
	return dictionary_dirpaths[i];
      }
    }
    
    std::cout << "!!!error cannot find dictionary!!!" << std::endl;
    std::cout << "please set dictionary at $PATH/../lib/normalizeNumexp/dic/" << std::endl;
    std::cout << "searched dirpaths:" << std::endl;
    for(int i=0; i<static_cast<int>(dictionary_dirpaths.size()); i++){
      std::cout << "    " << dictionary_dirpaths[i] << std::endl;
    }
    return "";
  }

}// dictionary_dirpath

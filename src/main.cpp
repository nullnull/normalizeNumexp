#include "normalize_numexp.hpp"
#include "optparse.h"
#include <iostream>
using namespace normalize_numexp;

class option : public optparse
{
public:
	bool help, version, cnt;
	std::string show;
	
public:
	option()
	: help(false), version(false) {}
	
	BEGIN_OPTION_MAP_INLINE()
	
	ON_OPTION(SHORTOPT('v') || LONGOPT("version"))
	version = true;
	
	ON_OPTION(SHORTOPT('h') || LONGOPT("help"))
	help = true;
	
	END_OPTION_MAP()
};

int usage(std::ostream& os, const char *argv0)
{
	os << "USAGE: " << argv0 << " [OPTIONS]" << std::endl;
	os << "This utility normalize (Japanese) numerical and temporal expressions in the input sentence." << std::endl;
	os << std::endl;
	os << "OPTIONS:" << std::endl;
	os << "  -v, --version         show this version information and exit" << std::endl;
	os << "  -h, --help            show this help message and exit" << std::endl;
	os << std::endl;
	return 0;
}

int version(std::ostream& os)
{
	os << NORMALIZENUMEXP_NAME;
	os << NORMALIZENUMEXP_VERSION << " ";
	os << NORMALIZENUMEXP_COPYRIGHT << std::endl;
	os << std::endl;
	return 0;
}


int main(int argc, char * argv[]){
	option opt;
	try { 
		int arg_used = opt.parse(argv, argc);
	} catch (const optparse::unrecognized_option& e) {
		std::cerr << "ERROR: unrecognized option: " << e.what() << std::endl;
		return 1;
	} catch (const optparse::invalid_value& e) {
		std::cerr << "ERROR: " << e.what() << std::endl;
		return 1;
	}
	if(opt.help){
		usage(std::cerr, argv[0]);
		return 1;
	}else if(opt.version){
		version(std::cerr);
		return 1;
	}
	
  NormalizeNumexp NN("ja");
  std::string sentence;
  std::vector<std::string> result;
	
  while(1) {
    sentence = "";
    std::cin >> sentence;
    if(sentence.empty()) break;
    NN.normalize(sentence, result);

    //show results
    for(int i=0; i<static_cast<int>(result.size()); i++){
      std::cout << result[i] << std::endl;
    }	
    std::cout << "END" << std::endl;
  }
}

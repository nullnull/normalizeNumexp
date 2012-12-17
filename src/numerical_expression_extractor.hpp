#include "numerical_expression_normalizer.hpp"
#include "abstime_expression_normalizer.hpp"
#include "reltime_expression_normalizer.hpp"
#include "duration_expression_normalizer.hpp"

namespace numerical_expression_extractor{

	struct InappropriateStrings {
		template <class Archive>
		void serialize(Archive &ar){
			ar & MEMBER(str);
		}
		std::string str;
	};

	class NumericalExpressionExtractor{
	public:
		NumericalExpressionExtractor(const std::string& language);
		void extract_numerical_expression(const std::string& text, std::vector<std::string>& result);
		
	private:
		void normalize_each_type_expressions(const std::string& text,
											 std::vector<numerical_expression_normalizer::NumericalExpression>& numexps,
											 std::vector<abstime_expression_normalizer::AbstimeExpression>& abstimeexps,
											 std::vector<reltime_expression_normalizer::ReltimeExpression>& reltimeexps,
											 std::vector<duration_expression_normalizer::DurationExpression>& durationexps);
											 
		void init_inappropriate_stringss(const std::string& language);
		void delete_inappropriate_extraction(std::vector<numerical_expression_normalizer::NumericalExpression>& numexps,
																				 std::vector<abstime_expression_normalizer::AbstimeExpression>& abstimeexps,
																				 std::vector<reltime_expression_normalizer::ReltimeExpression>& reltimeexps,
																				 std::vector<duration_expression_normalizer::DurationExpression>& durationexps);
		void delete_inappropriate_extraction_using_dictionary(std::vector<numerical_expression_normalizer::NumericalExpression>& numexps,
																													std::vector<abstime_expression_normalizer::AbstimeExpression>& abstimeexps,
																													std::vector<reltime_expression_normalizer::ReltimeExpression>& reltimeexps,
																													std::vector<duration_expression_normalizer::DurationExpression>& durationexps);
		template <class AnyTypeExpression>
		void delete_inappropriate_extraction_using_dictionary_one_type(std::vector<AnyTypeExpression>& any_type_expressions);
		
		
		numerical_expression_normalizer::NumericalExpressionNormalizer NEN;
		abstime_expression_normalizer::AbstimeExpressionNormalizer AEN;
		reltime_expression_normalizer::ReltimeExpressionNormalizer REN;
		duration_expression_normalizer::DurationExpressionNormalizer DEN;
//		vector<InappropriateStrings> inappropriate_stringss;
		std::map<std::string, bool> inappropriate_strings_to_bool;
	};
} //namespace numerical_expression_extractor



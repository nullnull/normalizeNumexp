#ifndef INAPPROPRIATE_EXPRESSION_REMOVER_H_
#define INAPPROPRIATE_EXPRESSION_REMOVER_H_
#include "numerical_expression_normalizer.hpp"
#include "abstime_expression_normalizer.hpp"
#include "reltime_expression_normalizer.hpp"
#include "duration_expression_normalizer.hpp"

namespace inappropriate_expression_remover{
	struct InappropriateStrings {
		template <class Archive>
		void serialize(Archive &ar){
			ar & MEMBER(str);
		}
		std::string str;
	};
	
	class InappropriateExpressionRemover{
	public:
		InappropriateExpressionRemover(const std::string& language);
		void remove_inappropriate_extraction(const std::string& text,
																				 std::vector<numerical_expression_normalizer::NumericalExpression>& numexps,
																				 std::vector<abstime_expression_normalizer::AbstimeExpression>& abstimeexps,
																				 std::vector<reltime_expression_normalizer::ReltimeExpression>& reltimeexps,
																				 std::vector<duration_expression_normalizer::DurationExpression>& durationexps);
	private:
		template <class AnyTypeExpression>
		void delete_inappropriate_extraction_using_dictionary_one_type(std::vector<AnyTypeExpression>& any_type_expressions);
		template <class AnyTypeExpression>
		bool is_url_strings(const std::string& text, const AnyTypeExpression& any_type_expression);
		template <class AnyTypeExpression>
		void delete_url_strings(const std::string& text, std::vector<AnyTypeExpression>& any_type_expressions);
		void delete_inappropriate_extraction_using_dictionary(const std::string& text,
																													 std::vector<numerical_expression_normalizer::NumericalExpression>& numexps,
																													 std::vector<abstime_expression_normalizer::AbstimeExpression>& abstimeexps,
																													 std::vector<reltime_expression_normalizer::ReltimeExpression>& reltimeexps,
																													 std::vector<duration_expression_normalizer::DurationExpression>& durationexps);
		void init_inappropriate_stringss(const std::string& language);
		void init_url_strings();
		
		std::map<std::string, bool> inappropriate_strings_to_bool;
		std::map<std::string, bool> url_strings_to_bool;
	};
} //namespace inappropriate_expression_remover.hpp

#endif


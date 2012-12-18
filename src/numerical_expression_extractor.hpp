#include "inappropriate_expression_remover.hpp"

namespace numerical_expression_extractor{

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
		
		numerical_expression_normalizer::NumericalExpressionNormalizer NEN;
		abstime_expression_normalizer::AbstimeExpressionNormalizer AEN;
		reltime_expression_normalizer::ReltimeExpressionNormalizer REN;
		duration_expression_normalizer::DurationExpressionNormalizer DEN;
		inappropriate_expression_remover::InappropriateExpressionRemover IER;
	};
} //namespace numerical_expression_extractor



#define NORMALIZENUMEXP_NAME "normalizeNumexp"
#define NORMALIZENUMEXP_VERSION "3.0"
#define NORMALIZENUMEXP_COPYRIGHT "Copyright (c) 2012 Katsuma Narisawa"

#include "inappropriate_expression_remover.hpp"

namespace normalize_numexp{

	class NormalizeNumexp{
	public:
		NormalizeNumexp(const std::string& language);
		void normalize(const std::string& text, std::vector<std::string>& result);
		void normalize_by_timex_format(const std::string& text, std::string& text_annotated);
		
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
} //namespace normalize_numexp



#ifndef DURATION_EXPRESSION_NORMALIZER_H_
#define DURATION_EXPRESSION_NORMALIZER_H_
#include <string>
#include "digit_utility.hpp"
#include "number_normalizer.hpp"
#include "normalizer_utility.hpp"
#include "normalizer_template.hpp"
#include <ux/ux.hpp>

namespace duration_expression_normalizer {
  
struct DurationExpression : normalizer_utility::NormalizedExpressionTemplate{
  DurationExpression(digit_utility::Number number)
  : normalizer_utility::NormalizedExpressionTemplate(number.original_expression, number.position_start, number.position_end),
    org_value_lowerbound(number.value_lowerbound),
    org_value_upperbound(number.value_upperbound),
    value_lowerbound(normalizer_utility::Time(INFINITY)),
    value_upperbound(normalizer_utility::Time(-INFINITY)),
    ordinary(false)
  {}
  
  double org_value_lowerbound, org_value_upperbound;
  normalizer_utility::Time value_lowerbound, value_upperbound;
  bool ordinary;
};

  
class LimitedDurationExpression : public normalizer_utility::LimitedExpressionTemplate{
public:
  template <class Archive>
  void serialize(Archive &ar){
    ar & MEMBER(pattern) & MEMBER(corresponding_time_position) & MEMBER(process_type) & MEMBER(ordinary) & MEMBER(option);
  }
  
  std::vector<std::string> corresponding_time_position;
  std::vector<std::string> process_type;
};

  
class DurationExpressionNormalizer : public normalizer_template::NormalizerTemplate<DurationExpression, LimitedDurationExpression>{
public:
  DurationExpressionNormalizer(const std::string& language) : NN(language) { language_ = language; init(); }
  
private:
  void init();
  void normalize_number(const std::string& text, std::vector<digit_utility::Number>& numbers);
  void revise_any_type_expression_by_matching_limited_expression(std::vector<DurationExpression>& durationexps, int& expression_id, LimitedDurationExpression matching_limited_duration_expression);
  void revise_any_type_expression_by_matching_prefix_counter(DurationExpression& any_type_expression, const LimitedDurationExpression& matching_limited_expression);
  void revise_any_type_expression_by_number_modifier(DurationExpression& durationexp, const normalizer_utility::NumberModifier& number_modifier);
  void delete_not_any_type_expression(std::vector<DurationExpression>& durationexps);
  void fix_by_range_expression(const pfi::data::string::ustring& utext, std::vector<DurationExpression>& durationexps);
  
  number_normalizer::NumberNormalizer NN;
};
  
} //namespace duration_expression_normalizer

#endif //RELTIME_EXPRESSON_NORMALIZER_H_

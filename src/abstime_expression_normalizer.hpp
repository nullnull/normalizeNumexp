#ifndef ABSTIME_EXPRESSION_NORMALIZER_H_
#define ABSTIME_EXPRESSION_NORMALIZER_H_
#include <string>
#include "digit_utility.hpp"
#include "number_normalizer.hpp"
#include "normalizer_utility.hpp"
#include "normalizer_template.hpp"
#include <ux/ux.hpp>

namespace abstime_expression_normalizer{

struct AbstimeExpression : normalizer_utility::NormalizedExpressionTemplate{
  AbstimeExpression(digit_utility::Number number)
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

  
class LimitedAbstimeExpression : public normalizer_utility::LimitedExpressionTemplate{
public:
  template <class Archive>
  void serialize(Archive &ar){
    ar & MEMBER(pattern) & MEMBER(corresponding_time_position) & MEMBER(process_type) & MEMBER(ordinary) & MEMBER(option);
  }

  std::vector<std::string> corresponding_time_position;
  std::vector<std::string> process_type;
};

  
class AbstimeExpressionNormalizer : public normalizer_template::NormalizerTemplate<AbstimeExpression, LimitedAbstimeExpression>{
public:
  AbstimeExpressionNormalizer(const std::string& language) : NN(language) { language_ = language; init(); }

private:
  void init();
  void normalize_number(const std::string& text, std::vector<digit_utility::Number>& numbers);
  void revise_any_type_expression_by_matching_limited_expression(std::vector<AbstimeExpression>& abstimeexps, int& expression_id, LimitedAbstimeExpression matching_limited_abstime_expression);
  void revise_any_type_expression_by_matching_prefix_counter(AbstimeExpression& any_type_expression, const LimitedAbstimeExpression& matching_limited_expression);
  void revise_any_type_expression_by_number_modifier(AbstimeExpression& abstimeexp, const normalizer_utility::NumberModifier& number_modifier);
  void delete_not_any_type_expression(std::vector<AbstimeExpression>& abstimeexps);
  void fix_by_range_expression(const pfi::data::string::ustring& utext, std::vector<AbstimeExpression>& abstimeexps);
  
  number_normalizer::NumberNormalizer NN;
};

} //namespace abstime_expression_normalizer

#endif //ABSTIME_EXPRESSON_NORMALIZER_H_

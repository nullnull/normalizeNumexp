#ifndef RELTIME_EXPRESSION_NORMALIZER_H_
#define RELTIME_EXPRESSION_NORMALIZER_H_
#include <string>
#include "digit_utility.hpp"
#include "number_normalizer.hpp"
#include "normalizer_utility.hpp"
#include "normalizer_template.hpp"
#include <ux/ux.hpp>

namespace reltime_expression_normalizer{
  
struct ReltimeExpression : normalizer_utility::NormalizedExpressionTemplate{
  ReltimeExpression(digit_utility::Number number)
  : normalizer_utility::NormalizedExpressionTemplate(number.original_expression, number.position_start, number.position_end),
    org_value_lowerbound(number.value_lowerbound),
    org_value_upperbound(number.value_upperbound),
    value_lowerbound_abs(normalizer_utility::Time(INFINITY)),
    value_upperbound_abs(normalizer_utility::Time(-INFINITY)),
    value_lowerbound_rel(normalizer_utility::Time(INFINITY)),
    value_upperbound_rel(normalizer_utility::Time(-INFINITY)),
  ordinary(false)
  {}
  
  double org_value_lowerbound, org_value_upperbound;
  normalizer_utility::Time value_lowerbound_abs, value_upperbound_abs;
  normalizer_utility::Time value_lowerbound_rel, value_upperbound_rel;
  bool ordinary;
};

  
class LimitedReltimeExpression : public normalizer_utility::LimitedExpressionTemplate{
public:
  template <class Archive>
  void serialize(Archive &ar){
    ar & MEMBER(pattern) & MEMBER(corresponding_time_position) & MEMBER(process_type) & MEMBER(ordinary) & MEMBER(option);
  }
  
  std::vector<std::string> corresponding_time_position;
  std::vector<std::string> process_type;
};
  

class ReltimeExpressionNormalizer : public normalizer_template::NormalizerTemplate<ReltimeExpression, LimitedReltimeExpression>{
public:
  ReltimeExpressionNormalizer(const std::string& language) : NN(language) { language_ = language; init(); }
  
private:
  void init();
  void normalize_number(const std::string& text, std::vector<digit_utility::Number>& numbers);
  void revise_any_type_expression_by_matching_limited_expression(std::vector<ReltimeExpression>& reltimeexps, int& expression_id, LimitedReltimeExpression matching_limited_reltime_expression);
  void revise_any_type_expression_by_matching_prefix_counter(ReltimeExpression& reltimeexp, const LimitedReltimeExpression& matching_limited_expression);
  void revise_any_type_expression_by_number_modifier(ReltimeExpression& reltimeexp, const normalizer_utility::NumberModifier& number_modifier);
  void delete_not_any_type_expression(std::vector<ReltimeExpression>& reltimeexps);
  void fix_by_range_expression(const pfi::data::string::ustring& utext, std::vector<ReltimeExpression>& reltimeexps);
  
  number_normalizer::NumberNormalizer NN;
};
  
} //namespace reltime_expression_normalizer

#endif //RELTIME_EXPRESSON_NORMALIZER_H_

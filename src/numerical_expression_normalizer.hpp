#ifndef NUMERICAL_EXPRESSION_NORMALIZER_H_
#define NUMERICAL_EXPRESSION_NORMALIZER_H_
#include <string>
#include <ux/ux.hpp>
#include "digit_utility.hpp"
#include "number_normalizer.hpp"
#include "normalizer_utility.hpp"
#include "normalizer_template.hpp"

namespace numerical_expression_normalizer{

struct NumericalExpression : normalizer_utility::NormalizedExpressionTemplate{
  NumericalExpression(const pfi::data::string::ustring& original_expression, // TODO : 実装方針が変わったので、この初期化リストはテストでしか用いていない。テストを変更して、これは削る。
         const int position_start,
         const int position_end,
         const double value_lowerbound,
         const double value_upperbound)
      : normalizer_utility::NormalizedExpressionTemplate(original_expression, position_start, position_end),
        value_lowerbound(value_lowerbound),
        value_upperbound(value_upperbound),
        counter(pfi::data::string::string_to_ustring("")),
        ordinary(false)
        {}
  
  NumericalExpression(digit_utility::Number number)
      : normalizer_utility::NormalizedExpressionTemplate(number.original_expression, number.position_start, number.position_end),
        value_lowerbound(number.value_lowerbound),
        value_upperbound(number.value_upperbound),
        counter(pfi::data::string::string_to_ustring("")),
        ordinary(false)
        {}

  double value_lowerbound, value_upperbound;
  pfi::data::string::ustring counter;
  bool ordinary;
};

  
struct Counter : public normalizer_utility::LimitedExpressionTemplate{
  template <class Archive>
  void serialize(Archive &ar){
    ar & MEMBER(pattern) & MEMBER(counter) & MEMBER(SI_prefix) & MEMBER(optional_power_of_ten) & MEMBER(ordinary) & MEMBER(option);
  }

  std::string counter;
  int SI_prefix; //「キロ」「ミリ」などの表記に使用
  int optional_power_of_ten; //「トン」のような特殊な表記（数に10^4を乗算する必要がある）の時に使用
};

  
class NumericalExpressionNormalizer : public normalizer_template::NormalizerTemplate<NumericalExpression, Counter>{
public:
  NumericalExpressionNormalizer(const std::string& language) : NN(language) { language_ = language; init(); }

private:
  void init();
  void normalize_number(const std::string& text, std::vector<digit_utility::Number>& numbers);
  void revise_any_type_expression_by_matching_limited_expression(std::vector<NumericalExpression>& numexps, int& expression_id, Counter matching_limited_expression);
  void revise_any_type_expression_by_matching_prefix_counter(NumericalExpression& numexps, const Counter& matching_limited_expression);
  void revise_any_type_expression_by_number_modifier(NumericalExpression& numexp, const normalizer_utility::NumberModifier& number_modifier);
  void delete_not_any_type_expression(std::vector<NumericalExpression>& numexps);
  void fix_by_range_expression(const pfi::data::string::ustring& utext, std::vector<NumericalExpression>& numexps);
  
  number_normalizer::NumberNormalizer NN;
};

} //namespace numerical_expression_normalizer

#endif //NUMERICAL_EXPRESSION_NORMALIZER_H_

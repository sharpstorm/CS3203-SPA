#include "AbstractTwoArgClause.h"

#include <utility>

AbstractTwoArgClause::AbstractTwoArgClause(
    ClauseArgumentPtr left,
    ClauseArgumentPtr right) :
    left(std::move(left)), right(std::move(right)) {}

const PQLSynonymNameList AbstractTwoArgClause::getUsedSynonyms() const {
  PQLSynonymNameList result;
  if (left->isNamed()) {
    result.push_back(left->getName());
  }
  if (right->isNamed()) {
    result.push_back(right->getName());
  }
  return result;
}

bool AbstractTwoArgClause::isSameSynonym() const {
  return left->isNamed() && right->isNamed()
      && left->getName() == right->getName();
}

ComplexityScore AbstractTwoArgClause::computeComplexityScore(
    const OverrideTable *table) const {
  return computeComplexityScore<
      COMPLEXITY_MODIFIER_NONE,
      COMPLEXITY_MODIFIER_NONE,
      COMPLEXITY_MODIFIER_NONE>(table);
}

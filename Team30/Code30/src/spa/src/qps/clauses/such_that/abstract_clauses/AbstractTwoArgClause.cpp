#include "AbstractTwoArgClause.h"

#include <utility>

AbstractTwoArgClause::AbstractTwoArgClause(
    ClauseArgumentPtr left,
    ClauseArgumentPtr right):
    left(std::move(left)), right(std::move(right)) {}

SynonymList AbstractTwoArgClause::getUsedSynonyms() {
  SynonymList result;
  if (left->isNamed()) {
    result.push_back(left->getName());
  }
  if (right->isNamed()) {
    result.push_back(right->getName());
  }
  return result;
}

bool AbstractTwoArgClause::isSameSynonym() {
  return left->isNamed() && right->isNamed()
      && left->getName() == right->getName();
}

bool AbstractTwoArgClause::canSubstitute(OverrideTable* table,
                                         PQLSynonymName synName) {
  return table->find(synName) != table->end();
}

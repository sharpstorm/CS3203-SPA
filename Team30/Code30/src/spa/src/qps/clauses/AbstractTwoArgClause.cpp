#include "AbstractTwoArgClause.h"
#include <utility>

using std::move;

AbstractTwoArgClause::AbstractTwoArgClause(
    ClauseArgumentPtr left,
    ClauseArgumentPtr right):
    left(move(left)), right(move(right)) {}

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

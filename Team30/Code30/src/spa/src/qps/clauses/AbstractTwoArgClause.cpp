#include "AbstractTwoArgClause.h"

using std::move;

AbstractTwoArgClause::AbstractTwoArgClause(
    ClauseArgumentPtr left,
    ClauseArgumentPtr right):
    left(move(left)), right(move(right)) {}

bool AbstractTwoArgClause::usesSynonym(string varName) {
  return left->isSynonymCalled(varName) || right->isSynonymCalled(varName);
}

bool AbstractTwoArgClause::isSameSynonym() {
  return left->isNamed() && right->isNamed()
      && left->getName() == right->getName();
}

#include <memory>
#include <utility>

#include "CallsTClause.h"

using std::move;

CallsTClause::CallsTClause(ClauseArgumentPtr left, ClauseArgumentPtr right)
    : AbstractTwoArgClause(move(left), move(right)) {

}
PQLQueryResult *CallsTClause::evaluateOn(shared_ptr<PkbQueryHandler> pkbQueryHandler) {
  return nullptr;
}
bool CallsTClause::validateArgTypes(VariableTable *variables) {
  return false;
}

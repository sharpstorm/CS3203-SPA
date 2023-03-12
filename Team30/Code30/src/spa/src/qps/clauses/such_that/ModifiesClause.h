#pragma once

#include <utility>

#include "abstract_clauses/AbstractAnyEntClause.h"
#include "qps/clauses/SuchThatClause.h"

typedef StmtEntInvoker ModifiesSInvoker;
typedef EntEntInvoker ModifiesPInvoker;

constexpr ModifiesSInvoker modifiesSInvoker = [](
    PkbQueryHandler* pkbQueryHandler,
    const StmtRef &leftArg,
    const EntityRef &rightArg){
  return pkbQueryHandler->queryModifies(leftArg, rightArg);
};

constexpr ModifiesPInvoker modifiesPInvoker = [](
    PkbQueryHandler* pkbQueryHandler,
    const EntityRef &leftArg,
    const EntityRef &rightArg){
  return pkbQueryHandler->queryModifies(leftArg, rightArg);
};

typedef AbstractAnyEntClause<
    modifiesPInvoker,
    modifiesSInvoker,
    ClauseArgument::isEither<ClauseArgument::isStatement,
                             ClauseArgument::isType<PQL_SYN_TYPE_PROCEDURE>>,
    ClauseArgument::isType<PQL_SYN_TYPE_VARIABLE>> AbstractModifiesClause;

class ModifiesClause: public AbstractModifiesClause {
 public:
  ModifiesClause(ClauseArgumentPtr left, ClauseArgumentPtr right)
      : AbstractAnyEntClause(std::move(left), std::move(right)) {
  }
};

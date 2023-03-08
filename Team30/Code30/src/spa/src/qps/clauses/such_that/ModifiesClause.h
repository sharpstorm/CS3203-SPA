#pragma once

#include <string>

#include "qps/clauses/SuchThatClause.h"
#include "qps/clauses/arguments/ClauseArgument.h"
#include "qps/clauses/AbstractTwoArgClause.h"
#include "abstract_clauses/AbstractAnyEntClause.h"

typedef StmtEntInvoker ModifiesSInvoker;
typedef EntEntInvoker ModifiesPInvoker;

constexpr ModifiesSInvoker modifiesSInvoker = [](PkbQueryHandler* pkbQueryHandler,
                                                 StmtRef leftArg,
                                                 EntityRef rightArg){
  return pkbQueryHandler->queryModifies(leftArg, rightArg);
};

constexpr ModifiesPInvoker modifiesPInvoker = [](PkbQueryHandler* pkbQueryHandler,
                                                 EntityRef leftArg,
                                                 EntityRef rightArg){
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

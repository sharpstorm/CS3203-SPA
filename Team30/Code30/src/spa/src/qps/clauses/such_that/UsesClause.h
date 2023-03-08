#pragma once

#include <string>

#include "qps/clauses/SuchThatClause.h"
#include "qps/clauses/arguments/ClauseArgument.h"
#include "qps/clauses/AbstractTwoArgClause.h"
#include "qps/clauses/such_that/abstract_clauses/AbstractAnyEntClause.h"

typedef StmtEntInvoker UsesSInvoker;
typedef EntEntInvoker UsesPInvoker;

constexpr UsesSInvoker usesSInvoker = [](PkbQueryHandler* pkbQueryHandler,
                                         StmtRef leftArg,
                                         EntityRef rightArg){
  return pkbQueryHandler->queryUses(leftArg, rightArg);
};

constexpr UsesPInvoker usesPInvoker = [](PkbQueryHandler* pkbQueryHandler,
                                         EntityRef leftArg,
                                         EntityRef rightArg){
  return pkbQueryHandler->queryUses(leftArg, rightArg);
};

typedef AbstractAnyEntClause<
    usesPInvoker,
    usesSInvoker,
    ClauseArgument::isEither<ClauseArgument::isStatement,
                             ClauseArgument::isType<PQL_SYN_TYPE_PROCEDURE>>,
    ClauseArgument::isType<PQL_SYN_TYPE_VARIABLE>> AbstractUsesClause;

class UsesClause: public AbstractUsesClause {
 public:
  UsesClause(ClauseArgumentPtr left, ClauseArgumentPtr right)
  : AbstractAnyEntClause(std::move(left), std::move(right)) {
  }
};

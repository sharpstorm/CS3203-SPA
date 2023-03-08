#pragma once

#include <memory>
#include <string>

#include "qps/clauses/arguments/ClauseArgument.h"
#include "common/Types.h"
#include "qps/clauses/Clause.h"
#include "qps/clauses/SuchThatClause.h"

class AbstractTwoArgClause:
    public SuchThatClause {
 protected:
  ClauseArgumentPtr left;
  ClauseArgumentPtr right;

  bool isSameSynonym();

 public:
  AbstractTwoArgClause(ClauseArgumentPtr left, ClauseArgumentPtr right);
  SynonymList getUsedSynonyms() override;
};

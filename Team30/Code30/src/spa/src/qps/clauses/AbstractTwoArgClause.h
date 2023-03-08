#pragma once

#include <memory>
#include <string>

#include "arguments/ClauseArgument.h"
#include "common/Types.h"
#include "Clause.h"
#include "SuchThatClause.h"

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

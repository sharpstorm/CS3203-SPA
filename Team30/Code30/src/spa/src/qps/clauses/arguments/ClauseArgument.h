#pragma once

#include <functional>
#include <memory>
#include "../../common/PQLQuerySynonym.h"
#include "common/Types.h"

using std::function, std::unique_ptr;

using SynonymPredicate = function<bool(PQLQuerySynonym)>;

class ClauseArgument {
 public:
  virtual ~ClauseArgument() = default;
  virtual bool synonymSatisfies(SynonymPredicate predicate);
  virtual bool isSynonymCalled(PQLSynonymName name);

  virtual bool isNamed();
  virtual bool isWildcard();

  virtual PQLSynonymName getName();
  virtual StmtRef toStmtRef() = 0;
  virtual EntityRef toEntityRef() = 0;

  static SynonymPredicate isStatement;

  template<PQLSynonymType type>
  constexpr static bool(*isType)(PQLQuerySynonym) = [](PQLQuerySynonym syn) {
    return syn.isType(type);
  };
};

using ClauseArgumentPtr = unique_ptr<ClauseArgument>;

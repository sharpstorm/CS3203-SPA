#pragma once

#include <functional>
#include "ClauseArgument.h"
#include "../../common/PQLQuerySynonym.h"

using std::function;

class SynonymArgument: public ClauseArgument {
 private:
  PQLQuerySynonym synonym;

 public:
  explicit SynonymArgument(PQLQuerySynonym synonym);
  bool synonymSatisfies(SynonymPredicate predicate) override;
  bool isSynonymCalled(PQLSynonymName name) override;
  bool isNamed() override;
  void invokeWithName(function<void(PQLSynonymName)> consumer);
  StmtRef toStmtRef();
  EntityRef toEntityRef();
};

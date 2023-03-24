#pragma once
#include <string>
#include "qps/common/PQLQuerySynonym.h"
#include "common/Types.h"

using std::string;

class OverrideTransformer {
 private:
  int stmtRef;
  string entRef;
 public:
  explicit OverrideTransformer(int stmt): stmtRef(stmt), entRef(NO_ENT) {}
  explicit OverrideTransformer(string ent): stmtRef(NO_STMT), entRef(ent) {}
  bool returnsInteger() const;
  StmtRef transformArg(StmtRef input) const;
  EntityRef transformArg(EntityRef input) const;
};

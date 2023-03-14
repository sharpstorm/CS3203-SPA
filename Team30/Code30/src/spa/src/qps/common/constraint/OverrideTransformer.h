#pragma once
#include <string>
#include "qps/common/PQLQuerySynonym.h"
#include "common/Types.h"

using std::string;

class OverrideTransformer {
 private:
  static const int NO_STMT_REF = -1;
  static const char NO_ENT_REF[];
  int stmtRef;
  string entRef;
 public:
  explicit OverrideTransformer(int stmt): stmtRef(stmt), entRef(NO_ENT_REF) {}
  explicit OverrideTransformer(string ent): stmtRef(NO_STMT_REF), entRef(ent) {}
  bool returnsInteger();
  StmtRef transformArg(StmtRef input);
  EntityRef transformArg(EntityRef input);
};

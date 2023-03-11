#pragma once

#include <memory>
#include <string>

#include "qps/common/AttributedSynonym.h"

using std::string, std::unique_ptr;

class WithArgument {
  static const int NO_STMT_REF = -1;
  static const char NO_ENT_REF[];
  AttributedSynonymPtr syn;
  int intValue;
  string identValue;
 public:
  explicit WithArgument(int intVal);
  explicit WithArgument(string identVal);
  explicit WithArgument(AttributedSynonymPtr syn);
  PQLSynonymName getSynName();
  PQLSynonymType getSynType();
  PQLSynonymAttribute getAttribute();
  bool doesReturnInteger();
  bool isAttributeValid();
  bool isSyn();
};

typedef unique_ptr<WithArgument> WithArgumentPtr;
#pragma once

#include <memory>
#include <string>

#include "qps/common/AttributedSynonym.h"
#include "qps/constraints/OverrideTransformer.h"

using std::string, std::unique_ptr;

class WithArgument {
  AttributedSynonymPtr syn;
  int intValue;
  string identValue;

 public:
  explicit WithArgument(const int &intVal);
  explicit WithArgument(const string &identVal);
  explicit WithArgument(AttributedSynonymPtr syn);

  bool isDefaultAttribute();
  bool doesReturnInteger();

  PQLSynonymName getSynName();
  bool isSynStatement();
  bool isSameSynTypeAs(const WithArgument *other);

  bool isStaticValueEqual(const WithArgument &other);

  bool isSyn() const;

  int toConstInt();
  string toConstIdent();
  AttributedSynonym toAttrSyn();
};

typedef unique_ptr<WithArgument> WithArgumentPtr;

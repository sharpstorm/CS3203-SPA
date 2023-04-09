#pragma once

#include <memory>

#include "qps/common/synonym/AttributedSynonym.h"
#include "qps/common/constraint/OverrideTransformer.h"

using std::unique_ptr;

class WithArgument {
  AttributedSynonymPtr syn;
  IntegerValue intValue;
  EntityValue identValue;

 public:
  explicit WithArgument(const IntegerValue &intVal);
  explicit WithArgument(const EntityValue &identVal);
  explicit WithArgument(AttributedSynonymPtr syn);

  bool isDefaultAttribute() const;
  bool returnsInteger() const;

  PQLSynonymName getSynName() const;
  bool isSynStatement() const;
  bool isSameSynTypeAs(const WithArgument *other) const;
  bool isStaticValueEqual(const WithArgument &other) const;

  bool isSyn() const;

  IntegerValue toConstInt() const;
  EntityValue toConstIdent() const;
  AttributedSynonym toAttrSyn() const;
};

typedef unique_ptr<WithArgument> WithArgumentPtr;

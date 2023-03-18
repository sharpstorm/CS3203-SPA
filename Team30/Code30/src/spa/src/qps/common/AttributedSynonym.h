#pragma once

#include <memory>
#include <vector>

#include "qps/common/PQLTypes.h"
#include "qps/common/PQLQuerySynonym.h"

using std::unique_ptr, std::vector, std::shared_ptr;

class AttributedSynonym {
  PQLQuerySynonym* syn;
  PQLSynonymAttribute attribute;
 public:
  AttributedSynonym();
  explicit AttributedSynonym(PQLQuerySynonym* syn);
  AttributedSynonym(PQLQuerySynonym* syn, PQLSynonymAttribute attr);

  PQLSynonymType getType();
  PQLSynonymName getName();
  PQLSynonymAttribute getAttribute();
  PQLQuerySynonym getSyn();
  bool validateAttribute();
  bool returnsInteger();
  bool isStatementType();

  struct hasher {
    std::size_t operator()(const AttributedSynonym &k) const {
      if (k.attribute != NO_ATTRIBUTE) {
        return std::hash<string>()(k.syn->getName()) ^
            std::hash<int>()(k.syn->getType());
      } else {
        return std::hash<string>()(k.syn->getName()) ^
            std::hash<int>()(k.syn->getType()) ^
            std::hash<int>()(k.attribute);
      }
    }
  };
};

typedef shared_ptr<AttributedSynonym> AttributedSynonymSPtr;
typedef unique_ptr<AttributedSynonym> AttributedSynonymPtr;
typedef vector<AttributedSynonym> AttributedSynonymList;

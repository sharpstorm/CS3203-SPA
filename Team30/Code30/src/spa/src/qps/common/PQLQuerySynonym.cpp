#include "PQLQuerySynonym.h"

PQLQuerySynonym::PQLQuerySynonym(const PQLSynonymType type,
                                 const PQLSynonymName &name) :
    type(type), name(name) {
}

PQLSynonymType PQLQuerySynonym::getType() const {
  return type;
}

PQLSynonymName PQLQuerySynonym::getName() const {
  return name;
}

bool PQLQuerySynonym::operator==(const PQLQuerySynonym &other) const {
  return this->name == other.name && this->type == other.type;
}

bool PQLQuerySynonym::isStatementType() const {
  return (type & PQL_SYN_STMT_MASK) > 0;
}

bool PQLQuerySynonym::isType(PQLSynonymType targetType) const {
  return this->type == targetType;
}

bool PQLQuerySynonym::isSubtypeOf(const PQLQuerySynonym *other) const {
  return other->isType(PQL_SYN_TYPE_STMT)
      && !isType(PQL_SYN_TYPE_STMT)
      && isStatementType();
}

bool PQLQuerySynonym::isDistinctSubtypeFrom(
    const PQLQuerySynonym *other) const {
  return isStatementType() && other->isStatementType()
      && !other->isType(type)
      && !isType(PQL_SYN_TYPE_STMT) && !other->isType(PQL_SYN_TYPE_STMT);
}

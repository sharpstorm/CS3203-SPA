#include "PQLQuerySynonym.h"

PQLQuerySynonym::PQLQuerySynonym(PQLSynonymType type, PQLSynonymName name):
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

bool PQLQuerySynonym::isEntityType() const {
  return (type & PQL_SYN_STMT_MASK) == 0;
}

bool PQLQuerySynonym::isType(PQLSynonymType targetType) const {
  return this->type == targetType;
}

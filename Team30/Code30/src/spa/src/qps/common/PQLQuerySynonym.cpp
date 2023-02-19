#include "PQLQuerySynonym.h"

PQLQuerySynonym::PQLQuerySynonym(PQLSynonymType type, PQLSynonymName name):
    type(type), name(name) {
}

PQLSynonymType PQLQuerySynonym::getType() {
  return type;
}

PQLSynonymName PQLQuerySynonym::getName() {
  return name;
}

bool PQLQuerySynonym::operator==(const PQLQuerySynonym &other) const {
  return this->name == other.name && this->type == other.type;
}

bool PQLQuerySynonym::isStatementType() {
  return (type & PQL_SYN_STMT_MASK) > 0;
}

bool PQLQuerySynonym::isEntityType() {
  return (type & PQL_SYN_STMT_MASK) == 0;
}

bool PQLQuerySynonym::isType(PQLSynonymType targetType) {
  return this->type == targetType;
}

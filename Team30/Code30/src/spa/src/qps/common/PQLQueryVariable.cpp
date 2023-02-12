#include "PQLQueryVariable.h"

bool PQLQueryVariable::operator==(const PQLQueryVariable &other) const {
  return this->name == other.name && this->type == other.type;
}

bool PQLQueryVariable::isStatementType() {
  return (type & PQL_SYN_STMT_MASK) > 0;
}

bool PQLQueryVariable::isEntityType() {
  return (type & PQL_SYN_STMT_MASK) == 0;
}

bool PQLQueryVariable::isType(PQLSynonymType type) {
  return this->type == type;
}

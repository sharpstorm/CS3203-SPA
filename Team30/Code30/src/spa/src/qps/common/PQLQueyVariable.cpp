#include "PQLQueryVariable.h"

bool PQLQueryVariable::operator==(const PQLQueryVariable &other) const {
  return this->name == other.name && this->type == other.type;
}

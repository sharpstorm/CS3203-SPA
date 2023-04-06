#include "QueryResultItem.h"

using std::to_string;

bool QueryResultItem::operator==(const QueryResultItem &other) const {
  return stmtRef == other.stmtRef && entRef == other.entRef;
}

bool QueryResultItem::operator!=(const QueryResultItem &other) const {
  return !(*this == other);
}

ProjectedValue QueryResultItem::project() const {
  if (stmtRef != NO_STMT) {
    return to_string(stmtRef);
  }

  if (entRef != NO_ENT) {
    return entRef;
  }

  return "";
}

StmtValue QueryResultItem::toStmtValue() const {
  return stmtRef;
}

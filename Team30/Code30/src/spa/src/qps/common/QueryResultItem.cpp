#include "QueryResultItem.h"

using std::to_string;

bool QueryResultItem::operator==(const QueryResultItem &other) const {
  return stmtRef == other.stmtRef && entRef == other.entRef;
}

bool QueryResultItem::operator!=(const QueryResultItem &other) const {
  return !(*this == other);
}

string QueryResultItem::project() {
  if (stmtRef != NO_STMT_REF) {
    return to_string(stmtRef);
  }

  if (entRef != NO_ENT_REF) {
    return entRef;
  }

  return "";
}

const char QueryResultItem::NO_ENT_REF[] = "";

int QueryResultItem::toStmtValue() {
  return stmtRef;
}

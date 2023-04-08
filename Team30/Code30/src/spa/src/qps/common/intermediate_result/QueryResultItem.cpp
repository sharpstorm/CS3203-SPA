#include "QueryResultItem.h"
#include "qps/common/QPSTypes.h"

using std::to_string;

QueryResultItem::QueryResultItem(const StmtValue &stmt) : stmtRef(stmt),
                                                          entRef(NO_ENT) {}

QueryResultItem::QueryResultItem(const EntityValue &ent) : stmtRef(NO_STMT),
                                                           entRef(ent) {}

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

#pragma once

#include <memory>
#include <unordered_set>
#include <unordered_map>
#include "common/Types.h"
#include "qps/common/PQLTypes.h"

using std::unique_ptr, std::unordered_set, std::unordered_map;

class QueryResultItem {
 private:
  StmtValue stmtRef;
  EntityValue entRef;

 public:
  explicit QueryResultItem(const StmtValue &stmt): stmtRef(stmt),
                                             entRef(NO_ENT) {}
  explicit QueryResultItem(const EntityValue &ent): stmtRef(NO_STMT),
                                               entRef(ent) {}
  StmtValue toStmtValue() const;
  ProjectedValue project() const;
  bool operator==(const QueryResultItem& other) const;
  bool operator!=(const QueryResultItem& other) const;

  struct hasher {
    std::size_t operator()(const QueryResultItem& k) const {
      if (k.stmtRef != NO_STMT) {
        return std::hash<StmtValue>()(k.stmtRef);
      } else {
        return std::hash<EntityValue>()(k.entRef);
      }
    }
  };
};

typedef unique_ptr<QueryResultItem> QueryResultItemPtr;

template<class T>
using QueryResultItemMap = unordered_map<QueryResultItem, T,
                                         QueryResultItem::hasher>;

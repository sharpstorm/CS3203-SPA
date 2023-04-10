#pragma once

#include <memory>
#include <unordered_map>

#include "common/Types.h"
#include "qps/common/synonym/PQLTypes.h"
#include "qps/common/QPSTypes.h"

/*
 * Because this is a templated class, templated methods must be
 * in the header file, or linker errors will occur
 */

using std::unique_ptr, std::unordered_map;

template<class ProjectionClosure>
using StmtAttributeProjector =
    ProjectedValue(*)(const StmtValue &value, const ProjectionClosure *closure);

class QueryResultItem {
 private:
  StmtValue stmtRef;
  EntityValue entRef;

 public:
  explicit QueryResultItem(const StmtValue &stmt);
  explicit QueryResultItem(const EntityValue &ent);

  ProjectedValue project() const;
  template<class ProjectionClosure>
  ProjectedValue projectAttribute(
      StmtAttributeProjector<ProjectionClosure> projector,
      const ProjectionClosure *closure) const;

  bool operator==(const QueryResultItem &other) const;
  bool operator!=(const QueryResultItem &other) const;

  struct hasher {
    std::size_t operator()(const QueryResultItem &k) const {
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

template<class ProjectionClosure>
ProjectedValue QueryResultItem::projectAttribute(
    StmtAttributeProjector<ProjectionClosure> projector,
    const ProjectionClosure *closure) const {
  return projector(stmtRef, closure);
}

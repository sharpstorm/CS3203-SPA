#pragma once

#include "qps/common/AttributedSynonym.h"
#include "pkb/queryHandlers/PkbQueryHandler.h"
#include "qps/common/intermediate_result/QueryResultItem.h"

class ResultItemProjector {
 private:
  const AttributedSynonym *syn;
  const PkbQueryHandler *pkbHandler;

  static ProjectedValue projectRead(const StmtValue &value,
                                    const PkbQueryHandler *handler);
  static ProjectedValue projectPrint(const StmtValue &value,
                                     const PkbQueryHandler *handler);
  static ProjectedValue projectCall(const StmtValue &value,
                                    const PkbQueryHandler *handler);
  ProjectedValue projectNonDefaultAttribute(const QueryResultItem *item) const;

 public:
  ResultItemProjector(const PkbQueryHandler *handler,
                      const AttributedSynonym *syn);
  ProjectedValue project(const QueryResultItem *item) const;
};

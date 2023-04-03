#include "ResultItemProjector.h"

ResultItemProjector::ResultItemProjector(const PkbQueryHandler *handler,
                                         const AttributedSynonym *syn) :
    pkbHandler(handler), syn(syn) {}

ProjectedValue ResultItemProjector::project(const QueryResultItem *item) const {
  if (!syn->isDefaultAttribute()) {
    return projectNonDefaultAttribute(item);
  }
  return item->project();
}

ProjectedValue ResultItemProjector::projectNonDefaultAttribute(
    const QueryResultItem *item) const {
  switch (syn->getType()) {
    case PQL_SYN_TYPE_READ:
      return item->projectAttribute(ResultItemProjector::projectRead,
                                    pkbHandler);
    case PQL_SYN_TYPE_PRINT:
      return item->projectAttribute(ResultItemProjector::projectPrint,
                                    pkbHandler);
    case PQL_SYN_TYPE_CALL:
      return item->projectAttribute(ResultItemProjector::projectCall,
                                    pkbHandler);
    default:return "";
  }
}

ProjectedValue ResultItemProjector::projectCall(
    const StmtValue &value, const PkbQueryHandler *handler) {
  return handler->getCalledDeclaration(value);
}

ProjectedValue ResultItemProjector::projectPrint(
    const StmtValue &value, const PkbQueryHandler *handler) {
  return handler->getPrintDeclarations(value);
}

ProjectedValue ResultItemProjector::projectRead(
    const StmtValue &value, const PkbQueryHandler *handler) {
  return handler->getReadDeclarations(value);
}

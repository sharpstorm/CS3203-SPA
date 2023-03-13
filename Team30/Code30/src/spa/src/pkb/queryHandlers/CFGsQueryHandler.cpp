#include "CFGsQueryHandler.h"

#include <string>

using std::string;

CFGsQueryHandler::CFGsQueryHandler(
    const CFGStorage *cfgsStorage,
    const IStructureMappingProvider *structureProvider)
    : cfgsStorage(cfgsStorage), structureProvider(structureProvider) {}

QueryResult<int, CFG*> CFGsQueryHandler::queryCFGs(StmtRef stmt) const {
  QueryResult<int, CFG*> result;
  if (!stmt.isKnown()) {
    return result;
  }
  string procedureName = structureProvider->getProcedureForLine(stmt.lineNum);
  result.add(stmt.lineNum, cfgsStorage->get(procedureName).get());
  return result;
}

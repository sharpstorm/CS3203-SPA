#include "CFGsWriter.h"

CFGsWriter::CFGsWriter(CFGStorage *cfgStorage) : cfgStorage(cfgStorage) {}

void CFGsWriter::addCFGs(EntityValue procedureName, CFGSPtr cfg) {
  cfgStorage->insert(procedureName, cfg);
}

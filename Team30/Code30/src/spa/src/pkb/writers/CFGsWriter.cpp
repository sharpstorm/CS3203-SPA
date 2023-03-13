#include "CFGsWriter.h"

CFGsWriter::CFGsWriter(CFGStorage *cfgStorage) : cfgStorage(cfgStorage) {}

void CFGsWriter::addCFGs(string procedureName, CFGSPtr cfg) {
  cfgStorage->set(procedureName, cfg);
}

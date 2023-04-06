#include "SourceLexerFactory.h"

SourceLexer SourceLexerFactory::makeLexer(const FileData *fileData) const {
  return SourceLexer(&tokenTable, fileData);
}

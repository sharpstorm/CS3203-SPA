#pragma once

#include "SourceLexerTokenTable.h"
#include "SourceLexer.h"

class SourceLexerFactory {
 private:
  SourceLexerTokenTable tokenTable;

 public:
  SourceLexer makeLexer(const FileData *fileData) const;
};

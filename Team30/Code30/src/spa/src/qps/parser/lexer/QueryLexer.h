#pragma once

#include <string>
#include <vector>
#include <memory>

#include "QueryLexerTokenTable.h"
#include "../../../common/UtilityTypes.h"

using std::string, std::vector, std::unique_ptr;

typedef vector<PQLToken> PQLTokenStream;
typedef unique_ptr<PQLTokenStream> PQLTokenStreamPtr;

class QueryLexer {
 public:
  explicit QueryLexer(string *query,
                      QueryLexerTokenTable* tokenTable);
  PQLTokenStreamPtr getTokenStream();

 private:
  string* query;
  PQLTokenStreamPtr result;

  string buffer;
  string literalBuffer;
  bool hasSeenChar;
  int literalSymbolCount;
  bool isProcessingLiteral;

  QueryLexerTokenTable* tokenTable;
  void processChar(const char &c);
  void flushBuffer();
  void processLiteral(const char &c, const PQLTokenType &type);
  void toggleLiteral();
  void startLiteral();
  void flushLiteral();
  void clearState();

  PQLToken resolveStringToken(string buffer, bool hasSeenChar);
  PQLToken validateIntegerToken(string* buffer);
  PQLToken validateIdentifier(string* buffer);

  void throwInvalidCharError();
};

typedef unique_ptr<QueryLexer> QueryLexerPtr;

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
  explicit QueryLexer(const string *query,
                      QueryLexerTokenTable* tokenTable);
  PQLTokenStreamPtr getTokenStream();

 private:
  const string *query;
  PQLTokenStreamPtr result;

  string buffer;
  string literalBuffer;
  bool hasSeenChar;
  int literalSymbolCount;
  bool isProcessingLiteral;

  QueryLexerTokenTable* tokenTable;
  void processChar(const char &c);
  void flushBuffer();

  void toggleLiteral();
  void startLiteral();
  void endLiteral();

  void appendStringToken(const PQLTokenType &tokenType,
                         const string &data);
  void appendSymbolToken(const PQLTokenType &tokenType,
                         const char &c);
  void clearState();

  PQLTokenType resolveStringToken(const string &buffer,
                                  const bool &hasSeenChar);
  void validateIntegerToken(const string &buffer);
  void validateIdentifier(const string &buffer);
};

typedef unique_ptr<QueryLexer> QueryLexerPtr;

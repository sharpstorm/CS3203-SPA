#pragma once

#include <string>
#include <vector>
#include <memory>

#include "QueryLexerTokenTable.h"
#include "../../../common/UtilityTypes.h"
#include "qps/common/QPSTypes.h"

using std::string, std::vector, std::unique_ptr;

typedef vector<PQLToken> PQLTokenStream;
typedef unique_ptr<PQLTokenStream> PQLTokenStreamPtr;

class QueryLexer {
 public:
  explicit QueryLexer(const PQLQueryString *query,
                      QueryLexerTokenTable* tokenTable);
  PQLTokenStreamPtr getTokenStream();

 private:
  const PQLQueryString *query;
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
  void validateIntegerToken(const string &buffer) const;
  void validateIdentifier(const string &buffer) const;
};

typedef unique_ptr<QueryLexer> QueryLexerPtr;

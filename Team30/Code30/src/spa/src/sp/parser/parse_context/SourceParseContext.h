#pragma once

#include "sp/common/SourceToken.h"
#include "sp/parser/SourceParseState.h"
#include "common/ast/ASTNode.h"
#include "sp/errors/SPError.h"

class SourceParseContext {
 public:
  virtual ~SourceParseContext() = default;
  virtual ASTNodePtr generateSubtree(SourceParseState* state) = 0;

 protected:
  template<typename... SourceTokenType>
  SourceToken *expect(SourceParseState* parserState,
                      SourceTokenType... tokenType);

  SourceToken* expectVarchar(SourceParseState* parserState);
};

template<typename... SourceTokenType>
SourceToken* SourceParseContext::expect(SourceParseState* parserState,
                                        SourceTokenType... tokenType) {
  SourceToken* currentToken = parserState->getCurrToken();

  if (currentToken == nullptr) {
    throw SPError(SPERR_END_OF_STREAM);
  }

  if ((currentToken->isType(tokenType)  || ... || false)) {
    parserState->advanceToken();
    return currentToken;
  }
  throw SPError(SPERR_UNEXPECTED_TOKEN);
}

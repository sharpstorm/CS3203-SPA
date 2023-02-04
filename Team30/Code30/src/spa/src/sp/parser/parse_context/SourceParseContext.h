#pragma once

#include <memory>
#include "../../common/SourceToken.h"
#include "sp/parser/SourceParseState.h"
#include "../../common/ASTNode/ASTNode.h"
#include "sp/errors/SPError.h"

using std::shared_ptr;

class SourceParseContext {
 public:
  virtual ~SourceParseContext() = default;
  virtual shared_ptr<ASTNode> generateSubtree(SourceParseState* state) = 0;

 private:
  virtual bool validate(SourceParseState* state) = 0;

 protected:
  template<typename... SourceTokenType>
  SourceToken *expect(SourceParseState* parserState,
                   SourceTokenType... tokenType);
};

template<typename... SourceTokenType>
SourceToken* SourceParseContext::expect(SourceParseState* parserState,
                                     SourceTokenType... tokenType) {
  SourceToken* currentToken = parserState->getCurrToken();

  if (currentToken == nullptr) {
    throw SPError("End of token stream");
  }

  if ((currentToken->isType(tokenType)  || ... || false)) {

    if (!parserState->isAtLast()) {
        parserState->advanceToken();
    }

    return currentToken;
  }

  throw SPError("Unexpected Token");
}

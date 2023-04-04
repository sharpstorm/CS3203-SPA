#pragma once

#include <string>
#include "qps/parser/PQLToken.h"
#include "qps/common/PQLTypes.h"
#include "qps/parser/token_parser/QueryTokenParseState.h"
#include "qps/parser/builder/QueryBuilder.h"
#include "IPQLDeclarationContext.h"

using std::string;

template <PQLSynonymType PQL_SYN_TYPE>
class PQLDeclarationContext: public IPQLDeclarationContext {
 public:
  void parse(QueryTokenParseState* parserState,
             QueryBuilder *builder) override;
};

template <PQLSynonymType PQL_SYN_TYPE>
void PQLDeclarationContext<PQL_SYN_TYPE>::parse(
    QueryTokenParseState *parserState,
    QueryBuilder* builder) {
  PQLSynonymName currentName = parserState->expectSynName();
  builder->addSynonym(currentName, PQL_SYN_TYPE);

  const PQLToken* currentToken = parserState->expect(PQL_TOKEN_COMMA,
                                               PQL_TOKEN_SEMICOLON);
  while (!currentToken->isType(PQL_TOKEN_SEMICOLON)) {
    currentName = parserState->expectSynName();
    builder->addSynonym(currentName, PQL_SYN_TYPE);
    currentToken = parserState->expect(PQL_TOKEN_COMMA, PQL_TOKEN_SEMICOLON);
  }
}

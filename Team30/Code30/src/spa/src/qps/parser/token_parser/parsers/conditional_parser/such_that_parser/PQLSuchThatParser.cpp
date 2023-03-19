#include "PQLSuchThatParser.h"

#include <utility>
#include <memory>

using std::move, std::unique_ptr;

void PQLSuchThatParser::parse(QueryTokenParseState *parserState,
                              QueryBuilder *queryBuilder) {
  parserState->expect(PQL_TOKEN_SUCH);
  parserState->expect(PQL_TOKEN_THAT);

  IPQLSuchThatClauseContext* context;
  unique_ptr<PQLToken> dummyAnd = make_unique<PQLToken>(PQL_TOKEN_AND);
  PQLToken* andToken = dummyAnd.get();
  while (andToken != nullptr) {
    context = getContext(parserState->getCurrentTokenType());
    if (context == nullptr) {
      throw QPSParserSyntaxError(QPS_PARSER_ERR_UNEXPECTED);
    }

    parserState->advanceToken();
    SuchThatClausePtr clause = context->parse(parserState, queryBuilder);
    if (clause != nullptr) {
      queryBuilder->addSuchThat(std::move(clause));
    }
    andToken = parserState->tryExpect(PQL_TOKEN_AND);
  }
}

IPQLSuchThatClauseContext *PQLSuchThatParser::getContext(PQLTokenType type) {
  switch (type) {
    case PQL_TOKEN_FOLLOWS:
      return &followsContext;
    case PQL_TOKEN_PARENT:
      return &parentContext;
    case PQL_TOKEN_USES:
      return &usesContext;
    case PQL_TOKEN_MODIFIES:
      return &modifiesContext;
    case PQL_TOKEN_CALLS:
      return &callsContext;
    case PQL_TOKEN_NEXT:
      return &nextContext;
    case PQL_TOKEN_AFFECTS:
      return &affectsContext;
    default:
      return nullptr;
  }
}

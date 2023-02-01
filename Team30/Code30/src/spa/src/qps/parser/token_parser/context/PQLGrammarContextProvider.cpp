#include "PQLGrammarContextProvider.h"
#include "declarations/PQLStmtContext.h"

unique_ptr<IPQLContext> PQLGrammarContextProvider::getContext(PQLToken *token) {
  switch (token->type) {
    case PQL_TOKEN_STMT:
      return std::make_unique<PQLStmtContext>();
    default:
      return nullptr;
  }
}

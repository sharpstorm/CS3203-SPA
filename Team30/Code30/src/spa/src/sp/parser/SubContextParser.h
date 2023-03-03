#pragma once

#include "common/ast/ASTNode.h"
#include "sp/parser/SourceParseState.h"
#include "sp/parser/expression_context/ExpressionContextProvider.h"

template <class ContextType, ContextType ENTRY_ENUM>
class SubContextParser {
 public:
  SubContextParser();
  ASTNodePtr parse(SourceParseState* state);

 private:
  IContextProviderPtr<ContextType> contextProvider;
};

template <class ContextType, ContextType ENTRY_ENUM>
ASTNodePtr SubContextParser<ContextType, ENTRY_ENUM>::parse(
    SourceParseState *state) {
  return contextProvider->getContext(ENTRY_ENUM)->generateSubtree(state);
}

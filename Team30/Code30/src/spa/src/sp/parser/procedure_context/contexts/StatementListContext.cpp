#include <memory>
#include <utility>

#include "sp/ast/StatementListNode.h"
#include "StatementListContext.h"

using std::make_unique;

ASTNodePtr StatementListContext::generateSubtree(
    SourceParseState *state) {
  ASTNodePtr node = make_unique<StatementListNode>();

  state->expect(SIMPLE_TOKEN_BRACKET_CURLY_LEFT);
  ASTNodePtr newNode = contextProvider
      ->generateSubtree(ProcedureContextType::STMT_CONTEXT, state);
  node->addChild(std::move(newNode));

  while (!state->currTokenIsOfType(SIMPLE_TOKEN_BRACKET_CURLY_RIGHT)) {
    newNode = contextProvider
        ->generateSubtree(ProcedureContextType::STMT_CONTEXT, state);
    node->addChild(std::move(newNode));
  }

  state->expect(SIMPLE_TOKEN_BRACKET_CURLY_RIGHT);
  return node;
}

#include <memory>

#include "sp/ast/StatementListNode.h"
#include "StatementListContext.h"

using std::make_shared;

ASTNodePtr StatementListContext::generateSubtree(
    SourceParseState *state) {
  ASTNodePtr node = make_shared<StatementListNode>();

  state->expect(SIMPLE_TOKEN_BRACKET_CURLY_LEFT);
  ASTNodePtr newNode = contextProvider
      ->getContext(ProcedureContextType::STMT_CONTEXT)
      ->generateSubtree(state);
  node->addChild(newNode);

  while (!state->currTokenIsOfType(SIMPLE_TOKEN_BRACKET_CURLY_RIGHT)) {
    newNode = contextProvider
        ->getContext(ProcedureContextType::STMT_CONTEXT)
        ->generateSubtree(state);
    node->addChild(newNode);
  }

  state->expect(SIMPLE_TOKEN_BRACKET_CURLY_RIGHT);
  state->setCached(node);

  return node;
}

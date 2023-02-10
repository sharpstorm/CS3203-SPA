#include <memory>
#include "RelationalFactorContext.h"

using std::shared_ptr;

shared_ptr<ASTNode> RelationalFactorContext::generateSubtree
    (SourceParseState *state) {
  return contextProvider->getContext(EXPR_CONTEXT)->generateSubtree(state);
}

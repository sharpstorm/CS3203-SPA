#include "FactorContext.h"

#include "../../common/ASTNode/math/PlusASTNode.h"

bool FactorContext::validate(SourceParseState *state) {
    return true;
}

shared_ptr<ASTNode> FactorContext::generateSubtree(SourceParseState *state) {
  switch (state->getCurrToken()->getType()) {
        case SIMPLE_TOKEN_INTEGER:
            return contextProvider->getContext(CONST_CONTEXT)->generateSubtree(state);
        case SIMPLE_TOKEN_VARIABLE:
            return contextProvider->getContext(VARIABLE_CONTEXT)->generateSubtree(state);
        default:
            throw SPError("Unknown token sequence");
    }
}
#pragma once

#include "RecursiveParseContext.h"
#include "../IGrammarContextProvider.h"
#include "sp/common/ASTNode/math/RelationalExpressionASTNode.h"

class RelationalExpressionContext: public RecursiveParseContext {
public:
    RelationalExpressionContext(IGrammarContextProvider* provider):
            RecursiveParseContext(provider) {}
    bool validate(SourceParseState* state);
    shared_ptr<ASTNode> generateSubtree(SourceParseState* state);

private:
    shared_ptr<RelationalExpressionASTNode> generateRelationalNode(string repr, SourceParseState* state,
                                                                   shared_ptr<ASTNode> leftNode);
};

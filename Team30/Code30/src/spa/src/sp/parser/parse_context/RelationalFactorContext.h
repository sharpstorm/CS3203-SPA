#pragma once

#include "RecursiveParseContext.h"
#include "../IGrammarContextProvider.h"

class RelationalFactorContext: public RecursiveParseContext {
public:
    RelationalFactorContext(IGrammarContextProvider* provider):
            RecursiveParseContext(provider) {}
    bool validate(SourceParseState* state);
    shared_ptr<ASTNode> generateSubtree(SourceParseState* state);

private:
};

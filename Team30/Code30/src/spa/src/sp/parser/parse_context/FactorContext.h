#pragma once

#include "RecursiveParseContext.h"
#include "../IGrammarContextProvider.h"

class FactorContext: public RecursiveParseContext {
public:
    FactorContext(IGrammarContextProvider* provider):
            RecursiveParseContext(provider) {}
    bool validate(SourceParseState* state);
    shared_ptr<ASTNode> generateSubtree(SourceParseState* state);

private:
};

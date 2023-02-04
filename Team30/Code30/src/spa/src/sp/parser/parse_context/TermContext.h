#pragma once

#include "RecursiveParseContext.h"
#include "../IGrammarContextProvider.h"

class TermContext: public RecursiveParseContext {
public:
    TermContext(IGrammarContextProvider* provider):
            RecursiveParseContext(provider) {}
    bool validate(SourceParseState* state);
    shared_ptr<ASTNode> generateSubtree(SourceParseState* state);

private:
};

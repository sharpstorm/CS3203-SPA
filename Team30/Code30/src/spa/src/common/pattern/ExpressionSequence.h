#pragma once

#include <vector>
#include <memory>
#include "PatternTrieNode.h"

using std::vector, std::unique_ptr;

typedef vector<SymbolIdent> ExpressionSequence;
typedef unique_ptr<ExpressionSequence> ExpressionSequencePtr;

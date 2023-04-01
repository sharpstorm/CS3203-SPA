#pragma once

#include <vector>
#include <string>
#include <memory>
#include "PatternTrieNode.h"

using std::string, std::vector, std::unique_ptr;

typedef vector<SymbolIdent> ExpressionSequence;
typedef unique_ptr<ExpressionSequence> ExpressionSequencePtr;

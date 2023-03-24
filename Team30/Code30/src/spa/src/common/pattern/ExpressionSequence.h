#pragma once

#include <vector>
#include <string>
#include <memory>

using std::string, std::vector, std::unique_ptr;

// TODO vector<uint16> ExpressionSequence variant
typedef vector<string> ExpressionSequence;
typedef unique_ptr<ExpressionSequence> ExpressionSequencePtr;

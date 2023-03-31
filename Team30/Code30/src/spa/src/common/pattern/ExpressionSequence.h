#pragma once

#include <vector>
#include <string>
#include <memory>

using std::string, std::vector, std::unique_ptr;

typedef vector<uint16_t> ExpressionSequence;
typedef unique_ptr<ExpressionSequence> ExpressionSequencePtr;

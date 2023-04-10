#pragma once

#include "ExpressionSequence.h"
#include "common/ast/IASTNode.h"

/*
 * Because this is a templated class, templated methods must be
 * in the header file, or linker errors will occur
 */

template<class T>
using PatternSymbolIdentGetter = SymbolIdent(*)(IASTNode *node, T *state);

template<class T>
class PatternNodeConverter {
 public:
  template<PatternSymbolIdentGetter<T> variableGetter,
      PatternSymbolIdentGetter<T> constantGetter>
  static SymbolIdent indexFromNode(IASTNode *node, T *closure);
};

template<class T>
template<PatternSymbolIdentGetter<T> variableGetter,
    PatternSymbolIdentGetter<T> constantGetter>
SymbolIdent PatternNodeConverter<T>::indexFromNode(IASTNode *node,
                                                   T *closure) {
  switch (node->getType()) {
    case ASTNODE_VARIABLE:
      return variableGetter(node, closure) & TRIE_SAFETY_MASK;
    case ASTNODE_CONSTANT:
      return (constantGetter(node, closure) & TRIE_SAFETY_MASK)
          | TRIE_CONST_MASK;
    case ASTNODE_PLUS:
      return TRIE_PLUS;
    case ASTNODE_MINUS:
      return TRIE_MINUS;
    case ASTNODE_TIMES:
      return TRIE_TIMES;
    case ASTNODE_DIV:
      return TRIE_DIV;
    case ASTNODE_MOD:
      return TRIE_MOD;
    default:
      return TRIE_INVALID_SYMBOL;
  }
}

#pragma once

#include <memory>
#include "SourceParseContext.h"

using std::unique_ptr;

template<class ContextType>
class IContextProvider {
 public:
  virtual ~IContextProvider() = default;
  virtual ASTNodePtr generateSubtree(ContextType type,
                                     SourceParseState *state) const = 0;
};

template<class ContextType>
using IContextProviderPtr = unique_ptr<IContextProvider<ContextType>>;

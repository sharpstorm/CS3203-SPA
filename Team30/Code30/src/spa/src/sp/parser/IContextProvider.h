#pragma once

#include <memory>
#include "SourceParseContext.h"

using std::unique_ptr;

template <class ContextType>
class IContextProvider {
 public:
  virtual ~IContextProvider() = default;
  virtual SourceParseContext* getContext(ContextType type) = 0;
};

template <class ContextType>
using IContextProviderPtr = unique_ptr<IContextProvider<ContextType>>;

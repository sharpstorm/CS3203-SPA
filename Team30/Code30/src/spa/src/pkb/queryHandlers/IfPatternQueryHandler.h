#pragma once

#include <string>

#include "common/Types.h"
#include "pkb/queryHandlers/interfaces/IIfPatternQueryHandler.h"
#include "pkb/storage/StorageTypes.h"

using std::string;

class IfPatternQueryHandler : public IIfPatternQueryHandler {
 public:
  explicit IfPatternQueryHandler(const IfPatternStorage *store);

  QueryResult<int, string> queryIfPattern(EntityRef) const override;

 private:
  const IfPatternStorage *store;

  bool validateArg(EntityRef) const;
};

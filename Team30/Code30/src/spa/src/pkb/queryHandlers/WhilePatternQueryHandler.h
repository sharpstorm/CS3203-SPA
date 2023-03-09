#pragma once

#include <string>

#include "common/Types.h"
#include "pkb/queryHandlers/interfaces/IWhilePatternQueryHandler.h"
#include "pkb/storage/StorageTypes.h"

using std::string;

class WhilePatternQueryHandler : public IWhilePatternQueryHandler {
 public:
  explicit WhilePatternQueryHandler(const WhilePatternStorage *store);

  QueryResult<int, string> queryWhilePattern(EntityRef) const override;

 private:
  const WhilePatternStorage *store;

  bool validateArg(EntityRef) const;
};

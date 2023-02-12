#pragma once

#include <string>

#include "EntityTableManager.h"
#include "TransitiveRelationTableManager.h"

using FollowsStorage = TransitiveRelationTableManager<int>;
using ParentStorage = TransitiveRelationTableManager<int>;
using SymbolStorage = EntityTableManager<std::string, EntityType>;
using StatementStorage = EntityTableManager<int, StmtType>;

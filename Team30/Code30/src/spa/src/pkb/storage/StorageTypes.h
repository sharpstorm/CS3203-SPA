#pragma once

#include <memory>
#include <string>

#include "EntityTableManager.h"
#include "TransitiveRelationTableManager.h"
#include "common/ASTNode/IASTNode.h"
#include "tables/HashKeyTable.h"

using FollowsStorage = TransitiveRelationTableManager<int>;
using ParentStorage = TransitiveRelationTableManager<int>;
using SymbolStorage = EntityTableManager<std::string, EntityType>;
using StatementStorage = EntityTableManager<int, StmtType>;
using ProcedureStorage = EntityTableManager<int, std::string>;
using AssignStorage = HashKeyTable<int, shared_ptr<IASTNode>>;

#pragma once

#include <memory>
#include <string>

#include "EntityTableManager.h"
#include "TransitiveRelationTableManager.h"
#include "common/ast/IASTNode.h"
#include "tables/HashKeyTable.h"
#include "tables/HashKeySetTable.h"

using std::string;

using FollowsStorage = TransitiveRelationTableManager<int>;
using ParentStorage = TransitiveRelationTableManager<int>;
using SymbolStorage = HashKeySetTable<EntityType, string>;
using StatementStorage = EntityTableManager<int, StmtType>;
using ProcedureStorage = EntityTableManager<int, string>;
using UsesStorage = RelationTableManager<int, string>;
using UsesPStorage = RelationTableManager<string, string>;
using ModifiesStorage = RelationTableManager<int, string>;
using ModifiesPStorage = RelationTableManager<string, string>;
using AssignStorage = HashKeyTable<int, shared_ptr<IASTNode>>;
using CallsStorage = TransitiveRelationTableManager<string>;
using CallStmtStorage = EntityTableManager<int, string>;

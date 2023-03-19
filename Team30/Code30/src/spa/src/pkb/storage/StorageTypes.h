#pragma once

#include <memory>

#include "EntityTableManager.h"
#include "RelationTableManager.h"
#include "TransitiveRelationTableManager.h"
#include "tables/HashKeyTable.h"
#include "tables/HashKeySetTable.h"
#include "common/Types.h"
#include "common/pattern/PatternTrie.h"
#include "common/cfg/CFG.h"
#include "pkb/storage/tables/ContiguousTable.h"
#include "pkb/storage/tables/ContiguousSetTable.h"

using VariableStorage = EntityTableManager<StmtValue, EntityValue>;
using EntityTable = ContiguousTable<EntityValue>;
using EntityRevTable = HashKeySetTable<EntityValue, StmtValue>;
using ConstantStorage = EntityTableManager<StmtValue, EntityValue>;
using ProcedureStorage = EntityTableManager<StmtValue, EntityValue>;

using StatementStorage = EntityTableManager<StmtValue, StmtType>;
using StmtTable = ContiguousTable<StmtType>;
using StmtRevTable = HashKeySetTable<StmtType, StmtValue>;

using CallStmtStorage = EntityTableManager<StmtValue, EntityValue>;
using CallStmtTable = HashKeyTable<StmtValue, EntityValue>;
using CallStmtRevTable = HashKeySetTable<EntityValue, StmtValue>;

using FollowsStorage = RelationTableManager<StmtValue, StmtValue>;
using FollowsTStorage = TransitiveRelationTableManager<StmtValue>;
using FollowsTable = ContiguousSetTable<StmtValue>;
using FollowsRevTable = ContiguousSetTable<StmtValue>;

using ParentStorage = RelationTableManager<StmtValue, StmtValue>;
using ParentTStorage = TransitiveRelationTableManager<StmtValue>;
using ParentTable = ContiguousSetTable<StmtValue>;
using ParentRevTable = ContiguousSetTable<StmtValue>;

using ModifiesStorage = RelationTableManager<StmtValue, EntityValue>;
using ModifiesTable = HashKeySetTable<StmtValue, EntityValue>;
using ModifiesRevTable = HashKeySetTable<EntityValue, StmtValue>;

using ModifiesPStorage = RelationTableManager<EntityValue, EntityValue>;
using ModifiesPTable = HashKeySetTable<EntityValue, EntityValue>;
using ModifiesPRevTable = HashKeySetTable<EntityValue, EntityValue>;

using UsesStorage = RelationTableManager<StmtValue, EntityValue>;
using UsesTable = HashKeySetTable<StmtValue, EntityValue>;
using UsesRevTable = HashKeySetTable<EntityValue, StmtValue>;

using UsesPStorage = RelationTableManager<EntityValue, EntityValue>;
using UsesPTable = HashKeySetTable<EntityValue, EntityValue>;
using UsesPRevTable = HashKeySetTable<EntityValue, EntityValue>;

using CallsStorage = RelationTableManager<EntityValue, EntityValue>;
using CallsTStorage = TransitiveRelationTableManager<EntityValue>;
using CallsTable = HashKeySetTable<EntityValue, EntityValue>;
using CallsRevTable = HashKeySetTable<EntityValue, EntityValue>;

using IfPatternStorage = RelationTableManager<StmtValue, EntityValue>;
using IfPatternTable = HashKeySetTable<StmtValue, EntityValue>;
using IfPatternRevTable = HashKeySetTable<EntityValue, StmtValue>;

using WhilePatternStorage = RelationTableManager<StmtValue, EntityValue>;
using WhilePatternTable = HashKeySetTable<StmtValue, EntityValue>;
using WhilePatternRevTable = HashKeySetTable<EntityValue, StmtValue>;

using AssignStorage = HashKeyTable<StmtValue, PatternTrieSPtr>;
using CFGStorage = HashKeyTable<EntityValue, CFGSPtr>;


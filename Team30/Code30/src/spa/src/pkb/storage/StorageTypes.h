#pragma once

#include <memory>

#include "IndexTableManager.h"
#include "RelationTableManager.h"
#include "StatementStorage.h"
#include "TransitiveRelationTableManager.h"
#include "common/Types.h"
#include "common/cfg/CFG.h"
#include "common/pattern/PatternTrie.h"
#include "pkb/storage/tables/ContiguousSetTable.h"
#include "pkb/storage/tables/ContiguousTable.h"
#include "tables/HashKeySetTable.h"
#include "tables/HashKeyTable.h"

using VariableStorage = IndexTableManager<EntityValue>;
using ConstantStorage = IndexTableManager<EntityValue>;
using VarTable = ContiguousTable<EntityValue>;
using VarRevTable = HashKeyTable<EntityValue, EntityIdx>;
using ConstTable = ContiguousTable<EntityValue>;
using ConstRevTable = HashKeyTable<EntityValue, EntityIdx>;

// stmtNum -> procedure
using ProcedureStmtTable = ContiguousTable<EntityValue>;
using ProcedureValues = EntityValueSet;
using StmtTable = ContiguousTable<StmtType>;
using StmtRevTable = HashKeySetTable<StmtType, StmtValue>;

using CallDeclarationTable = HashKeyTable<StmtValue, EntityValue>;

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

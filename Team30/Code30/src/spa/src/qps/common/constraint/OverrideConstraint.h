#pragma once

#include <string>
#include "Constraint.h"
#include "qps/common/PQLQuerySynonym.h"
#include "OverrideTransformer.h"
#include "qps/errors/QPSParserSemanticError.h"
#include "qps/common/AttributedSynonym.h"
#include "pkb/queryHandlers/PkbQueryHandler.h"

using std::string, std::to_string;

class OverrideConstraint : virtual public Constraint {
 private:
  AttributedSynonym syn;
  OverrideTransformer overrideTransformer;
  static StmtType getStmtTypeFromSynType(PQLSynonymType synType);
  static EntityType getEntTypeFromSynType(PQLSynonymType synType);
  bool existsInPKB(PkbQueryHandler* handler, PQLSynonymType synType);

 public:
  OverrideConstraint(AttributedSynonym syn, string identVal) :
      syn(syn), overrideTransformer(OverrideTransformer(identVal)) { }
  OverrideConstraint(AttributedSynonym syn, int intVal) :
      syn(syn), overrideTransformer(OverrideTransformer(intVal)) { }

  bool applyConstraint(VariableTable* varTable,
                       OverrideTable* overrideTable,
                       PkbQueryHandler* handler) {
    PQLSynonymName synName = syn.getName();
    PQLSynonymType synType = syn.getType();

    if (overrideTable->find(synName) != overrideTable->end()) {
      return false;
    }

    if (!existsInPKB(handler, synType)) {
      return false;
    }

    overrideTable->insert({synName, overrideTransformer});
    return true;
  }

  bool validateConstraint() override {
    if (!syn.validateAttribute()) {
      return false;
    }

    switch (syn.getAttribute()) {
      case STMT_NUM:
      case CONST_VALUE:
        return overrideTransformer.returnsInteger();
      case PROC_NAME:
      case VAR_NAME:
        return !overrideTransformer.returnsInteger();
      default:
        throw QPSParserSemanticError(QPS_PARSER_ERR_UNKNOWN_SYNONYM);
    }
  }
};

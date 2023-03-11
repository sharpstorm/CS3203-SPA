#include "AttributedSynonym.h"

AttributedSynonym::AttributedSynonym() : attribute(NO_ATTRIBUTE) { }

AttributedSynonym::AttributedSynonym(const PQLQuerySynonym &syn)
    : syn(syn), attribute(NO_ATTRIBUTE) { }

AttributedSynonym::AttributedSynonym(const PQLQuerySynonym& syn,
                                     PQLSynonymAttribute attr) :
                                     syn(syn), attribute(attr) { }

PQLSynonymAttribute AttributedSynonym::getAttribute() {
  return attribute;
}

bool AttributedSynonym::validateAttribute() {
  switch (syn.getType()) {
    case PQL_SYN_TYPE_STMT:
    case PQL_SYN_TYPE_ASSIGN:
    case PQL_SYN_TYPE_IF:
    case PQL_SYN_TYPE_WHILE:
      return attribute == STMT_NUM;
    case PQL_SYN_TYPE_READ:
    case PQL_SYN_TYPE_PRINT:
      return attribute == STMT_NUM || attribute == VAR_NAME;
    case PQL_SYN_TYPE_CALL:
      return attribute == STMT_NUM || attribute == PROC_NAME;
    case PQL_SYN_TYPE_CONSTANT:
      return attribute == CONST_VALUE;
    case PQL_SYN_TYPE_VARIABLE:
      return attribute == VAR_NAME;
    case PQL_SYN_TYPE_PROCEDURE:
      return attribute == PROC_NAME;
  }
}

PQLSynonymType AttributedSynonym::getType() {
  return syn.getType();
}

PQLSynonymName AttributedSynonym::getName() {
  return syn.getName();
}


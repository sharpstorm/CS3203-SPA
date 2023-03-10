#include "AttributedSynonym.h"

AttributedSynonym::AttributedSynonym() : attribute(NO_ATTRIBUTE) { }

AttributedSynonym::AttributedSynonym(PQLSynonymName name,
                                     PQLSynonymType type,
                                     PQLSynonymAttribute attribute) :
                                     PQLQuerySynonym(type, name),
                                     attribute(attribute) { }

AttributedSynonym::AttributedSynonym(const PQLQuerySynonym& syn,
                                     PQLSynonymAttribute attr) :
                                     PQLQuerySynonym(syn), attribute(attr) { }

PQLSynonymAttribute AttributedSynonym::getAttribute() {
  return attribute;
}

bool AttributedSynonym::validateAttribute() {
  switch (getType()) {
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
      return attribute == VALUE;
    case PQL_SYN_TYPE_VARIABLE:
      return attribute == VAR_NAME;
    case PQL_SYN_TYPE_PROCEDURE:
      return attribute == PROC_NAME;
  }
}


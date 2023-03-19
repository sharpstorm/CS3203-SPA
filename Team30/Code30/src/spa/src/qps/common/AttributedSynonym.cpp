#include "AttributedSynonym.h"
#include "qps/errors/QPSParserSemanticError.h"

AttributedSynonym::AttributedSynonym() : attribute(NO_ATTRIBUTE) { }

AttributedSynonym::AttributedSynonym(PQLQuerySynonym* syn)
    : syn(syn), attribute(NO_ATTRIBUTE) { }

AttributedSynonym::AttributedSynonym(PQLQuerySynonym* syn,
                                     PQLSynonymAttribute attr) :
                                     syn(syn), attribute(attr) { }

PQLSynonymAttribute AttributedSynonym::getAttribute() {
  return attribute;
}

PQLQuerySynonym* AttributedSynonym::getSyn() {
  return syn;
}

bool AttributedSynonym::validateAttribute() {
  switch (syn->getType()) {
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
    default:
      throw QPSParserSemanticError(QPS_PARSER_ERR_UNKNOWN_SYNONYM);
  }
}

PQLSynonymType AttributedSynonym::getType() {
  return syn->getType();
}

PQLSynonymName AttributedSynonym::getName() {
  return syn->getName();
}
bool AttributedSynonym::returnsInteger() {
  return (attribute & INT_RETURN_MASK) > 0;
}

bool AttributedSynonym::isStatementType() {
  return syn->isStatementType();
}

PQLQuerySynonym* AttributedSynonym::getSyn() {
  return syn;
}


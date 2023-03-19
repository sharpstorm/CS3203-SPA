#include "AttributedSynonym.h"
#include "qps/errors/QPSParserSemanticError.h"

AttributedSynonym::AttributedSynonym() : attribute(NO_ATTRIBUTE) { }

AttributedSynonym::AttributedSynonym(PQLQuerySynonymProxy synProxy) :
    synProxy(synProxy), attribute(NO_ATTRIBUTE) { }

AttributedSynonym::AttributedSynonym(PQLQuerySynonymProxy synProxy,
                                     PQLSynonymAttribute attr) :
    synProxy(synProxy), attribute(attr) { }

PQLQuerySynonym* AttributedSynonym::resolveProxy() {
  return *synProxy;
}

PQLSynonymAttribute AttributedSynonym::getAttribute() {
  return attribute;
}

bool AttributedSynonym::validateAttribute() {
  switch (resolveProxy()->getType()) {
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
  return resolveProxy()->getType();
}

PQLSynonymName AttributedSynonym::getName() {
  return resolveProxy()->getName();
}
bool AttributedSynonym::returnsInteger() {
  return (attribute & INT_RETURN_MASK) > 0;
}

bool AttributedSynonym::isStatementType() {
  return resolveProxy()->isStatementType();
}

PQLQuerySynonym* AttributedSynonym::getSyn() {
  return resolveProxy();
}

PQLQuerySynonymProxy AttributedSynonym::getSynProxy() {
  return synProxy;
}

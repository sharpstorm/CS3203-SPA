#include "AttributedSynonym.h"
#include "qps/errors/QPSParserSemanticError.h"

AttributedSynonym::AttributedSynonym(const PQLQuerySynonymProxy &synProxy) :
    synProxy(synProxy), attribute(NO_ATTRIBUTE) {}

AttributedSynonym::AttributedSynonym(const PQLQuerySynonymProxy &synProxy,
                                     const PQLSynonymAttribute &attr) :
    synProxy(synProxy), attribute(attr) {}

PQLSynonymAttribute AttributedSynonym::getAttribute() const {
  return attribute;
}

bool AttributedSynonym::validateAttribute() const {
  if (attribute == NO_ATTRIBUTE) {
    return true;
  }

  switch (synProxy->getType()) {
    case PQL_SYN_TYPE_STMT:
    case PQL_SYN_TYPE_ASSIGN:
    case PQL_SYN_TYPE_IF:
    case PQL_SYN_TYPE_WHILE:return attribute == STMT_NUM;
    case PQL_SYN_TYPE_READ:
    case PQL_SYN_TYPE_PRINT:
      return attribute == STMT_NUM || attribute == VAR_NAME;
    case PQL_SYN_TYPE_CALL:
      return attribute == STMT_NUM || attribute == PROC_NAME;
    case PQL_SYN_TYPE_CONSTANT:return attribute == CONST_VALUE;
    case PQL_SYN_TYPE_VARIABLE:return attribute == VAR_NAME;
    case PQL_SYN_TYPE_PROCEDURE:return attribute == PROC_NAME;
    default:return false;
  }
}

PQLSynonymType AttributedSynonym::getType() const {
  return synProxy->getType();
}

PQLSynonymName AttributedSynonym::getName() const {
  return synProxy->getName();
}
bool AttributedSynonym::returnsInteger() const {
  return (attribute & INT_RETURN_MASK) > 0;
}

bool AttributedSynonym::isStatementType() const {
  return synProxy->isStatementType();
}

bool AttributedSynonym::hasAttribute() const {
  return attribute != NO_ATTRIBUTE;
}

PQLQuerySynonymProxy AttributedSynonym::getSynProxy() const {
  return synProxy;
}

bool AttributedSynonym::isDefaultAttribute() const {
  PQLSynonymType synType = synProxy->getType();

  if (attribute == VAR_NAME) {
    return synType == PQL_SYN_TYPE_VARIABLE;
  } else if (attribute == PROC_NAME) {
    return synType == PQL_SYN_TYPE_PROCEDURE;
  }

  return true;
}

bool AttributedSynonym::isType(const PQLSynonymType &type) const {
  return synProxy->isType(type);
}

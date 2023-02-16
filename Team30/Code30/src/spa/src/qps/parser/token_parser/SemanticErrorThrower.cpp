#include "SemanticErrorThrower.h"
#include "qps/errors/QPSParserSemanticError.h"

SemanticErrorThrower::SemanticErrorThrower(): semanticErrorMsg("") {
}

void SemanticErrorThrower::setSemanticError(string error) {
  if (semanticErrorMsg != "") {
    return;
  }

  semanticErrorMsg = error;
}

bool SemanticErrorThrower::hasSemanticError() {
  return semanticErrorMsg != "";
}

void SemanticErrorThrower::throwIfSemanticError() {
  if (hasSemanticError()) {
    throw QPSParserSemanticError(semanticErrorMsg.c_str());
  }
}

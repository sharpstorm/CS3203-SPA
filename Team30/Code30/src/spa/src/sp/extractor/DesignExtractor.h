#pragma once

#include "sp/common/AST.h"
class DesignExtractor {
 public:
  DesignExtractor();
  void extractRelations(AST ast);
  void extractEntities(AST ast);
};
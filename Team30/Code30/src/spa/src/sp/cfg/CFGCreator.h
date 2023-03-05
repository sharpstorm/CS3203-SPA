#include "../ast/AST.h"
// Take in an AST and return the CFG

class CFGCreator {
 public:
  CFG createCFG(AST ast);
  CFG DFS(AST ast);
};
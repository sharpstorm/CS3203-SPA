#include <memory>
#include <string>
#include <vector>

#include "../ast/AST.h"
#include "../ast/statement/AssignNode.h"
#include "../ast/statement/CallNode.h"
#include "../ast/statement/IfNode.h"
#include "../ast/statement/PrintNode.h"
#include "../ast/statement/ReadNode.h"
#include "../ast/statement/StatementASTNode.h"
#include "../ast/statement/WhileNode.h"
// Take in an AST and return the CFG

using std::vector, std::string, std::shared_ptr;

class CFGCreator {
 public:
  CFG createCFG(AST ast);
  CFG DFS(ASTNodePtr node);
};
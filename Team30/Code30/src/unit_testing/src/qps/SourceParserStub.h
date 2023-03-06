#include <memory>

#include "common/ast/IAST.h"
#include "sp/ast/entity/VariableASTNode.h"
#include "sp/ast/AST.h"
#include "common/parser/ISourceExpressionParser.h"

using std::make_shared, std::make_unique;

class SourceParserStub: public ISourceExpressionParser {
 public:
  IASTPtr parseExpression(string expression) override {
    auto root = make_shared<VariableASTNode>("a");
    auto tree = make_unique<AST>(root);
    return tree;
  }
};

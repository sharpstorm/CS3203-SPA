#include "SourceTokenParser.h"
#include <iostream>

SourceTokenParser::SourceTokenParser() {
};

shared_ptr<ASTNode> SourceTokenParser::parse(vector<SourceToken>* tokens) {
  SourceParseState state(tokens);
  GrammarContextProvider gcp;
  gcp.getContext(EXPR_CONTEXT)->generateSubtree(&state);
//  gcp.getContext(EXPR_CONTEXT)->generateSubtree(&state);
   /*while (!state.isEnd()) {
    SourceToken* token = state.getCurrToken();
    std::cout << token->getType() << "\n";
    gcp.getContext(token->getType())->generateSubtree(&state);
  }*/

  return state.getCached();
    /* SourceParseState state(tokens);
    GrammarContextProvider gcp;
    vector<Token> arr;
    arr.push_back(Token("("));
    arr.push_back(Token("1"));
    // arr.push_back(Token("+"));
    // arr.push_back(Token("2"));
    // arr.push_back(Token(")"));
    // arr.push_back(Token("*"));
    // arr.push_back(Token("3"));
    // arr.push_back(Token("*"));
    // arr.push_back(Token("4"));
    SourceParseState s(&arr);
    while(!s.hasEnd()) {
        AbstractASTNode* node = gcp.getContext(&s)->generateSubtree(&s);
        // cout << s.prev->getType();
        s.updatePrev(node);
        s.prevTokenType = s.getCurrTokenType();
        s.forward();

        if(s.root == nullptr) {
            s.root = node;
        }
    }
    // an->print("==");
        // cout << "HERE" << endl;
    // }
    s.root->print("==");*/
}

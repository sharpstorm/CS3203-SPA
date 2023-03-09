#pragma once

#include <vector>

#include "qps/parser/PQLToken.h"

using std::vector, std::to_string;

class PQLTestTokenSequenceBuilder {
 private:
  vector<PQLToken> tokenSequence;

 public:
  PQLTestTokenSequenceBuilder() {}

  PQLTestTokenSequenceBuilder* addToken(PQLToken token) {
    tokenSequence.push_back(token);
    return this;
  }

  PQLTestTokenSequenceBuilder* addToken(PQLTokenType type) {
    tokenSequence.push_back(PQLToken(type));
    return this;
  }

  PQLTestTokenSequenceBuilder* addToken(PQLTokenType type, string value){
    tokenSequence.push_back(PQLToken(type, value));
    return this;
  }

  PQLTestTokenSequenceBuilder* synonym(string synonym){
    tokenSequence.push_back(PQLToken(PQL_TOKEN_STRING, synonym));
    return this;
  }

  PQLTestTokenSequenceBuilder* integer(int value){
    tokenSequence.push_back(PQLToken(PQL_TOKEN_INTEGER, to_string(value)));
    return this;
  }

  PQLTestTokenSequenceBuilder* ident(string value) {
    tokenSequence.push_back(PQLToken(PQL_TOKEN_STRING_LITERAL, value));
    return this;
  }

  PQLTestTokenSequenceBuilder* literal(string value){
    tokenSequence.push_back(PQLToken(PQL_TOKEN_LITERAL, value));
    return this;
  }

  PQLTestTokenSequenceBuilder* wildcard() {
    tokenSequence.push_back(PQLToken(PQL_TOKEN_UNDERSCORE));
    return this;
  }

  PQLTestTokenSequenceBuilder* comma() {
    tokenSequence.push_back(PQLToken(PQL_TOKEN_COMMA));
    return this;
  }

  PQLTestTokenSequenceBuilder* openBracket() {
    tokenSequence.push_back(PQLToken(PQL_TOKEN_BRACKET_OPEN));
    return this;
  }

  PQLTestTokenSequenceBuilder* closeBracket() {
    tokenSequence.push_back(PQLToken(PQL_TOKEN_BRACKET_CLOSE));
    return this;
  }

  PQLTestTokenSequenceBuilder* semicolon() {
    tokenSequence.push_back(PQLToken(PQL_TOKEN_SEMICOLON));
    return this;
  }

  vector<PQLToken> build() {
    return tokenSequence;
  }
};

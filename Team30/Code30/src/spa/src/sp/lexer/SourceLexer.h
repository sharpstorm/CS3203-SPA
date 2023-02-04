#pragma once
#include <iostream>
#include <vector>
#include "../common/SourceToken.h"
#include "SourceLexerTokenTable.h"

using namespace std;

class SourceLexer {
 public:
  SourceLexer();
  vector<SourceToken> tokenize(string* programLines);
 private:
  SourceLexerTokenTable tokenTable;
  bool isBreakChar(char c);
  bool isBreakString(char c, char cNext);
  bool isNextOfType(string* buffer, int curPos, SourceTokenType expectedType);
  int peekNextChar(string* buffer, int curPos);
  SourceToken resolveStringToken(string buffer, bool hasSeenChar);
  SourceToken validateIdentifier(string *buffer);
  SourceToken validateIntegerToken(string* buffer);
};

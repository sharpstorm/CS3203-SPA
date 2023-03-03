#pragma once

#include <string>

using std::string;

class IModifiesWriter {
 public:
  virtual ~IModifiesWriter() {}
  /**
   * Adds relation where statement directly modifies variable.
   */
  virtual void addModifies(int stmtNum, string variable, string procedure) = 0;
};

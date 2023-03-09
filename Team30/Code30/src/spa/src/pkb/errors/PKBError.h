#pragma once

#include <string>

const char PKBERR_DUPLICATE_PROCEDURE[] = "Procedure already defined";
const char PKBERR_CYCLIC_PROCEDURE[] = "Cyclic calls detected";
const char PKBERR_NONEXISTENT_PROCEDURE[] = "Non-existent procedure called";

using std::string;

class PKBError : public std::exception {
 private:
  string errMsg;

 public:
  explicit PKBError(const string message) : errMsg(message) {}
  const char *what() const noexcept override {
    return errMsg.c_str();
  }
};

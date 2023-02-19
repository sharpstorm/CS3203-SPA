#pragma once

#include <string>

using std::string;

class QPSError : public std::exception {
 private:
  const string errMsg;

 protected:
  explicit QPSError(string message): errMsg(message) {}

 public:
  const char* what() const noexcept override {
    return errMsg.c_str();
  }
};

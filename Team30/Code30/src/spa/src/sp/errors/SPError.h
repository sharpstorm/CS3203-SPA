#pragma once

const char SPERR_TOKEN_STARTS_WITH_DIGIT[] = "String token starts with digit";
const char SPERR_INTEGER_STARTS_WITH_ZERO[] = "Integer token starts with zero";
const char SPERR_UNKNOWN_TOKEN[] = "Unknown token";
const char SPERR_INVALID_SOURCE_CHAR[] = "Invalid Source Character Detected";

class SPError : public std::exception {
 private:
  const char* errMsg;

 public:
  explicit SPError(const char* message): errMsg(message) {}
  const char* what() const noexcept override {
    return errMsg;
  }
};

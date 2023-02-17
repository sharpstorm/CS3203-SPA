#pragma once

class QPSError : public std::exception {
 private:
  const char* errMsg;

 protected:
  explicit QPSError(const char* message): errMsg(message) {}

 public:
  const char* what() const noexcept override {
    return errMsg;
  }
};

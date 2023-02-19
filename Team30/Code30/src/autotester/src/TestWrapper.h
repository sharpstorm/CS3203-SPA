#ifndef TESTWRAPPER_H
#define TESTWRAPPER_H

#include <string>
#include <iostream>
#include <list>

#include "qps/IQPS.h"
#include "pkb/storage/PKB.h"
#include "sp/ISp.h"

// include your other headers here
#include "AbstractWrapper.h"
#include <memory>

using std::unique_ptr;

class TestWrapper : public AbstractWrapper {
 private:
  unique_ptr<IQPS> qps;
  unique_ptr<PKB> pkb;
  unique_ptr<ISp> sp;
  unique_ptr<PkbWriter> pkbWriter;

 public:
  // default constructor
  TestWrapper();
  
  // destructor
  ~TestWrapper();
  
  // method for parsing the SIMPLE source
  virtual void parse(std::string filename);
  
  // method for evaluating a query
  virtual void evaluate(std::string query, std::list<std::string>& results);
};

#endif

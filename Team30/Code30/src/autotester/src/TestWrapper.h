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

class TestWrapper : public AbstractWrapper {
 private:
  IQPS* qps;
  PKB* pkb;
  ISp* sp;
  PkbWriter* pkbWriter;

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

#include <memory>
#include <vector>

#include "TestWrapper.h"
#include "qps/QPSInitialiser.h"

using std::shared_ptr, std::vector;

// implementation code of WrapperFactory - do NOT modify the next 5 lines
AbstractWrapper* WrapperFactory::wrapper = 0;
AbstractWrapper* WrapperFactory::createWrapper() {
  if (wrapper == 0) wrapper = new TestWrapper;
  return wrapper;
}
// Do not modify the following line
volatile bool AbstractWrapper::GlobalStop = false;

// a default constructor
TestWrapper::TestWrapper() {
  // create any objects here as instance variables of this class
  // as well as any initialization required for your spa program
  PKB* pkb = new PKB();
  PkbQueryHandler* pkbQH = new PkbQueryHandler(pkb);
  shared_ptr<PkbQueryHandler> pkbQH_ptr = shared_ptr<PkbQueryHandler>(pkbQH);

  QPSInitialiser qpsInitialiser;
  qps = qpsInitialiser.initQPS(pkbQH_ptr);
}

// method for parsing the SIMPLE source
void TestWrapper::parse(std::string filename) {
	// call your parser to do the parsing
  // ...rest of your code...
}

// method to evaluating a query
void TestWrapper::evaluate(std::string query, std::list<std::string>& results){
// call your evaluator to evaluate the query here
  // ...code to evaluate query...
  vector<string>* queryResult = qps->evaluate(query);
  // store the answers to the query in the results list (it is initially empty)
  // each result must be a string.
  for (auto qr : *queryResult) {
    results.push_back(qr);
  }
}

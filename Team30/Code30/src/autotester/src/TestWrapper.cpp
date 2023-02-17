#include <memory>
#include <vector>

#include "TestWrapper.h"
#include "qps/QPSFacade.h"
#include "sp/SpFacade.h"
#include "qps/errors/QPSLexerError.h"
#include "qps/errors/QPSParserSyntaxError.h"
#include "qps/errors/QPSParserSemanticError.h"

using std::shared_ptr, std::vector, std::make_shared;

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
  pkb = new PKB();
  pkbWriter = new PkbWriter(pkb);
  shared_ptr<PkbQueryHandler> pkbQH_ptr = make_shared<PkbQueryHandler>(pkb);

  qps = new QPSFacade(pkbQH_ptr);
  sp = new SpFacade();
}

TestWrapper::~TestWrapper() {
  delete(qps);
  delete(sp);
  delete(pkbWriter);
  delete(pkb);
}

// method for parsing the SIMPLE source
void TestWrapper::parse(std::string filename) {
  sp->parseSource(filename, pkbWriter);
}

// method to evaluating a query
void TestWrapper::evaluate(std::string query, std::list<std::string>& results){
// call your evaluator to evaluate the query here
  UniqueVectorPtr<string> queryResult;
  try {
    queryResult = qps->evaluate(query);
  } catch (QPSLexerError QPSParserSyntaxError) {
    results.push_back("SyntaxError");
    return;
  } catch (QPSParserSyntaxError) {
    results.push_back("SyntaxError");
    return;
  } catch (QPSParserSemanticError) {
    results.push_back("SemanticError");
    return;
  } catch (std::exception) {
    return;
  } catch (...) {
    return;
  }

  // store the answers to the query in the results list (it is initially empty)
  // each result must be a string.
  for (auto qr : *queryResult) {
    results.push_back(qr);
  }
}

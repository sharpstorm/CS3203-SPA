#include <list>
#include <vector>

using std::vector, std::list;

class CFG {
 public:
  explicit CFG();
  void addNode(int lineNum1, int lineNum2);

 private:
  vector<int> nodeMap;
  vector<list<int>> links;
};
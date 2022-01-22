#include <iostream>
using namespace std;

struct Pixel{
private:
  int label;
  int order;

public:
  getLabel() { return label; }
  getOrder() { return order; }
  setLabel(int l) { label = l; }
  setOrder(int o) { order = o; }
};

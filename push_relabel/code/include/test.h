#include <unordered_map>
#include <functional>
#include "util.h"

bool testGraphData(PushRelabel&);
bool testHub(int, PushRelabel&);

typedef std::unordered_map<int, bool(*)(PushRelabel&)> testFunMap;


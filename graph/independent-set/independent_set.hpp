#ifndef GUARD_INDEPENDENT_SET
#define GUARD_INDEPENDENT_SET

#include <cstdlib>
#include <vector>
#include <iostream>
#include <cassert>

class IndependentSet{
  typedef unsigned long long ll;
  std::vector<ll> graph;
  ll              used;
  inline int  degree(int v) { return __builtin_popcountll(graph[v] & ~used); }
public:
  
  IndependentSet(int V) : graph(V), used(0) {
    assert(V < 64);
    for (int i = 0; i < V; i++){
      graph[i] |= 1LL << i;
    }
  }
  
  void add_edge(int u, int v){
    assert(u < (int)graph.size() && v < (int)graph.size());
    graph[u] |= 1LL << v;
    graph[v] |= 1LL << u;
  }
  
  int solve(){
    int res      = 0;
    int next     = -1;
    ll  old_used = used;
    
    for (size_t v = 0; v < graph.size(); v++){
      if (used & (1LL << v)){
        continue;
      } else if (degree(v) <=  1){
        next = v;
        break;
      } else if (next == -1 || degree(v) > degree(next)){
        next = v;
      }
    }

    if (next == -1){
      return 0;
    } else if (degree(next) > 1){
      used |= 1LL << next;
      res = solve();
      used = old_used;
    }
    
    used |= graph[next];
    res = std::max(res, solve() + 1);
    used = old_used;
    return res;
  }
};


#endif
  

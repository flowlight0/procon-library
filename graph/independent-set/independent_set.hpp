#ifndef GUARD_INDEPENDENT_SET
#define GUARD_INDEPENDENT_SET

#include <cstdlib>
#include <vector>
#include <iostream>
#include <cassert>
#include <bitset>

template <size_t V> class IndependentSet{
  typedef unsigned long long ll;
  std::vector<std::bitset<V> > graph;
  std::bitset<V> used_mask;
  inline int  degree(int v) { return (graph[v] & ~used_mask).count(); }
public: 
  IndependentSet() : graph(V) {
    for (size_t v = 0; v < V; v++){
      graph[v].set(v, 1);
    }
  }
  
  void AddEdge(int u, int v){
    assert(u < (int)graph.size() && v < (int)graph.size());
    graph[u].set(v, 1);
    graph[v].set(u, 1);
  }
  
  int Solve_(){
    // std::cout << used_mask << std::endl;
    int res      = 0;
    int next     = -1;
    std::bitset<V> old_mask = used_mask;
    
    for (size_t v = 0; v < graph.size(); v++){
      if (used_mask.test(v)){
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
      used_mask.set(next, 1);
      res = Solve_();
      used_mask.set(next, 0);
    }

    used_mask |= graph[next];
    res = std::max(res, Solve_() + 1);
    used_mask = old_mask;
    return res;
  }

  int Solve(int N){
    return Solve_() - (V - N);
  }
};


#endif
  

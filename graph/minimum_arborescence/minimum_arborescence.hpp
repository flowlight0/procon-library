#ifndef MINIMUM_ARBORESCENCE_H
#define MINIMUM_ARBORESCENCE_H

#include <vector>
#include <limits>
using namespace std;

template <typename T> class MinimumArborescence {
  typedef pair<int, T> Edge;
  typedef vector<vector<Edge> > Graph;
  static constexpr T INF = numeric_limits<T>::max();
  
public: 
  int dfsCount(int v, vector<int> &visit, const Graph &G){
    int res  = 1;
    visit[v] = 1;
    for (const Edge &e : G[v]){
      if (visit[e.first] == -1) res += dfsCount(e.first, visit, G);
    }
    return res;
  }

  T solve(int r, Graph G){
    int n = G.size();
    
    // Check the existence of an arborescence
    vector<int> visit(n, -1);
    if (dfsCount(r, visit, G) != n) return INF;
    
    // Construct a reversed graph
    Graph H(n);
    for (int v = 0; v < n; v++){
      for (const Edge &e: G[v]){
        H[e.first].push_back(make_pair(v, e.second));
      }
    }
    
    T cost = 0;
    for (vector<int> active(n, true); ;){
      vector<T>   min_cost(n, INF);
      vector<int> min_prev(n);
      vector<int> visit(n, -1);
      for (int v = 0; v < n; v++){
        for (const Edge &e : H[v]){
          if (v != r && active[v] && active[e.first] && e.second < min_cost[v]){
            min_cost[v] = e.second;
            min_prev[v] = e.first;
          }
        }
      }
      
      bool has_cycle = false;
      for (int u = 0; u < n; u++){
        if (visit[u] != -1 || !active[u] || u == r) continue;
        
        int v = visit[u] = u;
        for (v = min_prev[v]; v != r && visit[v] == -1; v = min_prev[v]){
          visit[v] = u;
        }
        
        if (visit[v] == u){     // cycle found
          active.push_back(true);
          H.push_back(vector<Edge>());
          G.push_back(vector<Edge>());
          vector<int> cycle_nodes;
          for (int w = v; cycle_nodes.empty() || w != v; w = min_prev[w]){
            active[w] = false;
            cycle_nodes.push_back(w);
          }
          for (int v : cycle_nodes){
            cost += min_cost[v];
            for (const auto &e : H[v]){
              if (active[e.first]){
                H[n].push_back(Edge(e.first, e.second - min_cost[v]));
                G[e.first].push_back(Edge(n, e.second - min_cost[v]));
              }
            }
            for (const auto &e : G[v]){
              if (active[e.first]){
                G[n].push_back(Edge(e.first, e.second));
                H[e.first].push_back(Edge(n, e.second));
              } 
            }
          }
          has_cycle = true;
          n++;
        }
      }
      
      if (!has_cycle){
        for (int v = 0; v < n; v++){
          if (v != r && active[v]) cost += min_cost[v];
        }
        return cost;
      }
    }
  } 
};

#endif /* MINIMUM_ARBORESCENCE_H */

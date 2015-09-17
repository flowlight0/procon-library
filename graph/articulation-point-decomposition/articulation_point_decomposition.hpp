#ifndef ARTICULATION_POINT_DECOMPOSITION_H
#define ARTICULATION_POINT_DECOMPOSITION_H

#include <vector>
#include <stack>
#include <cstdlib>
#include <tuple>
#include <stack>
#include <algorithm>
#include <cassert>

#ifndef fst
#define fst first
#define snd second
#endif

using std::vector;
using std::pair;

class ArticulationPointDecomposition {
  vector<vector<pair<int, int> > > G;
  vector<int> ord;
  vector<int> low;
  vector<int> par;
  void Dfs(int v, int p, int &times, std::stack<std::tuple<int, int, int> > &S);
  
public:
  vector<int> articulation_point;
  vector<vector<int> > edge_group;
  ArticulationPointDecomposition(const vector<pair<int, int> > &es);
};

void ArticulationPointDecomposition::Dfs(int v, int p, int &times, std::stack<std::tuple<int, int, int> > &S){
  ord[v] = low[v] = ++times;
  for (const auto &e : G[v]){
    int w = e.fst;
    int i = e.snd;
    
    if (ord[w] < ord[v] && w != p) {
      S.push(std::make_tuple(v, w, i));
    }

    if (ord[w] == -1){
      Dfs(w, v, times, S);
      low[v] = std::min(low[v], low[w]);

      if ((ord[v] == 1 && ord[w] != 2) || (ord[v] != 1 && low[w] >= ord[v])){
        articulation_point.push_back(v);
      }
      
      if (low[w] >= ord[v]){
        edge_group.push_back(vector<int>());
        while (true){
          int a, b, j;
          std::tie(a, b, j) = S.top(); S.pop();
          edge_group.back().push_back(j);
          if (v == a && w == b) break;
        }
      } 
    } else {
      low[v] = std::min(low[v], ord[w]);
    }
  }
}

ArticulationPointDecomposition::ArticulationPointDecomposition(const vector<pair<int, int> > &es){
  assert(es.size() < 1ull << 31);
  int V = 0;
  for (const auto &e : es) V = std::max(V, std::max(e.fst, e.snd) + 1);
  
  ord = vector<int>(V, -1);
  low = vector<int>(V, -1);
  par = vector<int>(V, -1);
  G   = vector<vector<pair<int, int> > >(V);
  for (size_t i = 0; i < es.size(); i++){
    G[es[i].fst].emplace_back(es[i].snd, i);
    G[es[i].snd].emplace_back(es[i].fst, i);
  }

  for (int v = 0; v < V; v++){
    if (ord[v] == -1){
      int times = 0;
      std::stack<std::tuple<int, int, int> > S;
      Dfs(v, -1, times, S);
    }
  }
  std::sort(articulation_point.begin(), articulation_point.end());
  articulation_point.erase(std::unique(articulation_point.begin(), articulation_point.end()),
                           articulation_point.end());
}

#endif /* ARTICULATION_POINT_DECOMPOSITION_H */

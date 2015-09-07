#ifndef BRIDGE_DECOMPOSITION_H
#define BRIDGE_DECOMPOSITION_H

#include <queue>
#include <vector>
#include <cstdlib>
#include <algorithm>
using std::vector;

class BridgeDecomposition{
  #define fst first
#define snd second
  std::vector<std::vector<int> >  G;
  std::vector<int> ord;
  std::vector<int> low;
  std::vector<int> par;
  void Dfs(int v, int &times);
  
public:
  std::vector<std::vector<int> > edge_group;
  BridgeDecomposition(const std::vector<std::pair<int, int> > &es);
  std::vector<int> GetLargestBiconnectedComponet();
};

void BridgeDecomposition::Dfs(int v, int &times){
  ord[v] = low[v] = times++;
  for (int w : G[v]){
    if (ord[w] == -1){
      par[w] = v;
      Dfs(w, times);
      low[v] = std::min(low[v], low[w]);
    } else if (w != par[v]){
      low[v] = std::min(low[v], low[w]);
    }
  }
}

BridgeDecomposition::BridgeDecomposition(const vector<std::pair<int, int> > &es){
  int V = 0;
  for (const auto &e : es) V = std::max(V, std::max(e.fst, e.snd) + 1);
  
  ord = vector<int>(V, -1);
  low = vector<int>(V, -1);
  par = vector<int>(V, -1);
  G   = vector<vector<int> >(V);
  for (const auto &e : es){
    G[e.fst].push_back(e.snd);
    G[e.snd].push_back(e.fst);
  }

  int times = 0;
  for (int v = 0; v < V; v++) if (ord[v] == -1){
    Dfs(v, times);
  }
  
  G.clear();
  G = vector<vector<int> > (V);
  
  for (size_t i = 0; i < es.size(); i++){
    std::pair<int, int> e = es[i];
    if (par[e.snd] != e.fst) std::swap(e.fst, e.snd);
    if (par[e.snd] != e.fst) continue;
    if (low[e.snd] > ord[e.fst]){
      edge_group.push_back(vector<int>(1, i));
    } else {
      G[e.fst].push_back(e.snd);
      G[e.snd].push_back(e.fst);
    }
  }
  
  vector<int> visit(V, -1);
  
  int num_cc = edge_group.size();
  for (int s = 0; s < V; s++) if (visit[s] == -1){
    
    visit[s] = num_cc;
    int visit_count = 1;
    std::queue<int> que;
    que.push(s);
    while (!que.empty()){
      int v = que.front(); que.pop();
      for (int w : G[v]){
        if (visit[w] == -1){
          visit[w] = num_cc;
          visit_count++;
          que.push(w);
        }
      }
    }

    num_cc++;
  }
  edge_group.resize(num_cc);
  // cerr << es.size() << " " <<
  for (size_t i = 0; i < es.size(); i++){
    std::pair<int, int> e = es[i];
    if (visit[e.fst] == visit[e.snd]) edge_group[visit[e.fst]].push_back(i);
  }  
}

vector<int> BridgeDecomposition::GetLargestBiconnectedComponet(){
  const auto &eg = this->edge_group;
  int best_id = 0;
  for (size_t i = 0; i < eg.size(); i++){
    if (eg[i].size() > eg[best_id].size()) best_id = i;
  }
  return eg[best_id];
}

#endif /* BRIDGE_DECOMPOSITION_H */

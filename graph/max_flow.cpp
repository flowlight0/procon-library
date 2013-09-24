/************************************************************
Maximum Flow
Dinic's Algorithm: O(|E| |V|^2)

verified at
http://www.spoj.com/problems/FASTFLOW/
************************************************************/

#include <vector>
#include <queue>
#include <cstdlib>
#include <algorithm>
#include <climits>

class MaxFlow{
  typedef long long ll;
  
  struct edge{
    int to, rev;
    ll cap;
    edge(){}
    edge(int to, ll cap, int rev) : to(to), cap(cap), rev(rev){}
  };
  
  std::vector<std::vector<edge> > G;
  std::vector<int> level;
  std::vector<int> iter;

  void bfs(int s){
    std::fill(level.begin(), level.end(), -1);
    std::queue<int> que;
    level[s] = 0;
    que.push(s);
    while(!que.empty()){
      int v = que.front(); que.pop();
      for(int i = 0; i < (int)G[v].size(); i++){
	edge &e = G[v][i];
	if(e.cap > 0 && level[e.to] < 0){
	  level[e.to] = level[v] + 1;
	  que.push(e.to);
	}
      }
    }
  }

    
  ll dfs(int v, int t, ll f){
    if(v == t) return f;
    
    for(int &i = iter[v]; i < (int)G[v].size(); i++){
      edge &e = G[v][i];
      if(e.cap > 0 && level[e.to] > level[v]){
	ll d = dfs(e.to, t, std::min(f, e.cap));
	if(d > 0){
	  e.cap -= d;
	  G[e.to][e.rev].cap += d;
	  return d;
	}
      }
    }
    return 0;
  }
  
public:
  MaxFlow(int V) :
    G(std::vector<std::vector<edge> >(V)),
    level(std::vector<int>(V)),
    iter (std::vector<int>(V)) { }
  
  void add_edge(int from, int to, ll cap){
    G[from].push_back(edge(to, cap, G[to].size()));
    G[to].push_back(edge(from, 0, G[from].size() - 1));
  }

  ll solve(int s, int t){
    ll res = 0, f;
    for(;;){
      bfs(s);
      if(level[t] < 0) break;
      std::fill(iter.begin(), iter.end(), 0);
      while((f = dfs(s, t, LONG_LONG_MAX)) > 0) res += f;
    }
    return res;
  }
};



// solution for http://www.spoj.com/problems/FASTFLOW/
#include <iostream>
using namespace std;

int main(){
  int N, M;
  cin >> N >> M;

  MaxFlow mf(N);
  
  for(int i = 0; i < M; i++){
    int from, to, cost;
    cin >> from >> to >> cost;
    from--, to--;
    mf.add_edge(from, to, cost);
    mf.add_edge(to, from, cost);    
  }
  
  cout << mf.solve(0, N - 1) << endl;
}

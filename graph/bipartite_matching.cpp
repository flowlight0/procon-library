/************************************************************
Hopcroft–Karp algorithm O(|E| |V|^{1/2})

verified at
http://www.spoj.com/problems/MATCHING/

reference
http://en.wikipedia.org/wiki/Hopcroft%E2%80%93Karp_algorithm
 ************************************************************/


#include <vector>
#include <queue>
#include <cassert>
using namespace std;

class BipartiteMatching{
  int V;
  int W;
  vector<int> match;
  vector<int> level;
  vector<vector<int> > graph;
  
  int bfs(){
    fill(level.begin(), level.end(), -1);
    queue<int> que;
    for(int v = 0; v < V; v++){
      if(match[v] == -1){
        que.push(v);
        level[v] = 0;
      }
    }
    bool done = true;
    while(!que.empty()){
      int v = que.front(); que.pop();
      for(size_t i = 0; i < graph[v].size(); i++){
        int w = graph[v][i], nextv = match[w];
        if(nextv == -1){
          done = false;
        }else if(level[nextv] == -1){
          level[nextv] = level[v] + 1;
          que.push(nextv);
        }
      }
    }
    return !done;
  }
  
  bool dfs(int v){
    for(size_t i = 0; i < graph[v].size(); i++){
      int w = graph[v][i], nextv = match[w];
      if(nextv == -1 || (level[nextv] == level[v] + 1 && dfs(nextv))){
        match[v] = w;
        match[w] = v;
        return true;
      }
    }
    level[v] = -2;
    return false;
  }

public:
  BipartiteMatching(int _V, int _W) :
    V(_V), W(_W),
    match(vector<int>(V + W, -1)),
    level(vector<int>(V)),
    graph(vector<vector<int> >(V)){}
  
  void add_edge(int from, int to){
    assert(0 <= from && from < V && V <= to && to < V + W);
    graph[from].push_back(to);
  }
  
  int solve(){
    int ans = 0;
    while(bfs()){
      for(int v = 0; v < V; v++){
        if(match[v] == -1 && dfs(v)) ans++;
      }
    }
    return ans;
  }
};


// solution for http://www.spoj.com/problems/MATCHING/
#include <iostream>

int main(){
  int N, M, P, A, B;
  cin >> N >> M >> P;
  BipartiteMatching bm(N, M);
  for(int i = 0; i < P; i++){
    cin >> A >> B;
    A--, B--;
    bm.add_edge(A, B + N);
  }
  cout << bm.solve() << endl;
  return 0;
}

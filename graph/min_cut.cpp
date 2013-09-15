/********************************************************
Minimum Cut
Shoer-Wagner O(|V| ^ 3)

http://www.prefield.com/algorithm/graph/minimum_cut.html
を元に実装。
最小カットの容量だけではなく集合も求めるようにしたためコードが
長くなった。

verified at
http://poj.org/problem?id=2914
******************************************************* */

#include <vector>
#include <algorithm>
#include <set>

template <typename T> class MinCut{
  typedef std::vector<std::vector<T> > Graph;
  typedef std::set<int>                Cut;

  int   N;
  Graph G;
  
  std::pair<T, Cut> solve(Graph &H, std::vector<int> V, int N){
    if(N == 2){
      Cut cut; cut.insert(V[0]);
      return std::make_pair(H[V[0]][V[1]], cut);
    }
    
    std::vector<T> S(N, 0);
    int u, v;
    T w;
    for(int i = 0; i < N; i++){
      u    = v;
      v    = max_element(S.begin(), S.end()) - S.begin();
      w    = S[v];
      S[v] = -1;
      for(int j = 0; j < N; j++){
        if(S[j] >= 0){
          S[j] += H[V[v]][V[j]];
        }
      }
    }
    
    int vp = V[v], up = V[u];
    for(int i = 0; i < N; i++){
      H[V[i]][up] += H[V[i]][vp];
      H[up][V[i]] += H[vp][V[i]]; 
    }
    V.erase(V.begin() + v);
      
    std::pair<T, Cut> ans = solve(H, V, N - 1);
    if(ans.first > w){
      Cut c; c.insert(vp);
      ans = std::make_pair(w, c);
    }
    if(ans.second.count(up)) ans.second.insert(vp);
      
    return ans;
  }
  
public:
  MinCut(int N) : N(N), G(Graph(N, std::vector<T>(N))){}
    
  void add_edge(int u, int v, T cap){
    G[u][v] += cap;
    G[v][u] += cap;
  }
  
  T capacity(int u, int v) const {
    return G[u][v];
  }
  
  std::pair<T, Cut> solve(){
    int   N = G.size();
    Graph H = G;
    std::vector<int> V(N);
    for(int i = 0; i < N; i++) V[i] = i;
    return N < 2 ? std::make_pair(0, Cut()) : solve(H, V, N);
  }
};


// solution for http://poj.org/problem?id=2914
#include <cstdio>
#include <cassert>
using namespace std;

int main(){
  int N, M;
  while(scanf("%d%d", &N, &M) != EOF){
    MinCut<int> mincut(N);
    for(int i = 0; i < M; i++){
      int a, b, cap;
      scanf("%d%d%d", &a, &b, &cap);
      mincut.add_edge(a, b, cap);
    }

    std::pair<int, std::set<int> > res = mincut.solve();
    int c = 0;
    // for(set<int>::iterator iter = res.second.begin();
    //     iter != res.second.end(); iter++)
    //   {
    //     cout << "cut: " << *iter << endl;
    //   }

    for(int i = 0; i < N; i++){
      for(int j = 0; j < i; j++){
        if(res.second.count(i) != res.second.count(j)){
          c += mincut.capacity(i, j);
        }
      }
    }
    // cout << c << " " << res.first << endl;
    assert(c == res.first);
    
    printf("%d\n", res.first);
  }
  return 0;
}

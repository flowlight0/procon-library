/***********************************************************
有向グラフの最小平均長閉路を求めるO(NM)閉路の長さだけしかまだ実装してな
いhttp://www.spoj.com/problems/WORDRING/
***********************************************************/
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <limits>
#include <cassert>

class MinMeanCycle{
  typedef std::vector<int>    Ivec;
  typedef std::vector<double> Dvec;
    
  Ivec from, to;
  Dvec costs;
    
public:
  
  void add_edge(int u, int v, double cost){
    from  .push_back(u);
    to    .push_back(v);
    costs .push_back(cost);
  };
    
  std::pair<double, Ivec > solve(){
    int V = 2;
    int E = from.size();
    for (int i = 0; i < E; i++){
      V = std::max(V, std::max(from[i], to[i]) + 2);
    }
        
    const double inf = std::numeric_limits<double>::max() / 10;
    std::vector<Dvec> F(V + 1, Dvec(V, inf));
    std::fill(F[1].begin(), F[1].end(), 0);

    for (int i = 2; i <= V; i++){
      for (int e = 0; e < E; e++){
        int    f = from[e];
        int    t = to[e];
        double c = costs[e];
        if (F[i - 1][f] < inf && F[i - 1][f] + c < F[i][t]){
          F[i][t] = F[i - 1][f] + c;
        }
      }
    }
        
    int    res_x = -1;
    double res_c;
        
    for (int x = 0; x < V; x++){
      double c = -inf;
      for (int k = 1; k <= V - 1; k++){
        if (F[k][x] == inf) continue;
        c = std::max(c, (F[V][x] - F[k][x]) / (V - k));
      }
      if (res_x == -1 || c < res_c){
        res_x = x;
        res_c = c;
      }
    }
        
    if (res_x == -1){
      return std::make_pair(inf, Ivec());
    } else{
      Ivec cycle;
      return std::make_pair(res_c, cycle);
    }
  };
};

/***********************************************************
solution for
http://www.spoj.com/problems/WORDRING/
***********************************************************/
#include <cstdio>
#include <cstring>
#include <iostream>
using namespace std;

int main(){
  int n;
  char buf[1010];
  while (cin >> n && n){
    MinMeanCycle mmc;

    int count = 0;
    while (n--){
      scanf("%s\n", buf);
      int len = strlen(buf);
      if (len > 1){
        count++;
        int f = (buf[0] - 'a') * 30 + (buf[1] - 'a');
        int t = (buf[len - 2] - 'a') * 30 + (buf[len - 1] - 'a');
        mmc.add_edge(f, t, -len);
      }
    }

    double c = -mmc.solve().first;
    if (count == 0 || c < -1e5){
      printf("No solution.\n");
    } else{
      printf("%.2lf\n", c + 1e-9);
    }
  }
  return 0;
}



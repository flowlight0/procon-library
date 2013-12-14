/*********************************************************
UnionFind

find  : O(logn)
weight: O(logn)
same  : O(logn)
unite : O(logn)

verified at
http://www.spoj.com/problems/MST/ (only find, same and unite are used.)
***********************************************************/
		
#include <vector>
using namespace std;

class UnionFind{
    vector<int> parent;
    vector<int> count;
    vector<int> rank;
public:
    UnionFind(int N) : parent(vector<int>(N)),
                       count(vector<int>(N, 1)),
                       rank(vector<int>(N, 0)){
        for(int i = 0; i < N; i++) parent[i] = i;
    }
  
    int find(int x){
        if(x == parent[x]) return x;
        else return parent[x] = find(parent[x]);
    }
  
    int weight(int x){
        return count[find(x)];
    }
  
    bool same(int x, int y){
        return find(x) == find(y);
    }
    
    void unite(int x, int y){
        x = find(x);
        y = find(y);
        if(x == y) return;
        if(rank[x] < rank[y]){
            count[y] += count[x];
            parent[x] = y;
        }else{
            count[x] += count[y];
            parent[y] = x;
            if(rank[x] == rank[y]) rank[y]++;
        }
    }
};


// solution for http://www.spoj.com/problems/MST/

#include <cstdio>
#include <algorithm>

struct edge{
    int from, to, cost;
    edge(){}
    edge(int from, int to, int cost) : from(from), to(to), cost(cost){}
  
    friend bool operator<(const edge &e1, const edge &e2){
        return e1.cost < e2.cost;
    }
};


int main(){
    int N, M;
    scanf("%d%d", &N, &M);
  
    vector<edge> es(M);
  
    for(int i = 0; i < M; i++){
        scanf("%d%d%d", &es[i].from, &es[i].to, &es[i].cost);
        es[i].from--;
        es[i].to--;
    }
    sort(es.begin(), es.end());
  
    long long res = 0;

    UnionFind uf(N);
	       
    for(int i = 0; i < M; i++){
        if(!uf.same(es[i].from, es[i]. to)){
            res += es[i].cost;
            uf.unite(es[i].from, es[i]. to);
        }
    }
  
    printf("%lld\n", res);
  
    return 0;
}

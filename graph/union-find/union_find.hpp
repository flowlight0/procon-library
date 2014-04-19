/*********************************************************
UnionFind

find(x)     : return the id of the component including x in O(logn) time.
size(x)     : return size of the component including x in O(logn) time.
same(x, y)  : return true if x and y are in the same component in O(logn) time.
unite(x, y) : create an edge between x and y in O(logn) time.
count()     : return the current number of components in O(1) time.

verified at
http://www.spoj.com/problems/MST/ (only find, same and unite are used.)
***********************************************************/

#ifndef GUARD_UNION_FIND
#define GUARD_UNION_FIND

#include <vector>

class UnionFind{
  int num_components;
  std::vector<int> parent;
  std::vector<int> weight;
  std::vector<int> rank;
public:
  UnionFind(int N) : num_components(N),
                     parent(std::vector<int>(N)),
                     weight(std::vector<int>(N, 1)),
                     rank(std::vector<int>(N, 0)){
    for(int i = 0; i < N; i++) parent[i] = i;
  }
  
  int find(int x){
    if(x == parent[x]) return x;
    else return parent[x] = find(parent[x]);
  }
  
  int size(int x){
    return weight[find(x)];
  }

  
  bool same(int x, int y){
    return find(x) == find(y);
  }
    
  void unite(int x, int y){
    x = find(x);
    y = find(y);
    if(x == y) return;

    num_components--;
    if(rank[x] < rank[y]){
      weight[y] += weight[x];
      parent[x] = y;
    }else{
      weight[x] += weight[y];
      parent[y] = x;
      if(rank[x] == rank[y]) rank[y]++;
    }
  }
  
  int count(){
    return num_components;
  }
};


#endif

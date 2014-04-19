/***************************************
Segment Tree

answer these query in O(log N) times.
1. range sum     query
2. range minimum query
3. range add     query

verified at
 pku 3468 (only add and sum)

kyuridenamidaさんの記事を参考にしてます。
***************************************/

#ifndef GUARD_SEGMENT_TREE
#define GUARD_SEGMENT_TREE

#include <algorithm>
#include <vector>
#include <limits>

template <typename T> class SegTree{
  struct node_t{
    T   min;
    T   sum;
    T   add;
    int pos;
  };
    
  int                 N;
  std::vector<T>      A;
  std::vector<node_t> data;

  inline void eval(int k, int l, int r){
    if(data[k].add == 0) return;
    
    if(r - l > 1){
      data[2 * k + 1].add += data[k].add;
      data[2 * k + 2].add += data[k].add;
    }
    
    data[k].sum += data[k].add * (r - l);
    data[k].min += data[k].add;
    data[k].add  = 0;
  }
  
  inline void merge(int k){
    int chl  = k * 2 + 1;
    int chr  = k * 2 + 2;
    T   lmin = data[chl].min;
    T   rmin = data[chr].min;
    data[k].sum = data[chl].sum + data[chr].sum;
    data[k].min = std::min(lmin, rmin);
    data[k].pos = lmin < rmin ? data[chl].pos : data[chr].pos;
  }
    
  void build(int k, int l, int r){
    if(r - l == 1){
      data[k].min = A[l];
      data[k].sum = A[l];
      data[k].pos = l;
      data[k].add = 0;
    }else{
      int m   = (l + r) / 2;
      int chl = 2 * k + 1;
      int chr = 2 * k + 2;
      build(chl, l, m);
      build(chr, m, r);
      merge(k);
    }
  }
  
  void add(int a, int b, T x, int k, int l, int r){
    eval(k, l, r);
    if(b <= l || r <= a){
      return;
    }else if(a <= l && r <= b){
      data[k].add += x;
      eval(k, l, r);
    }else{
      int m   = (l + r) / 2;
      int chl = k * 2 + 1;
      int chr = k * 2 + 2;
      add(a, b, x, chl, l, m);
      add(a, b, x, chr, m, r);
      merge(k);
    }
  }
  
  T min(int a, int b, int &pos, int k, int l, int r){
    eval(k, l, r);
    if(b <= l || r <= a){
      return std::numeric_limits<T>::max();
    }else if(a <= l && r <= b){
      pos = data[k].pos;
      return data[k].min;
    }else{
      int m   = (l + r) / 2;
      int chl = k * 2 + 1;
      int chr = k * 2 + 2;
      int lpos = -1, rpos = -1;
      T lmin = min(a, b, lpos, chl, l, m);
      T rmin = min(a, b, rpos, chr, m, r);
      merge(k);
      pos = lmin < rmin ? lpos : rpos;
      return std::min(lmin, rmin);
    }
  }
  
  T sum(int a, int b, int k, int l, int r){
    eval(k, l, r);
    if(b <= l || r <= a){
      return 0;
    }else if(a <= l && r <= b){
      return data[k].sum;
    }else{
      int m   = (l + r) / 2;
      int chl = k * 2 + 1;
      int chr = k * 2 + 2;
      T   vl  = sum(a, b, chl, l, m);
      T   vr  = sum(a, b, chr, m, r);
      merge(k);
      return vl + vr;
    }
  }
  
public:
  SegTree(size_t N) : N(N), A(std::vector<T>(N, 0)){
    data.resize(4 * N);
    build(0, 0, N);
  }
    
  SegTree(const std::vector<T> &A) : N(A.size()), A(A){
    data.resize(4 * N);
    build(0, 0, N);
  }
    
  void add(int l, int r, T x) {add(l, r, x, 0, 0, N);}
  T min(int l, int r, int &pos){pos = -1; return min(l, r, pos, 0, 0, N);}
  T min(int l, int r){int tmp;return min(l, r, tmp);}
  T sum(int l, int r){return sum(l, r, 0, 0, N);}
};

#endif

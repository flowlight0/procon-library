/*******************************************************

vector<int> vecに対してO(N)でSuffix Arrayの構築
Suffix ArrayからのLCPのO(N)構築

ただしvecの各要素はmax(vec.size(), 256)以下の正整数
終端文字'$'は使用しない

- Suffix Arrayのコードは元論文のものからほぼそのまま
Juha Karkkainen and Peter Sanders "Simple Linear Work Suffix Array construction"
- LCPのコードは蟻本ほぼそのまま

verified at
http://www.spoj.com/problems/SARRAY/ (構築)
http://www.spoj.com/problems/SUBST1/ (LCP)

*******************************************************/
#ifndef GUARD_SUFFIX_ARRAY
#define GUARD_SUFFIX_ARRAY

#include <string>
#include <vector>
#include <limits>
#include <algorithm>
#include <stack>
#include <iostream>
#include <cassert>
using namespace std;

class SuffixArray{
  typedef std::vector<int> Array;
  
  int   size;
  Array org_vec;
  Array suf;
  Array lcp;
  Array rank;
  std::vector<Array> rmq;
  bool use_rmq;
  
  inline bool leq(int a1, int a2, int b1, int b2){
    return a1 != b1 ? a1 < b1 : a2 <= b2;
  }
  
  inline bool leq(int a1, int a2, int a3, int b1, int b2, int b3){
    return a1 != b1 ? a1 < b1 : leq(a2, a3, b2, b3);
  }

  inline int get_i(int p, int n){
    return p < n ? p * 3 + 1 : (p - n) * 3 + 2;
  }
    
  void radix_pass(Array &a, Array &b, const Array &s,
                 int offset, int n, int alpha_size)
  {
    const int least_alphabet_size = 256;
    alpha_size = std::max(alpha_size, least_alphabet_size);
        
    Array count(alpha_size + 1, 0);
        
    for (int i = 0; i < n; i++) count[s[a[i]+offset]]++;
        
    for (int i = 0, sum = 0;  i <= alpha_size; i++){
      int tmp  = count[i];
      count[i] = sum;
      sum += tmp;
    }
        
    for (int i = 0; i < n; i++) b[count[s[a[i]+offset]]++] = a[i];
  }
    
  void build_sa(const Array &S, Array &suf, int n){
    int n0  = (n + 2) / 3;
    int n1  = (n + 1) / 3;
    int n2  = n / 3;
    int n02 = n0 + n2;
    Array S12  (n02 + 3), S0  (n0);
    Array suf12(n02 + 3), suf0(n0);
    
    for (int i = 0; i < 3; i++){
      S12[n02 + i] = suf12[n02 + i] = 0;
    }
        
    for (int i = 0, j = 0; i < n + (n0 - n1); i++){
      if(i % 3 != 0) S12[j++] = i;
    }
    
    // radix sort the mod 1 and 2 triples
    radix_pass(S12, suf12, S, 2, n02, S.size());
    radix_pass(suf12, S12, S, 1, n02, S.size());
    radix_pass(S12, suf12, S, 0, n02, S.size());
    
    // find lexicographic names of triples
    int name = 0;
    int c0 = -1, c1 = -1, c2 = -1;
    for (int i = 0; i < n02; i++){
      if (S[suf12[i]] != c0 || S[suf12[i]+1] != c1 || S[suf12[i]+2] != c2){
        name++;
        c0 = S[suf12[i]  ];
        c1 = S[suf12[i]+1];
        c2 = S[suf12[i]+2];
      }
      int offset = suf12[i] % 3 == 1 ? 0 : n0;
      S12[suf12[i]/3+offset] = name;
    }
    
    if (name < n02){
      build_sa(S12, suf12, n02);
      for (int i = 0; i < n02; i++) S12[suf12[i]] = i + 1;
    } else {
      for (int i = 0; i < n02; i++) suf12[S12[i] - 1] = i;
    }
    
    // sort the mod 0 suffixes by their first character.
    for (int i = 0, j = 0; i < n02; i++){
      if(suf12[i] < n0) S0[j++] = 3 * suf12[i];
    }
        
    radix_pass(S0, suf0, S, 0, n0, S.size());
    
    int t = n0 - n1, p = 0, k = 0;
    while(t < n02 && p < n0){
      int i        = get_i(suf12[t], n0);
      int j        = suf0[p];
      bool small12 = (suf12[t] < n0) ? 
        leq(S[i], S12[suf12[t]+n0], S[j], S12[j/3]) :
        leq(S[i], S[i+1], S12[suf12[t]-n0+1], S[j], S[j+1], S12[j/3+n0]);
      if(small12){
        suf[k] = i; t++;
      }else{
        suf[k] = j; p++;
      }
      k++;
    }
    while(t < n02){ suf[k++] = get_i(suf12[t], n0); t++;}
    while(p < n0 ){ suf[k++] = suf0[p++]; }
  }
    
  void build_lcp(){
    for(int i = 0; i < size; i++) rank[suf[i]] = i;
    int h = 0;
    for(int i = 0; i < size; i++){
      int j = rank[i] + 1 < size ? suf[rank[i] + 1] : size;
      for(h = std::max(0, h - 1); std::max(i, j) + h < size; h++)
        if(org_vec[j + h] != org_vec[i + h]) break;
      lcp[rank[i]] = h;
    }
  }
  
  void build_rmq(){
    rmq.clear();
    for (int r = 1; r <= size; r <<= 1){
      Array rmq_(size);
      Array &pre = rmq.empty() ? rmq_ : rmq.back();
      
      for (int i = 0; i <= size - r; i++){
        if (rmq.empty()){
          rmq_[i] = lcp[i];
        } else {
          rmq_[i]  = std::min(pre[i], pre[i + (r >> 1)]);
        }
      }
      rmq.push_back(rmq_);
    }
  }
    
public:
  SuffixArray(const Array &vec, bool use_rmq = false){ construct(vec, use_rmq);}
    
  void construct(const Array &vec_, bool use_rmq_){
    use_rmq = use_rmq_;
    size    = vec_.size();
    org_vec = vec_;
    for(int i = 0; i < 3; i++) org_vec.push_back(0);
    suf  .resize(org_vec.size());
    lcp  .resize(org_vec.size());
    rank .resize(org_vec.size());
    
    build_sa (org_vec, suf, size);
    build_lcp();
    if (use_rmq) build_rmq();
  }

  int operator[](size_t pos) const { return suf[pos]; }
  int get_height(size_t pos) const { return lcp[pos]; }
  int get_rank  (size_t pos) const { return rank[pos]; }
  int get_size() const { return size; }
  int get_lcp(int l, int r) const {
    if (!use_rmq || r - l < 1){
      return -1;
    } else {
      int k = 31 - __builtin_clz(r - l);
      return std::min(rmq[k][l], rmq[k][r - (1 << k)]);
    }
  }
};

#endif

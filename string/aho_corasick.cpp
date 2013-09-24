// Aho Corasick
// O(\sum_{i}{|P_i|})で構築
// verified at
// http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=1320


#include <string>
#include <vector>
#include <queue>
#include <cassert>
#include <numeric>
#include <iostream>
#include <algorithm>

class AhoCorasick{
  static const int ASIZE = 256;
  
  struct node_t {
    int     ID;
    node_t *failure;
    node_t *output;
    node_t *next[ASIZE];
    std::vector<int> match;
    
    node_t(int id) : ID(id), failure(NULL), output(NULL){
      std::fill(next, next + ASIZE, (node_t*)NULL);
    }
  };

  size_t n_size;
  size_t length;
  node_t *root;
  node_t **nodes;
  
  inline node_t *new_node(){
    nodes[n_size] = new node_t(n_size);
    return nodes[n_size++];
  }
  
public:
  AhoCorasick(const std::vector<std::string> &pats) : n_size(0){
    length = accumulate(pats.begin(), pats.end(), std::string("")).size();
    
    nodes = new node_t* [length + 1];
    root  = new_node();
    
    // construct key word tree
    
    for(size_t i = 0; i < pats.size(); i++){
      node_t *v = root;
      for(size_t j = 0; j < pats[i].size(); j++){
        int c = pats[i][j];
        if(!v->next[c]) v->next[c] = new_node();
        v = v->next[c];
      }
      v->match.push_back(i);
    }
    
    // construct failure link and output link
    std::queue<node_t*> que;
    root->failure = root;
    root->output  = root;

    que.push(root);
    while(!que.empty()){
      node_t *p = que.front(); que.pop();
      
      for(int x = 0; x < ASIZE; x++){
        if(!p->next[x]) continue;
        node_t *v = p->next[x];
        node_t *w = p->failure;
        
        if(p == root){
          v->failure = root;
          v->output  = root;
        }else{
          while(w != root && !w->next[x]) w = w->failure;
          v->failure = w->next[x] ? w->next[x] : root;
          
          if(v->failure->match.empty()){
            v->output = v->failure->output;
          }else{
            v->output = v->failure;
          }
        }
        que.push(v);
      }
    }
  }

  ~AhoCorasick(){
    for(size_t i = 0; i < n_size; i++) delete nodes[i];
    delete [] nodes;
  }
  
  // initial state is always zero.
  size_t state_size() const{ return n_size; }
  int    init_state() const{ return 0; }
  
  int next_state(size_t state, int c) const{
    assert(state < n_size);
    node_t *v = nodes[state];
    while(v != root && !v->next[c]) v = v->failure;
    v = v->next[c] ? v->next[c] : root;
    return v->ID;
  }

  std::vector<int> match(size_t state) const{
    assert(state < n_size);
    
    std::vector<int> res;
    for(node_t *v = nodes[state]; v != root; v = v->output){
      for(size_t i = 0; i < v->match.size(); i++){
        res.push_back(v->match[i]);
      }
    }
    return res;
  }
  
};

#include <cstring>
#include <tuple>
#include <queue>
using namespace std;


int dist[60][60][110];

int main(){
  int N, M, P, sr, sc, gr, gc;
  char field[60][60];

  int dr[]   = {1, 0, -1, 0};
  int dc[]   = {0, -1, 0, 1};
  char dir[] = {'D', 'L', 'U', 'R'};

  
  while(cin >> N >> M && N + M){

    for(int i = 0; i < N; i++){
      for(int j = 0; j < M; j++){
        cin >> field[i][j];
        if(field[i][j] == 'S'){
          sr = i;
          sc = j;
        }
        if(field[i][j] == 'G'){
          gr = i;
          gc = j;
        }
      }
    }
    
    cin >> P;
    vector<string> ps(P);
    for(int i = 0; i < P; i++) cin >> ps[i];
    AhoCorasick aho(ps);

    memset(dist, -1, sizeof(dist));

    
    queue<tuple<int, int, int>> que;
    que.push(make_tuple(sr, sc, aho.init_state()));
    dist[sr][sc][aho.init_state()] = 0;

    bool ok = false;
    
    while(!que.empty()){
      int r, c, s;
      std::tie(r, c, s) = que.front(); que.pop();
      
      if(r == gr && c == gc){
        cout << dist[r][c][s] << endl;
        ok = true;
        break;
      }

      for(int i = 0; i < 4; i++){
        int nr = r + dr[i];
        int nc = c + dc[i];
        int ns = aho.next_state(s, dir[i]);
        
        if(!aho.match(ns).empty()) continue;
        
        if(0 <= nr && nr < N && 0 <= nc && nc < M &&
           field[nr][nc] != '#' && dist[nr][nc][ns] == -1)
          {
            dist[nr][nc][ns] = dist[r][c][s] + 1;
            que.push(make_tuple(nr, nc, ns));
          }
        
      }
    }
    if(!ok) cout << -1 << endl;
    
  }
}

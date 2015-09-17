#include <gtest/gtest.h>
#include <vector>
#include <cstdlib>
#include "independent_set.hpp"
using namespace std;

int brute_force(const vector<vector<int> > &adj){
  int n   = adj.size();
  int res = 0;
  for (int mask = 0; mask < (1 << n); mask++){
    if (res >= __builtin_popcount(mask)) continue;
    bool ok = true;
    for (int i = 0; i < n; i++){
      if (((mask >> i) & 1) == 0) continue;;
      for (int j = 0; j < i; j++){
        if (((mask >> j) & 1) == 1 && adj[i][j]) ok = false;
      }
    }
    if (ok) res = __builtin_popcount(mask);
  }
  return res;
}

vector<vector<int> > generate_graph(int N, double prob){
  vector<vector<int> > G(N, vector<int>(N));
  for (int i = 0; i < N; i++){
    for (int j = 0; j < N; j++){
      double p = (double) rand() / RAND_MAX;
      if (p < prob){
        G[i][j] = 1;
        G[j][i] = 1;
      }
    }
  }
  return G;
}


const int NMAX = 15;
TEST(SMALL_TEST, LINE){
  for (int N = 1; N <= NMAX; N++){
    IndependentSet<NMAX> iset;
    vector<vector<int> > G(N, vector<int>(N, 0));
    for (int i = 0; i < N - 1; i++){
      iset.AddEdge(i, i + 1);
    }
    ASSERT_EQ(iset.Solve(N), (N + 1) / 2);
  }
}

TEST(SMALL_TEST, STAR){
  for (int N = 1; N <= NMAX; N++){
    IndependentSet<NMAX> iset;
    vector<vector<int> > G(N, vector<int>(N, 0));
    for (int i = 0; i < N - 1; i++){
      iset.AddEdge(0, i + 1);
    }
    ASSERT_EQ(iset.Solve(N), N == 1 ? 1 : N - 1);
  }
}

TEST(SMALL_TEST, CIRCLE){
  for (int N = 1; N <= NMAX; N++){
    IndependentSet<NMAX> iset;
    vector<vector<int> > G(N, vector<int>(N, 0));
    for (int i = 0; i < N; i++){
      iset.AddEdge(i, (i + 1) % N);
    }
    ASSERT_EQ(iset.Solve(N), N == 1 ? 1 : N / 2);
  }
}

TEST(SMALL_TEST, COMPLETE){
  for (int N = 1; N <= NMAX; N++){
    IndependentSet<NMAX> iset;
    vector<vector<int> > G(N, vector<int>(N, 0));
    for (int i = 0; i < N; i++){
      for (int j = 0; j < i; j++){
        iset.AddEdge(i, j);
      }
    }
    ASSERT_EQ(iset.Solve(N), 1);
  }
}

TEST(SMALL_TEST, RANDOM){
  const int Q = 30;
  const int N = 16;
  
  for (int q = 0; q < Q; q++){
    vector<vector<int> > G = generate_graph(N, 0.3);
    IndependentSet<N> iset;
    for (int i = 0; i < N; i++){
      for (int j = 0; j < i; j++){
        if (G[i][j]) iset.AddEdge(i, j);
      }
    }
    ASSERT_EQ(brute_force(G), iset.Solve(N));
  }
}

TEST(LARGE_TEST, SPARSE){
  const int Q = 10;
  const int N = 50;
  for (int q = 0; q < Q; q++){
    vector<vector<int> > G = generate_graph(N, 0.3);
    IndependentSet<N> iset;
    for (int i = 0; i < N; i++){
      for (int j = 0; j < i; j++){
        if (G[i][j]) iset.AddEdge(i, j);
      }
    }
    iset.Solve(N);
  }
}

TEST(LARGE_TEST, dense){
  const int Q = 10;
  const int N = 100;
  for (int q = 0; q < Q; q++){
    vector<vector<int> > G = generate_graph(N, 0.7);
    IndependentSet<N> iset;
    for (int i = 0; i < N; i++){
      for (int j = 0; j < i; j++){
        if (G[i][j]) iset.AddEdge(i, j);
      }
    }
    iset.Solve(N);
  }
}

int main(int argc, char **argv){
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

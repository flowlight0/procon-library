#include <string>
#include <cstdlib>
#include <gtest/gtest.h>
#include <boost/random.hpp>
#include <boost/random/uniform_real.hpp>
#include "strong_connected_components.hpp"
using namespace std;

typedef vector<vector<int> > Graph;

Graph line_graph(int N){
    Graph g(N);
    for (int i = 0; i < N - 1; i++){
        g[i].push_back(i + 1);
    }
    return g;
}

Graph circle_graph(int N){
    Graph g(N);
    for (int i = 0; i < N; i++){
        g[i].push_back((i + 1) % N);
    }
    return g;
}

Graph random_graph(int N, double p){
    Graph g(N);
    mt19937 gen(0);
    boost::uniform_real<> dst(0, 1);
    boost::variate_generator<mt19937&, boost::uniform_real<> > rand(gen, dst);
    
    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
            if (rand() < p){
                g[i].push_back(j);
            }
        }
    }
    return g;
}

TEST(SCC_TEST, LINE_SMALL){
    const int n = 10;
    Graph g = line_graph(n);
    StrongConnectedComponent scc(g);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < i; j++)
            ASSERT_NE(scc[i], scc[j]);
}
        
 TEST(SCC_TEST, LINE_LARGE){
    const int n = 1000000;
    const int q = 1000000;
    Graph g = line_graph(n);
    StrongConnectedComponent scc(g);
    for (int i = 0; i < q; i++){
        int s = rand() % n;
        int t = rand() % n;
        if (s != t) ASSERT_NE(scc[s], scc[t]);
    }
}

TEST(SCC_TEST, CIRCLE_SMALL){
    const int n = 10;
    Graph g = circle_graph(n);
    StrongConnectedComponent scc(g);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < i; j++)
            ASSERT_EQ(scc[i], scc[j]);
}

TEST(SCC_TEST, CIRCLE_LARGE){
    const int n = 1000000;
    const int q = 1000000;
    Graph g = circle_graph(n);
    StrongConnectedComponent scc(g);
    for (int i = 0; i < q; i++){
        int s = rand() % n;
        int t = rand() % n;
        if (s != t) ASSERT_EQ(scc[s], scc[t]);
    }
}

void random_graph_verify(int n, const Graph &g){
    vector<vector<int> > adj(n, vector<int>(n, 0));
    for (int i = 0; i < n; i++) adj[i][i] = 1;
    
    for (int v = 0; v < n; v++){
        for (int w : g[v]) adj[v][w] = 1;
    }

    for (int k = 0; k < n; k++){
        for (int i = 0; i < n; i++){
            for (int j = 0; j < n; j++){
                adj[i][j] |= adj[i][k] & adj[k][j];
            }
        }
    }
    
    StrongConnectedComponent scc(g);
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            ASSERT_EQ(scc[i] == scc[j], adj[i][j] && adj[j][i]);
        }
    }
}

TEST(SCC_TEST, RANDOM_SMALL_SPARSE){
    random_graph_verify(10, random_graph(10, 0.2));
}

TEST(SCC_TEST, RANDOM_SMALL_DENSE){
    random_graph_verify(10, random_graph(10, 0.8));
}


TEST(SCC_TEST, RANDOM_MIDDLE_SPARSE){
    random_graph_verify(1000, random_graph(1000, 0.15));
}

TEST(SCC_TEST, RANDOM_MIDDLE_DENSE){
    random_graph_verify(1000, random_graph(1000, 0.5));
}

int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

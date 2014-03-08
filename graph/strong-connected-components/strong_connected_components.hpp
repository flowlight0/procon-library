/***********************************************************
蟻本にのっている２回dfsをする方法
非再帰による実装
O(V + E)
***********************************************************/

#include <vector>
#include <cstdlib>
#include <algorithm>

class StrongConnectedComponent{
    typedef std::vector<std::vector<int> > Graph; // adjacency list
private:
    size_t V;
    std::vector<bool> visit;
    std::vector<int>  scc;
    
    void dfs(int v, std::vector<int> &vs, const Graph &G){
        visit[v] = true;
        for (size_t i = 0; i < G[v].size(); i++)
            if (!visit[G[v][i]]) dfs(G[v][i], vs, G);
        vs.push_back(v);
    }

    void rdfs(int v, int id, const Graph &G){
        visit [v] = true;
        scc   [v] = id;
        for (size_t i = 0; i < G[v].size(); i++)
            if (!visit[G[v][i]]) rdfs(G[v][i], id, G);
    }
    
public:
    StrongConnectedComponent(const Graph &G) : V(G.size()){
        visit .resize(V, false);
        scc   .resize(V, -1);
        Graph rG(V);
        for (size_t v = 0; v < V; v++)
            for (size_t i = 0; i < G[v].size(); i++)
                rG[G[v][i]].push_back(v);
        std::vector<int> vs;
        for (size_t v = 0; v < V; v++) if (!visit[v]) dfs(v, vs, G);
        std::reverse(vs.begin(), vs.end());
        std::fill(visit.begin(), visit.end(), false);
        int id = 0;
        for (size_t v = 0; v < V; v++) if (!visit[vs[v]]) rdfs(vs[v], id++, rG);
    }
    int operator[](size_t idx) const { return scc[idx]; }
};

/***********************************************************
 solution for 
http://uva.onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&page=show_problem&category=24&problem=183
***********************************************************/

#include <iostream>
#include <string>
#include <map>
using namespace std;

int main(){
    int n, m, t = 0;
    while(cin >> n >> m && n + m > 0){
        if (t++ > 0) cout << endl;
        
        vector<string> names(n);
        map<string, int> nameToNum;
        
        int id = 0;

        vector<vector<int> > G(n);
        while(m--){
            string a, b;
            cin >> a >> b;
            if (nameToNum.find(a) == nameToNum.end()){
                names[id] = a;
                nameToNum[a] = id++;
            }
            if (nameToNum.find(b) == nameToNum.end()){
                names[id] = b;
                nameToNum[b] = id++;
            }
            G[nameToNum[a]].push_back(nameToNum[b]);
        }
        
        map<int, vector<int> > circles;
        StrongConnectedComponent scc(G);
        
        for (int i = 0; i < n; i++) circles[scc[i]].push_back(i);
        
        cout << "Calling circles for data set " << t << ":" << endl;
        for (map<int, vector<int> >::iterator iter = circles.begin(); iter != circles.end(); iter++){
            vector<int> &vi = iter->second;
            for (size_t i = 0; i < vi.size(); i++){
                cout << names[vi[i]];
                cout << (i + 1 == vi.size() ? "\n" : ", ");
            }
        }
    }
}

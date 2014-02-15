/***********************************************************
CDCL-SAT solver for competitive programming
- Conflict Driven Clause Learning
- VSIDS

each variable is represented by integer from 1 to V
(where V is maximum absolute value in all clauses)

a positive literal x  is represented by x
a negative literal ~x is represented by -x
0 means an undefined literals (for implementation convenience)

member method
- bool Solve()                // return true if we find valid assignment
- void AddClause(vector<int>) // add a new clause
- vector<int> Model()         // return a valid assignment when Solve() == true

***********************************************************/
#ifndef GUARD_MINI2SAT
#define GUARD_MINI2SAT

#include <cstdlib>
#include <cassert>
#include <vector>
#include <cstdint>
#include <algorithm>
#include <map>
#include <set>
#include <iostream>

using namespace std;

class Mini2Sat{
public:
    struct Lit{
        int x;
        Lit() : x(-1) { }
        Lit(int v, bool sign = false){ this->x = v * 2 + sign;}
        bool operator==(Lit p) const { return x == p.x; }
        bool operator!=(Lit p) const { return x != p.x; }
        inline bool Sign() const { return x & 1; }
        inline int Var () const { return x >> 1; }
        inline int ToInt() const { return x; }
        friend inline Lit operator~(Lit p){ p.x ^= 1; return p; }
        friend inline Lit operator^(Lit p, bool b){ p.x ^= b; return p;}
        friend inline bool operator<(Lit p, Lit q){ return p.x < q.x; }
        friend ostream &operator<<(ostream &out, Lit p){
            if (p.Sign()) out << "~";
            out << p.Var();
            return out;
        }
    };
    
    struct LBool{
        uint8_t x;
        LBool(uint8_t v = 0) : x(v) {}
        bool  operator== (LBool b) const { return (b.x & x & 2) || b.x == x; }
        bool  operator!= (LBool b) const { return !(*this == b); }
        LBool operator^  (bool b ) const { return LBool(x^b); }
        inline int ToInt() const { return x; }
    };
    
    const LBool LFalse = LBool(0);
    const LBool LTrue  = LBool(1);
    const LBool LUndef = LBool(2);
    const Lit LitUndef = Lit();
    
    typedef vector<Lit> clause;
    
private:
    int   nVars;
    size_t qhead;
    vector<clause*> clauses;
    vector<clause*> learnts;
    vector<clause*> reason;
    vector<LBool>   assign;
    vector<int>     level;
    vector<bool>    seen;
    vector<Lit>     trail;
    vector<int>     trail_lim;
    
    inline LBool Value(int x) const { return assign[x]; }
    inline LBool Value(Lit p) const { return assign[p.Var()] ^ p.Sign(); }

    void Init(){
        for (auto c : clauses)
            for (auto l : *c)
                nVars = max(nVars, l.Var() + 1);
        qhead = 0;
        trail.clear();
        trail_lim.clear();
        
        assign .resize(nVars, LUndef);
        level  .resize(nVars, -1);
        reason .resize(nVars, NULL);
        seen   .resize(nVars, false);
    }
    
    
    void Assign(Lit p, clause *c){
        assert(Value(p) == LUndef);
        assign[p.Var()] = LBool(!p.Sign());
        reason[p.Var()] = c;
        level [p.Var()] = DecisionLevel();
        trail.push_back(p);
    }
    
    void CancelUntil(int level){
        assert(level < DecisionLevel());
        for (int c = trail.size() - 1; c >= trail_lim[level]; c--){
            int x     = trail[c].Var();
            assign[x] = LUndef;
            reason[x] = NULL;
        }
        trail.resize(trail_lim[level]);
        trail_lim.resize(level);
        qhead = trail.size();
    }
    
    
    inline int DecisionLevel(){ return trail_lim.size();}
    
    void Analyze(clause *confl, vector<Lit> &out, int &bt_level){
        int pathC    = 0;
        int index    = trail.size() - 1;
        Lit p        = LitUndef;
        bt_level = 0;
        out.clear();
        out.push_back(p);
        do{
            assert(confl != NULL);
            clause &c = *confl;
            for (size_t j = 0; j < c.size(); j++){
                Lit q = c[j];
                if (q != p && !seen[q.Var()] && level[q.Var()] > 0){
                    seen[q.Var()] = true;
                    if (level[q.Var()] >= DecisionLevel()){
                        pathC++;
                    } else{
                        out.push_back(q);
                        bt_level = max(bt_level, level[q.Var()]);
                    }            
                }
            }
            while (!seen[trail[index--].Var()]);
            p     = trail[index + 1];
            confl = reason[p.Var()];
            seen[p.Var()] = 0;
            pathC--;
        }while (pathC > 0);
        out[0] = ~p;
        for (auto l : out) seen[l.Var()] = false;
    }
    
    int NextVar(){
        for (auto c : clauses){
            bool ok = false;
            for (Lit l : *c)
                if (Value(l) == LTrue){
                    ok = true;
                    break;
                }
            if (ok) continue;
            for (Lit l : *c)
                if (Value(l) == LUndef)
                    return l.Var();
        }
        return -1;
    };
    
    void AddClause(const vector<Lit> &lits, bool learnt){
        clause *c = new clause;
        for (Lit l : lits) c->push_back(l);
        if (learnt)
            learnts.push_back(c);
        else
            clauses.push_back(c);
    }

    LBool BcpClause(clause *c, bool &updated){
        int rest = c->size();
        for (Lit l : *c) if (Value(l) == LFalse) rest--;
        if (rest == 0) return LFalse;
        if (rest >= 2) return LUndef;
        
        for (Lit l : *c)
            if (Value(l) == LUndef){
                Assign(l, c);
                updated = true;
            }
        return LTrue;
    }
    
    clause *Bcp(){
        bool updated = true;
        while (updated){
            updated = false;
            for (auto c: clauses)
                if (BcpClause(c, updated) == LFalse)
                    return c;
            for (auto c: learnts)
                if (BcpClause(c, updated) == LFalse)
                    return c;
        }
        return NULL;
    }
    
public:
    vector<bool>    model;
    ~Mini2Sat(){
        for (auto c : clauses) delete c;
        for (auto c : learnts) delete c;
    }
    
    void AddClause(const vector<int> &lits){
        clause *c = new clause;
        for (auto l : lits) c->push_back(l > 0 ? Lit(l) : ~Lit(-l));
        AddClause(*c, false);
        delete c;
    }
    
    bool Solve(){
        Init();
        for(;;){
            clause *confl = Bcp();
            if (confl != NULL){
                if (DecisionLevel() == 0) return false;
                int         bt_level;
                vector<Lit> learnt;
                Analyze(confl, learnt, bt_level);
                AddClause(learnt, true);
                CancelUntil(bt_level);
            } else {
                int next = NextVar();
                if (next == -1){
                    model.resize(nVars, false);
                    for (int v = 1; v < nVars; v++)
                        if (assign[v] == LTrue) model[v] = true;
                    return true;
                }
                trail_lim.push_back(trail.size());
                Assign(~Lit(next), NULL);
            }
        }
    }
};
#endif

/***********************************************************

    struct Watcher{
        clause *c;
        Lit blocker;
        Watcher(){};
        Watcher(clause *c, Lit blocker) : c(c), blocker(blocker) {}
    };
    vector<vector<Watcher> > watches;


void AddClause(const vector<Lit> &lits, bool learnt){
        if (!learnt){
            vector<Lit> qs = lits;
            std::sort(qs.begin(), qs.end());
            qs.erase(std::unique(qs.begin(), qs.end()), qs.end());
            for (size_t i = 0; i + 1 < qs.size(); i++)
                if (qs[i] == ~qs[i + 1])
                    return;
        }

        clause *c = (clause*)&lits;
        watches[~lits[0].ToInt()].push_back(Watcher(c, lits[1]));
        watches[~lits[1].ToInt()].push_back(Watcher(c, lits[0]));
        
    }

clause *Bcp(){
        clause *confl = NULL;
        while (qhead < trail.size()){
            Lit p = trail[qhead++];
            vector<Watcher> &ws = watches[p.ToInt()];
            
            size_t i = 0, j = 0;
            for (; i < ws.size(); i++){
                Lit b = ws[i].blocker;
                if (Value(b) == LTrue){ ws[j++] = ws[i++]; continue;}
                
                clause *cr        = ws[i].c;
                clause &c         = *cr;
                Lit     false_lit = ~p;
                if (c[0] == false_lit) swap(c[0], c[1]);
                assert(c[1] == false_lit);
                i++;
                
                Lit first = c[0];
                Watcher w(cr, first);
                if (first != b && Value(first) == LTrue){ ws[j++] = w; continue;}

                for (size_t k = 2; k < c.size(); k++){
                    if (Value(c[k]) != LFalse){
                        swap(c[1], c[k]);
                        watches[~c[1].ToInt()].push_back(w);
                        goto NextClause;
                    }
                }
                
                ws[j++] = w;
                if (Value(first) == LFalse){
                    confl = cr;
                    qhead = trail.size();
                    while (i < ws.size()) ws[j++] = ws[i++];
                } else{
                    Assign(first, cr);
                }
            NextClause: ;
            }
            ws.resize(j);
        }
        return confl;
    }
    

 ***********************************************************/





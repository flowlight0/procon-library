#include <gtest/gtest.h>
#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/foreach.hpp>

#include "Mini2Sat.hpp"
using namespace std;

TEST(SMALL_TEST, TEST0){
    vector<int> c0 = {-1, 2, 3};
    vector<int> c1 = {1, -2};
    vector<int> c2 = {2, 3};
    vector<int> c3 = {-3};
    Mini2Sat solver;
    solver.AddClause(c0);
    solver.AddClause(c1);
    solver.AddClause(c2);
    solver.AddClause(c3);
    
    ASSERT_TRUE(solver.Solve());
    ASSERT_EQ(solver.model[1], true);
    ASSERT_EQ(solver.model[2], true);
    ASSERT_EQ(solver.model[3], false);
}

TEST(SMALL_TEST, TEST1){
    vector<int> c0 = {-1, 2, 3};
    vector<int> c1 = {1, -2};
    vector<int> c2 = {2, 3};
    vector<int> c3 = {-3};
    vector<int> c4 = {-1, -2};
    Mini2Sat solver;
    solver.AddClause(c0);
    solver.AddClause(c1);
    solver.AddClause(c2);
    solver.AddClause(c3);
    solver.AddClause(c4);
    ASSERT_FALSE(solver.Solve());
}

void load_file(const string &file, Mini2Sat &solver){
    ifstream ifs(file);
    string str;

    while (getline(ifs, str)){
        if (str[0] == 'c' || str[0] == 'p') continue;
        int lit;
        vector<int> lits;
        istringstream iss(str);
        while (iss >> lit && lit != 0) lits.push_back(lit);
        solver.AddClause(lits);
    }
}

TEST(AIM_TEST, YES){
    namespace fs = boost::filesystem;
    fs::path dir("./aim_yes");
    BOOST_FOREACH(const fs::path& p, make_pair(fs::directory_iterator(dir),
                                               fs::directory_iterator())) {
        if (!fs::is_directory(p)){
            Mini2Sat solver;
            load_file(p.string(), solver);
            cerr << p.string() << endl;
            ASSERT_TRUE(solver.Solve());
        }
    }
}

TEST(AIM_TEST, NO){
    // https://sites.google.com/site/boostjp/tips/filesystem
    namespace fs = boost::filesystem;
    fs::path dir("./aim_no");
    BOOST_FOREACH(const fs::path& p, make_pair(fs::directory_iterator(dir),
                                               fs::directory_iterator())) {
        if (!fs::is_directory(p)){
            Mini2Sat solver;
            load_file(p.string(), solver);
            cerr << p.string() << endl;
            ASSERT_FALSE(solver.Solve());
        }
    }
}

int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

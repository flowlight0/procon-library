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

// TEST(AIM_TEST, YES){
//     namespace fs = boost::filesystem;
//     fs::path dir("./aim_yes");
//     BOOST_FOREACH(const fs::path& p, make_pair(fs::directory_iterator(dir),
//                                                fs::directory_iterator())) {
//         if (!fs::is_directory(p)){
//             Mini2Sat solver;
//             load_file(p.string(), solver);
//             cerr << p.string() << endl;
//             ASSERT_TRUE(solver.Solve());
//         }
//     }
// }

// TEST(AIM_TEST, NO){
//     // https://sites.google.com/site/boostjp/tips/filesystem
//     namespace fs = boost::filesystem;
//     fs::path dir("./aim_no");
//     BOOST_FOREACH(const fs::path& p, make_pair(fs::directory_iterator(dir),
//                                                fs::directory_iterator())) {
//         if (!fs::is_directory(p)){
//             Mini2Sat solver;
//             load_file(p.string(), solver);
//             cerr << p.string() << endl;
//             ASSERT_FALSE(solver.Solve());
//         }
//     }
// }

// TEST(SUDOKU_TEST, YES){
//     vector<string> board = {
//         "--A----C-----O-I",
//         "-J--A-B-P-CGF-H-",
//         "--D--F-I-E----P-",
//         "-G-EL-H----M-J--",
//         "----E----C--G---" , 
//         "-I--K-GA-B---E-J" , 
//         "D-GP--J-F----A--" , 
//         "-E---C-B--DP--O-" , 
//         "E--F-M--D--L-K-A" , 
//         "-C--------O-I-L-" , 
//         "H-P-C--F-A--B---" , 
//         "---G-OD---J----H" , 
//         "K---J----H-A-P-L" , 
//         "--B--P--E--K--A-" , 
//         "-H--B--K--FI-C--" , 
//         "--F---C--D--H-N-" , 
//     };
//     Mini2Sat solver;
//     int n = board.size();
//     for (int r = 0; r < n; r++){
//         for (int j = 0; j < n; j++){
//             for (int k = j + 1; k < n; k++){
//                 for (int m = 0; m < n; m++){
//                     int ac = (n * n) * m + (r * n + j);
//                     int bc = (n * n) * m + (r * n + k);
//                     solver.AddClause({-ac, -bc});
//                 }
//             }
//         }
//     }
    
//     for (int c = 0; c < n; c++){
//         for (int j = 0; j < n; j++){
//             for (int k = j + 1; k < n; k++){
//                 for (int m = 0; m < n; m++){
//                     int ac = (n * n) * m + (j * n + c);
//                     int bc = (n * n) * m + (k * n + c);
//                     solver.AddClause({-ac, -bc});
//                 }
//             }
//         }
//     }
    
//     int d = sqrt(n) + 0.5;
//     for (int r1 = 0; r1 < n; r1++){
//         for (int r2 = 0; r2 < n; r2++){
//             for (int c1 = 0; c1 < n; c1++){
//                 for (int c2 = 0; c2 < n; c2++){
//                     if (r1 / d != r2 / d || c1 / d != c2 / d) continue;
//                     if (r1 == r2 && c1 == c2) continue;
//                     for (int m = 0; m < n; m++){
//                         int ac = (n * n) * m + (r1 * n + c1);
//                         int bc = (n * n) * m + (r2 * n + c2);
//                         solver.AddClause({-ac, -bc});
//                     }
//                 }
//             }
//         }
//     }

//     for (int r = 0; r < n; r++){
//         for (int c = 0;  c < n; c++){
//             if (board[r][c] == '-'){
//                 vector<int> v(n);
//                 for (int m = 0; m < n; m++) v[m] = (n * n) * m + r * n + c;
//                 solver.AddClause(v);
//             } else {
//                 int m = board[r][c] - 'A';
//                 solver.AddClause({(n * n) * m + r * n + c});
//             }
//         }
//     }
//     ASSERT_TRUE(solver.Solve());

//     cout << endl;
//     for (int r = 0; r < n; r++){
//         for (int c = 0; c < n; c++){
//             for (int m = 0; m < n; m++){
//                 if (solver.model[(n * n) * m + r * n + c]){
//                     cout << (char)('A' + m);
//                 }
//             }
//         }
//         cout << endl;
//     }
//     cout << endl;
// }

int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

#include <gtest/gtest.h>
#include <string>
#include <cstdlib>
#include <sstream>
#include <Python.h>
#include "Fraction.hpp"
using namespace std;

TEST(ADD_TEST, SMALL){
    ASSERT_EQ(Fraction(3, 2) + Fraction(1, 2), Fraction(2, 1));
    ASSERT_EQ(Fraction(4, 5) + Fraction(1, 3), Fraction(17, 15));
    ASSERT_EQ(Fraction(3, 5) + Fraction(-2, 2), Fraction(-2, 5));
}

TEST(ADD_TEST, RANDOM){
    int a = 123412;
    int b = 978098;
    int c = 234;
    int d = 1234;
    Fraction A(a, b);
    Fraction B(c, d);
    Py_Initialize();
    
    // PyObject* f = Py_BuildValue("Fraction(i,i)", a, b);
    // assert(PyObject_GetAttrString(f, "numerator") != NULL);
    // long long  N = PyLong_AsLongLong(PyObject_GetAttrString(f, "numerator"));
    // long long  D = PyLong_AsLongLong(PyObject_GetAttrString(f, "denominator"));
    // cout << N << " " << D << endl;
    // PyObject* f2 = Py_BuildValue("Fraction(i,i)", c, d);
    // assert(f1 != NULL);
    // assert(f2 != NULL);    
    // PyObject* f  = PyNumber_Multiply(f1, f2);
    

    Py_Finalize();
    // ASSERT_EQ(A + B, Fraction(N, D));
}

TEST(SUB_TEST, SMALL){
    ASSERT_EQ(Fraction(3, 2) - Fraction(1, 2), Fraction(1, 1));
    ASSERT_EQ(Fraction(4, 5) - Fraction(1, 3), Fraction(7, 15));
    ASSERT_EQ(Fraction(3, 5) - Fraction(-2, 2), Fraction(8, 5));
}

TEST(MUL_TEST, SMALL){
    ASSERT_EQ(Fraction(3, 2) * Fraction(1, 2), Fraction(3, 4));
    ASSERT_EQ(Fraction(4, 5) * Fraction(1, 3), Fraction(4, 15));
    ASSERT_EQ(Fraction(3, 5) * Fraction(-2, 2), Fraction(3, -5));
}

TEST(DIV_TEST, SMALL){
    ASSERT_EQ(Fraction(3, 2) / Fraction(1, 2), Fraction(3, 1));
    ASSERT_EQ(Fraction(4, 5) / Fraction(1, 3), Fraction(12, 5));
    ASSERT_EQ(Fraction(3, 5) / Fraction(-2, 2), Fraction(-3, 5));
}

int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

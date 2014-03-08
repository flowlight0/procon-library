CXX = clang++ -std=c++11
CXXFLAGS = -g -Wall -Wextra -O3

HEADERS = $(wildcard *.hpp)
SRCS = test.cpp
OBJS = $(SRCS:.cpp=.o)
LIBS = -lgtest -lpthread 


test: $(OBJS) $(SRCS) 
	$(CXX) $(CXXFLAGS) $(OBJS) $(LIBS) -o $@

test.o: $(HEADERS)

.PHONY: check-syntax clean test_flymake.o

clean:
	rm -f test $(OBJS)

check-syntax:
	$(CXX) $(CXXFLAGS) -pedantic -fsyntax-only $(CHK_SOURCES)


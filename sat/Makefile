CXX = g++ -std=c++11
CXXFLAGS = -pg -Wall -Wextra -O3 

HEADERS = $(wildcard *.hpp)
SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)
LIBS = -lgtest -lpthread -lboost_system -lboost_filesystem


test: $(OBJS) $(SRCS) 
	$(CXX) $(CXXFLAGS) $(OBJS) $(LIBS) -o $@

test.o: $(HEADERS)

.PHONY: check-syntax clean test_flymake.o

clean:
	rm -f test $(OBJS)

# check-syntax:
# 	$(CXX) $(CXXFLAGS) -pedantic -fsyntax-only $(CHK_SOURCES)


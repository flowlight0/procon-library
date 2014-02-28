CXX = g++ -std=c++11
CXXFLAGS = -I/usr/include/python3.3m -I/usr/include/x86_64-linux-gnu/python3.3m  -Wno-unused-result -g -fstack-protector --param=ssp-buffer-size=4 -Wformat -Werror=format-security  -DNDEBUG -g -fwrapv -O3 -Wall

LDFLAGS = -L/usr/lib/python3.3/config-3.3m-x86_64-linux-gnu -L/usr/lib -lpthread -ldl -lgtest -lboost_system -lboost_filesystem -lutil -lm  -lpython3.3m -Xlinker -export-dynamic

HEADERS = $(wildcard *.hpp)
SRCS = test.cpp
OBJS = $(SRCS:.cpp=.o)

test: $(OBJS) $(SRCS) 
	$(CXX) $(CXXFLAGS) $(OBJS) -lgtest -lpthread -o $@ $(LDFLAGS)

test.o: $(HEADERS)

.PHONY: check-syntax clean test_flymake.o

clean:
	rm -f test $(OBJS)

check-syntax:
	$(CXX) $(CXXFLAGS) -pedantic -fsyntax-only $(CHK_SOURCES)


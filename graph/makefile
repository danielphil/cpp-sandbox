CC=clang++
CFLAGS=-c -Wall -MMD -MP -std=c++14 -Wno-c++98-compat -Wno-c++11-compat-pedantic \
 -Wno-float-equal -O3 -I ../googletest-release-1.8.0/googletest/include --system-header-prefix=gtest
LDFLAGS=-L ../googletest-release-1.8.0/googletest/make -lpthread
SOURCES=$(subst ,,$(wildcard *.cpp))

EXECUTABLE=test
OBJECTS=$(SOURCES:.cpp=.o)

#brackets.o: brackets.cpp
#	$(CC) $(CFLAGS) $< -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) $< -o $@

$(EXECUTABLE): $(OBJECTS) ../googletest-release-1.8.0/googletest/make/gtest_main.a
	$(CC) $(LDFLAGS) $^ -o $@

run: $(EXECUTABLE)
	./$(EXECUTABLE)

all: $(OBJECTS) $(EXECUTABLE)

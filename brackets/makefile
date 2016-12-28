CC=clang++
CFLAGS=-c -Weverything -MMD -MP -std=c++14 -Wno-c++98-compat -Wno-c++11-compat-pedantic \
 -Wno-float-equal -O3 -I ../googletest-release-1.8.0/googletest/include --system-header-prefix=gtest
LDFLAGS=-v -L ../googletest-release-1.8.0/googletest/make -lgtest_main
SOURCES=$(subst ,,$(wildcard *.cpp))

EXECUTABLE=test
OBJECTS=$(SOURCES:.cpp=.o)

#brackets.o: brackets.cpp
#	$(CC) $(CFLAGS) $< -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) $< -o $@

test: brackets.o
	$(CC) $(LDFLAGS) $^ -o $@

#$(EXECUTABLE): $(OBJECTS)
#	$(CC) $(LDFLAGS) $^ -o $@

all: $(OBJECTS) $(EXECUTABLE)
	@echo $(SOURCES)

#-include $(OBJECTS:.o=.d)



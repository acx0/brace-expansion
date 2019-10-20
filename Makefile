CXX = clang++
CXX_STD = -std=c++14
CXXFLAGS += $(CXX_STD) -Wall -Wextra -Wold-style-cast -Wshadow $(SANITIZE_FLAGS) #-g -fno-omit-frame-pointer #-O1

# set gtest include directories as system directories to prevent warnings in gtest headers
CPPFLAGS += -MMD -MP -isystem $(GTEST_DIR)/include -isystem $(GMOCK_DIR)/include
LDFLAGS += -pthread $(SANITIZE_FLAGS)

# available sanitizers: address,leak,memory,thread,undefined
# note: disable when using valgrind, certain sanitizers conflict
SANITIZE_FLAGS = -fsanitize=address,leak,undefined

VALGRIND_FLAGS = --leak-check=yes

GTEST_FLAGS = --gtest_color=yes
GTEST_ROOT = /usr/src/googletest
GTEST_DIR = $(GTEST_ROOT)/googletest
GMOCK_DIR = $(GTEST_ROOT)/googlemock
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h $(GTEST_DIR)/include/gtest/internal/*.h
GMOCK_HEADERS = $(GMOCK_DIR)/include/gmock/*.h $(GMOCK_DIR)/include/gmock/internal/*.h $(GTEST_HEADERS)
GTEST_SRC = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)
GMOCK_SRC = $(GMOCK_DIR)/src/*.cc $(GMOCK_HEADERS)
GTEST_OBJ = gtest-all.o gmock-all.o gmock_main.o

MAIN_SRC = main.cc
ALL_SRC = $(wildcard *.cc)
TEST_SRC = $(wildcard *_test.cc)
BIN_SRC = $(filter-out $(TEST_SRC), $(ALL_SRC))

BIN_OBJ = $(BIN_SRC:%.cc=%.o)
TEST_OBJ = $(TEST_SRC:%.cc=%.o)
TEST_DEP_OBJ = $(filter-out $(MAIN_SRC:%.cc=%.o), $(BIN_OBJ))

BIN = brace-expansion-bin
TEST_BIN = brace-expansion-unit-tests

$(BIN): $(BIN_OBJ)
	$(CXX) $^ $(LDFLAGS) -o $@

.PHONY: test
test: $(TEST_BIN)
	./$(TEST_BIN) $(GTEST_FLAGS) $(ARGS)

.PHONY: vtest
vtest: $(TEST_BIN)
	valgrind $(VALGRIND_FLAGS) ./$(TEST_BIN) $(GTEST_FLAGS) $(ARGS)

$(TEST_BIN): $(TEST_DEP_OBJ) $(TEST_OBJ) gmock_main.a
	$(CXX) $^ $(LDFLAGS) -o $@

# for using custom main function to run tests
# gmock.a: gmock-all.o gtest-all.o
# 	$(AR) $(ARFLAGS) $@ $^

# uses gtest provided main function to run tests
gmock_main.a: $(GTEST_OBJ)
	$(AR) $(ARFLAGS) $@ $^

gtest-all.o: $(GTEST_SRC)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -I$(GTEST_DIR) -c $(GTEST_DIR)/src/gtest-all.cc

gmock-all.o: $(GMOCK_SRC)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -I$(GTEST_DIR) -I$(GMOCK_DIR) -c $(GMOCK_DIR)/src/gmock-all.cc

gmock_main.o: $(GMOCK_SRC)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -I$(GTEST_DIR) -I$(GMOCK_DIR) -c $(GMOCK_DIR)/src/gmock_main.cc

# usage: make ARGS='<args>' run
.PHONY: run
run: $(BIN)
	./$(BIN) $(ARGS)

.PHONY: vrun
vrun: $(BIN)
	valgrind $(VALGRIND_FLAGS) ./$(BIN) $(ARGS)

.PHONY: clean
clean:
	rm -f $(BIN) $(BIN_OBJ) *.d $(TEST_BIN) $(TEST_OBJ) $(GTEST_OBJ) gmock_main.a

-include $(ALL_SRC:%.cc=%.d)

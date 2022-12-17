# Executable names:
EXE = project
TEST = test

# Rule for `all` (first/default rule):
project: $(EXE)
all: $(EXE) $(TEST) data

# Add all object files needed for compiling:
EXE_OBJ = src/main.o
OBJS = src/main.o src/core/graph.o src/core/edge.o src/core/vertex.o \
	src/algorithms/betweenness.o src/algorithms/bfs.o src/algorithms/dijkstras.o \
	src/utils/utils.o src/utils/argument_parser.o

AZURE_DATA_URL = https://storageaccountrgfuna61f.blob.core.windows.net/cs225
DATA_FILES = data.csv mini_data.csv tiny_data.csv 10mb_data.csv 30mb_data.csv

data:
	@for f in $(DATA_FILES); do curl $(AZURE_DATA_URL)/$${f} --output data/$${f}; done

# Compiler/linker comfig and object/depfile directory:
CXX = clang++
LD = clang++
OBJS_DIR = .objs
INC_DIR = include/

# -MMD and -MP asks clang++ to generate a .d file listing the headers used in the source code for use in the Make process.
#   -MMD: "Write a depfile containing user headers"
#   -MP : "Create phony target for each dependency (other than main file)"
#   (https://clang.llvm.org/docs/ClangCommandLineReference.html)
DEPFILE_FLAGS = -MMD -MP

# Provide lots of helpful warning/errors:
WARNINGS = -pedantic -Wall -Werror -Wfatal-errors -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function

# Flags for compile:
CXXFLAGS += $(CS225) -std=c++1y -stdlib=libc++ -O3 $(WARNINGS) $(DEPFILE_FLAGS) -g -c -pthread -I$(INC_DIR)

# Flags for linking:
LDFLAGS += $(CS225) -std=c++1y -stdlib=libc++ -lc++abi -pthread

# Rule for linking the final executable:
# - $(EXE) depends on all object files in $(OBJS)
# - `patsubst` function adds the directory name $(OBJS_DIR) before every object file
$(EXE): output_msg $(patsubst %.o, $(OBJS_DIR)/%.o, $(OBJS))
	$(LD) $(filter-out $<, $^) $(LDFLAGS) -o $@

# Ensure .objs/ exists:
$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR)
	@mkdir -p $(OBJS_DIR)/tests
	@mkdir -p $(OBJS_DIR)/src
	@mkdir -p $(OBJS_DIR)/src/core
	@mkdir -p $(OBJS_DIR)/src/utils
	@mkdir -p $(OBJS_DIR)/src/algorithms

# Rules for compiling source code.
# - Every object file is required by $(EXE)
# - Generates the rule requiring the .cpp file of the same name
$(OBJS_DIR)/%.o: %.cpp | $(OBJS_DIR)
	$(CXX) $(CXXFLAGS) $< -o $@

# Rules for compiling test suite.
# - Grab every .cpp file in tests/, compile them to .o files
# - Build the test program w/ catchmain.cpp from src
OBJS_TEST += $(filter-out $(EXE_OBJ), $(OBJS))
CPP_TEST = $(wildcard tests/*.cpp)
CPP_TEST += tests/catchmain.cpp
OBJS_TEST += $(CPP_TEST:.cpp=.o)

$(TEST): output_msg $(patsubst %.o, $(OBJS_DIR)/%.o, $(OBJS_TEST))
	$(LD) $(filter-out $<, $^) $(LDFLAGS) -o $@

# Additional dependencies for object files are included in the clang++
# generated .d files (from $(DEPFILE_FLAGS)):
-include $(OBJS_DIR)/*.d
-include $(OBJS_DIR)/src/*.d
-include $(OBJS_DIR)/tests/*.d

# Custom Clang version enforcement Makefile rule:
ccred=$(shell echo -e "\033[0;31m")
ccyellow=$(shell echo -e "\033[0;33m")
ccend=$(shell echo -e "\033[0m")

IS_EWS=$(shell hostname | grep "ews.illinois.edu") 
IS_CORRECT_CLANG=$(shell clang -v 2>&1 | grep "version 6")
ifneq ($(strip $(IS_EWS)),)
ifeq ($(strip $(IS_CORRECT_CLANG)),)
CLANG_VERSION_MSG = $(error $(ccred) On EWS, please run 'module load llvm/6.0.1' first when running this project. $(ccend))
endif
endif

output_msg: ; $(CLANG_VERSION_MSG)

# Standard C++ Makefile rules:
clean:
	rm -rf $(EXE) $(TEST) $(OBJS_DIR) $(CLEAN_RM) *.o *.d

tidy: clean
	rm -rf doc
	rm -f data/*data.csv

.PHONY: all tidy clean output_msg data

# $^	all dependencies
# $<	first dependency
# $@	output
# $(@D) target's directory
# .PHONY prevents searching for these target names in OS and perform action avery time

TARGET = novott37
OBJECT_DIR = bin
SOURCE_DIR = src
DOC_DIR = doc
DOC_CONFIG = Doxyfile
TEST_DIR = tests
TEST_FILE = tests
DEBUG_DIR = debug

CXX = g++
CXXFLAGS = -Wall -pedantic -std=c++17 -g -fsanitize="address"
LD = g++
LDFLAGS = -Wall -pedantic -std=c++17 -g -fsanitize="address"

HEADERS := $(wildcard $(SOURCE_DIR)/*.h) # finds all header files
SOURCES := $(wildcard $(SOURCE_DIR)/*.cpp) # finds all source files
OBJECTS := $(patsubst $(SOURCE_DIR)/%.cpp, $(OBJECT_DIR)/%.o, $(SOURCES)) # for every source file assign equivalent object file
DEPENDENCIES := $(OBJECTS:%.o=%.d) # for every object file assign dependency file

.PHONY: all
all: compile doc

.PHONY: run
run : compile
	./$(TARGET)

.PHONY: compile
compile: $(TARGET)

$(TARGET): $(OBJECT_DIR)/main
	cp $< $@

$(OBJECT_DIR)/main: $(OBJECTS)
	mkdir -p $(@D)
	$(LD) $(LDFLAGS) $^ -o $@

$(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -MMD -c $< -o $@

doc: Doxyfile $(HEADERS)
	mkdir -p $(DOC_DIR)
	doxygen $(DOC_CONFIG) > /dev/null

$(DEBUG_DIR)/$(TEST_FILE).o: $(TEST_DIR)/$(TEST_FILE).cpp
	mkdir -p $(DEBUG_DIR)
	$(CXX) $(CXXFLAGS) -MMD $< -c -o $(DEBUG_DIR)/$(TEST_FILE).o

.PHONY: test
test: $(DEBUG_DIR)/$(TEST_FILE).o $(filter-out $(OBJECT_DIR)/main.o, $(OBJECTS))
	$(CXX) $(CXXFLAGS) $^ -o $(DEBUG_DIR)/$@
	./$(DEBUG_DIR)/$@

.PHONY: clean
clean:
	rm -rf $(OBJECT_DIR)/ $(DOC_DIR) $(TARGET) $(DEBUG_DIR) *.o *.out

-include $(DEPENDENCIES)

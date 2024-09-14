TARGET = ./main
TESTS = ./tests
CXX = g++

SRC_DIR = ./modules/
INCLUDE_DIR = ./modules/
BIN_DIR = ./bin/

CXXFLAGS = -I$(INCLUDE_DIR)

SRC = $(wildcard $(SRC_DIR)*.cpp)

OBJ = $(patsubst $(SRC_DIR)%.cpp, $(BIN_DIR)%.o, $(SRC))

all : create_bin_dir $(TARGET)

test : create_bin_dir $(TESTS)

$(BIN_DIR)%.o : $(SRC_DIR)%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET) : $(OBJ) $(BIN_DIR)main.o
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BIN_DIR)main.o : main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp -o $@

$(TESTS) : $(OBJ) $(BIN_DIR)tests.o
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BIN_DIR)tests.o : tests.cpp
	$(CXX) $(CXXFLAGS) -c tests.cpp -o $@

create_bin_dir :
	@mkdir -p $(BIN_DIR)

clean:
	@rm $(TARGET)
	@rm -rf $(BIN_DIR)

clean_test:
	@rm $(TESTS)
	@rm -rf $(BIN_DIR)
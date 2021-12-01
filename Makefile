CXX       := g++
CXX_FLAGS := -std=c++17 -ggdb

BIN     := bin
SRC     := src
INC		:= -Iinclude -IC:\LIBRARIES

LIBRARIES   := 
EXECUTABLE  := main


all: $(BIN)/$(EXECUTABLE)

run: clean all
	clear
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp
	$(CXX) $(CXX_FLAGS) $(INC) $^ -o $@ $(LIBRARIES)

clean:
	-rm $(BIN)/*~~

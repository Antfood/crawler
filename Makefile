SRC=src
OBJ=obj
BIN=bin

TEST_OBJ=tests/obj
CFLAGS += -Wall -Wextra -g3 -Iinclude 
CPP_FLAGS = -Wpedantic -std=c++17 -Wcast-qual -Wnon-virtual-dtor -Woverloaded-virtual -Wold-style-cast

CC=clang++
TARGET=$(BIN)/crawler
RM=rm -rf

$(shell mkdir -p obj bin)
SRCS=$(wildcard $(SRC)/*.cpp)

OBJS=$(patsubst $(SRC)/%.cpp, $(OBJ)/%.o, $(SRCS))

all: $(TARGET)

run: ${TARGET}
	./${TARGET} 

test: ${TARGET}
	./${TARGET} --test

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(CFLAGS) $(CPP_FLAGS)

$(OBJ)/%.o: $(SRC)/%.cpp
	$(CC) $(CFLAGS) $(CPP_FLAGS) -c $< -o $@

clean:
	$(RM) $(TARGET) $(BIN)/*.dSYM $(OBJ)/*.o 

.PHONY:
	all run clean 

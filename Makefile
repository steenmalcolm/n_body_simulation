CC := g++
CFLAGS := -Wall -pedantic -std=c++14

TARGET := main.exe
SRC := main.cpp

INC := -I inc




$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(INC) -o $@ $^

clean:
	rm -rf *.exe
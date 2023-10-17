CC := g++
CFLAGS := -Wall -pedantic -std=c++14

TARGET := main.exe
SRC := main.cpp

INC := -I /usr/local/include
LIB :=  -lglfw.3 -L /Applications/Development/macOS/lib -lvulkan  



$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(LIB) -o $@ $^

clean:
	rm -rf *.exe
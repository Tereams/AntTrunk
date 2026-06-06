CXX = clang++
CXXFLAGS = -std=c++17 -Wall -Iinclude

SRC = \
	src/main.cpp \
	src/finance_app.cpp \
	src/cli.cpp \
	src/cli_utils.cpp

TARGET = build/AntTrunk

all:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET)

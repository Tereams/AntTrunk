CXX = clang++
CXXFLAGS = -std=c++17 -Wall -Iinclude

SRC = \
	src/main.cpp \
	src/Account.cpp \
	src/Transaction.cpp \
	src/FinanceLogic.cpp

TARGET = AntTrunk

all:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET)

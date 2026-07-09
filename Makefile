CXX = clang++
CXXFLAGS = -std=c++17 -Wall -Iinclude
LDFLAGS = -lsqlite3

SRC = \
	src/main.cpp \
	src/ui/mui_app.cpp \
	src/cli.cpp \
	src/cli_utils.cpp \
	src/account_db.cpp \
	src/transaction_db.cpp \
	src/recurring_transaction_db.cpp

TARGET = build/AntTrunk

all:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET)

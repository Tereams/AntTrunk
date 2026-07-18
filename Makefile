CXX = clang++

FTXUI_PREFIX := $(shell brew --prefix ftxui)

CXXFLAGS = \
	-std=c++17 \
	-Wall \
	-Iinclude \
	-I$(FTXUI_PREFIX)/include

LDFLAGS = \
	-lsqlite3 \
	-L$(FTXUI_PREFIX)/lib \
	-lftxui-component \
	-lftxui-dom \
	-lftxui-screen

SRC := $(shell find src -name "*.cpp")

TARGET = build/AntTrunk

all:
	mkdir -p build
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET)

CXX = clang++
CXXFLAGS = -std=c++20 -Isrc -Iexternal

SOURCES = $(wildcard src/*.cpp)
HEADERS = $(wildcard src/*.h) $(wildcard external/*.hpp)

mossworld: $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o mossworld

run: mossworld
	./mossworld

clean:
	rm -f mossworld

.PHONY: run clean
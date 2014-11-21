.PHONY: install clean clean-verbose lint test

FOLDER=/usr/local/include/src

SOURCES = $(wildcard examples/*.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

CFLAGS  = -Wall -Wextra -O2 -std=c++0x -I$(CURDIR) -pedantic -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wformat=2 -Winit-self -Wmissing-include-dirs -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wstrict-overflow=5 -Wswitch-default -Wundef -Werror
LDFLAGS = -lm

install:
	@if [ -a $(FOLDER) ]; then echo "Folder src already exists in \"/usr/local/include\", sorry but I don't know what to do, I hope it's me :-)" && exit -1; else exit 0; fi;
	@sudo ln -s $(CURDIR)/src/ /usr/local/include/src
	@sudo ln -s $(CURDIR)/inputGenerator.hpp /usr/local/include/inputGenerator.hpp
	@echo "Install OK"

clean:
	@mkdir tmp
	@mv examples/*.cpp tmp/
	@rm -f examples/*
	@mv tmp/* examples/
	@rm -rf tmp
	@rm -f src/*.gch
	@echo "Clean OK"

clean-verbose:
	@mkdir -v tmp
	@mv -v examples/*.cpp tmp/
	@rm -fv examples/*
	@mv -v tmp/* examples/
	@rm -rfv tmp
	@rm -fv src/*.gch
	@echo "Clean OK"

lint:
	find . -name "*.hpp" | xargs python2 cpplint.py --filter=-legal --counting=detailed

benchmark:
	$(CXX) $(CFLAGS) examples/benchmark.cpp -o examples/benchmark $(LDFLAGS)
	./examples/benchmark

%.o: %.cpp
	$(CXX) $(CFLAGS) $< -o $@ $(LDFLAGS)

test: $(OBJECTS) benchmark

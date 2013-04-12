.PHONY: install

FOLDER=/usr/local/include/src
SOURCES = examples/benchmark.cpp examples/include_install_test.cpp examples/random_bipartite_graphs.cpp examples/random_chains.cpp examples/random_graphs.cpp examples/random_numbers.cpp examples/random_strings.cpp examples/random_trees.cpp examples/random_vectors.cpp
OBJECTS = $(SOURCES:.cpp=.o)

CFLAGS  = -Wall -Wextra -O2 -std=c++0x -I$(CURDIR) -pedantic -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wformat=2 -Winit-self -Wmissing-include-dirs -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wstrict-overflow=5 -Wswitch-default -Wundef -Werror
LDFLAGS = -lm

install:
	@if [ -a $(FOLDER) ]; then echo "Folder src already exists in \"/usr/local/include\", sorry but I don't know what to do, I hope it's me :-)" && exit -1; else exit 0; fi;
	@sudo ln --symbolic $(CURDIR)/src/ /usr/local/include/src
	@sudo ln --symbolic $(CURDIR)/inputGenerator.hpp /usr/local/include/inputGenerator.hpp
	@echo "Install OK"

clean:
	@mkdir tmp
	@cp examples/*.cpp tmp/
	@rm -f examples/*
	@cp tmp/* examples/
	@rm -rf tmp
	@echo "Clean OK"

clean-verbose:
	@mkdir -v tmp
	@cp -v examples/*.cpp tmp/
	@rm -fv examples/*
	@cp -v tmp/* examples/
	@rm -rfv tmp
	@echo "Clean OK"

lint:
	find . -name "*.hpp" | xargs python2 cpplint.py --filter=-legal --counting=detailed

benchmark:

.cpp.o:
	$(CXX) $(CFLAGS) $< -o $@ $(LDFLAGS)

test: $(OBJECTS)

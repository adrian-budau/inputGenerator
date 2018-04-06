.PHONY: install clean clean-verbose lint test

FOLDER=/usr/local/include/src

SOURCES = $(wildcard examples/*.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

CFLAGS  = -Wall -Wextra -O2 -std=c++11 -I$(CURDIR) -pedantic -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wformat=2 -Winit-self -Wmissing-include-dirs -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wstrict-overflow=2 -Wswitch-default -Wundef -Werror -Iexamples/
LDFLAGS = -lm

install:
	@if [ -d $(FOLDER) ]; then echo "Folder src already exists in \"/usr/local/include\", sorry but I don't know what to do, I hope it's me :-)" && exit 1; else exit 0; fi;
	@sudo ln -s $(CURDIR)/src/ /usr/local/include/src
	@sudo ln -s $(CURDIR)/inputGenerator.hpp /usr/local/include/inputGenerator.hpp
	@echo "Install OK"

clean:
	@mkdir tmp
	@rm -rf examples/*.dSYM
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
	$(CXX) $(CFLAGS) -DINPUT_GENERATOR_DEBUG examples/benchmark.cpp -o bin/benchmark $(LDFLAGS)
	./bin/benchmark

countfefete:
	$(CXX) $(CFLAGS) -DINPUT_GENERATOR_DEBUG examples/countfefete.cpp -o bin/countfefete $(LDFLAGS)
	@./bin/countfefete 17 10000 1000000000 0
	@./bin/countfefete 18 40000 1000000000 0
	@./bin/countfefete 19 70000 1000000000 0
	@./bin/countfefete 20 100000 1000000000 0

%.o: %.cpp
	$(CXX) $(CFLAGS) $< -o $@ $(LDFLAGS)

test: $(OBJECTS) benchmark countfefete

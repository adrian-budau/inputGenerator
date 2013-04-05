.PHONY: install

FOLDER=/usr/local/include/src

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

.PHONY: install

current = $(CURDIR)

install:
	sudo ln --symbolic $(CURDIR)/src/ /usr/local/include/src
	sudo ln --symbolic $(CURDIR)/inputGenerator.hpp /usr/local/include/inputGenerator.hpp

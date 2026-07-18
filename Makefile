.PHONY: all clean test check build

all:
	$(MAKE) -C c all

clean:
	$(MAKE) -C c clean

test:
	$(MAKE) -C c test

check:
	$(MAKE) -C c check

build:
	$(MAKE) -C c tin

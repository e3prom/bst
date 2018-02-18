PREFIX=/usr/local

all:
	+$(MAKE) -C src
install:
	install -m 0755 src/bstrings $(PREFIX)/bin

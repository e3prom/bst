CC=gcc
CFLAGS=-c -Wall
LDFLAGS=
GIT=/usr/bin/git
GIT_VERSION=$(shell git describe --abbrev=4 --dirty --always --tags)

TARGET = bstrings
OBJECTS = $(SOURCES:.c=.o)
SOURCES = bstrings.c version.c

check:	check-if-git-exists
check-if-git-exists:	; @which git >/dev/null

all: $(SOURCES) $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

#version.c: check-if-git-exists ../.git/COMMIT_EDITMSG ../.git/HEAD
	cat version.in > version.c
	awk 'BEGIN {print "#include \"include/version.h\""}' >> version.c
	$(GIT) describe --abbrev=4 --always --tags | awk '{print "const char * program_version = \""$$0"\";"} END {}' >> version.c
	$(GIT) rev-parse HEAD | awk 'BEGIN {print "const char * program_build = \""$$0"\";"} END {}' >> version.c
	date --utc | awk 'BEGIN {} {print "const char * program_build_time = \""$$0"\";"} END {}' >> version.c

clean: 
	rm $(TARGET) $(OBJECTS)

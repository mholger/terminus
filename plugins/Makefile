
DIRS = gfiles
CC = gcc
CFLAGS = -shared -lc -fpic -g -Wall
OBJECTS = 
INCFLAGS = 
LDFLAGS = -Wl,-rpath,/usr/local/lib
#LIBS = -lncurses

%:
	$(MAKE) -C $@

all: plugins

plugins:
	-for d in $(DIRS); do $(MAKE) -C $$d; done

.SUFFIXES:	.c .cc .C .cpp .so

.c.so :
	$(CC) -o $@ $(CFLAGS) $< $(INCFLAGS)

count:
	wc *.c *.cc *.C *.cpp *.h *.hpp

clean:
	rm -f *.so
	-for d in $(DIRS); do $(MAKE) -C $$d clean; done

install:
	-for d in $(DIRS); do $(MAKE) -C $$d install; done

.PHONY: all
.PHONY: count
.PHONY: clean

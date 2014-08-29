
CC = gcc
CFLAGS = -g -Wall
OBJECTS = ansi.o bbs.o com.o init.o logging.o menu.o msgbase.o newuser.o plugins.o rbuf.o user.o userio.o utility.o version.o
INCFLAGS = 
LDFLAGS = -Wl,-export-dynamic,-rpath,/usr/local/lib -ldl -lpthread
#LIBS = -lncurses
CP = cp

export BBSHOME := /home/bbs
export PLUGHOME := ${BBSHOME}/terminus/plugins

all: bbs
	$(MAKE) -C plugins

$(OBJECTS): vars.h vardec.h strings.h

bbs: $(OBJECTS)
	$(CC) $(CFLAGS) $(INCFLAGS) -c version.c
	$(CC) -o bbs $(OBJECTS) $(LDFLAGS) $(LIBS)

.SUFFIXES:	.c .cc .C .cpp .o

.c.o :
	$(CC) -o $@ -c $(CFLAGS) $< $(INCFLAGS)

count:
	wc *.c *.cc *.C *.cpp *.h *.hpp

clean:
	rm -f *.o
	$(MAKE) -C plugins clean

install:
	$(CP) bbs ${BBSHOME}
	$(MAKE) -C plugins install
	
.PHONY: all
.PHONY: count
.PHONY: clean

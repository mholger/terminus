
CC = gcc
CFLAGS = -lc -fpic -g -Wall -c
OBJECTS = gfiles.so
INCFLAGS = 
LDFLAGS = -shared -Wl,-rpath,/usr/local/lib

CP = cp
#LIBS = -lncurses

all: $(OBJECTS)

gfiles: gfiles.so

.SUFFIXES:	.c .cc .C .cpp .so

.o.so :
	$(CC) -o $@ $(LDFLAGS) $< $(INCFLAGS)
	$(CP) $@ ..
	

.c.o :
	$(CC) -o $@ $(CFLAGS) $< $(INCFLAGS)

count:
	wc *.c *.cc *.C *.cpp *.h *.hpp

clean:
	rm -f *.o *.so

install:
	$(CP) $(OBJECTS) $(PLUGHOME)

.PHONY: all
.PHONY: count
.PHONY: clean

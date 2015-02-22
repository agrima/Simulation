CC =  /opt/csw/gcc3/bin/gcc
LDFLAGS =
CFLAGS =
CPPFLAGS =
MAKE = make

all: prob1

../lib/csim.a:
	(cd lib && $(MAKE))

prob1: prob1.c ../lib/csim.a
	$(CC) -Ilib $(LDFLAGS) $(CFLAGS) $(CPPFLAGS) -o prob1 prob1.c ../lib/csim.a -lm

clean:
	rm -f prob1

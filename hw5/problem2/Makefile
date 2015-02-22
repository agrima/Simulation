CC =  /opt/csw/gcc3/bin/gcc
LDFLAGS =
CFLAGS =
CPPFLAGS =
MAKE = make

all: prob2

../lib/csim.a:
	(cd lib && $(MAKE))

prob2: prob2.c ../lib/csim.a
	$(CC) -Ilib $(LDFLAGS) $(CFLAGS) $(CPPFLAGS) -o prob2 prob2.c ../lib/csim.a -lm

clean:
	rm -f prob2
